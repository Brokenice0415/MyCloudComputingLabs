#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "pool.h"

/* private */
int
is_thread_alive(pthread_t tid) {
	int kill_rc = pthread_kill(tid, 0);
	if (kill_rc == ESRCH) {
		return false;
	}
	return true;
}

/* private */
/* admin thread */
void *
admin_thread(void *threadpool) {
	threadpool_t *pool = (threadpool_t *)threadpool;
	while (!pool->shutdown) {
		//printf("admin ------- \n");
		sleep(DEFAULT_TIME);
		
		/* get pool status */
		pthread_mutex_lock(&(pool->lock));
		int queue_size = pool->queue_size;
		int live_thr_num = pool->live_thr_num;
		pthread_mutex_unlock(&(pool->lock));
		
		pthread_mutex_lock(&(pool->thread_counter));
		int busy_thr_num = pool->busy_thr_num;
		pthread_mutex_unlock(&(pool->thread_counter));
		
		//printf("admin -busy-%d- -live-%d- \n", busy_thr_num, live_thr_num);
		
		/* add new thread if needing*/
		if ((queue_size >= MIN_WAIT_TASK_NUM) &&
			(live_thr_num <= pool->max_thr_num)
			) {
			//printf("admin add------- \n");
			pthread_mutex_lock(&(pool->lock));
			int add = 0;
			
			for (int i = 0; 
				((i < pool->max_thr_num) && (add < DEFAULT_THREAD_NUM) && (pool->live_thr_num < pool-> max_thr_num));
				i++) {
				if ((pool->threads[i] == 0) || 
					!is_thread_alive(pool->threads[i])
					) {
					pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool);
					add ++;
					pool->live_thr_num ++;
					//printf("new thread ----- \n");
				}	
			}
				
			pthread_mutex_unlock(&(pool->lock));
		}
		
		if (((busy_thr_num * 2) < live_thr_num) &&
			(live_thr_num > pool->min_thr_num)
			) {
			pthread_mutex_lock(&(pool->lock));
			pool->wait_exit_thr_num = DEFAULT_THREAD_NUM;
			pthread_mutex_unlock(&(pool->lock));
			
			for (int i = 0; i < DEFAULT_THREAD_NUM; i++) {
				pthread_cond_signal(&(pool->queue_not_empty));
				//printf("admin clear --\n");
			}
		}
	}
	
	return NULL;
}

/* public */
/* create threadpool */
threadpool_t *
threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size) {
	threadpool_t *pool = NULL;
	do {
		/* malloc for TM */
		pool = 
			(threadpool_t *)malloc(sizeof(threadpool_t));
		if (pool == NULL) {
		//printf("ERROR: malloc threadpool false; \n");
		}
		/* init TM */
		pool->min_thr_num = min_thr_num;
		pool->max_thr_num = max_thr_num;
		pool->busy_thr_num = 0;
		pool->live_thr_num = min_thr_num;
		pool->wait_exit_thr_num = 0;
		pool->queue_front = 0;
		pool->queue_rear = 0;
		pool->queue_size = 0;
		pool->queue_max_size = queue_max_size;
		pool->shutdown = 0;
		
		/* malloc for threads in TM*/
		pool->threads = 
			(pthread_t *)malloc(sizeof(pthread_t) * max_thr_num);
		if (pool->threads == NULL) {
			//printf("ERROR: malloc threads false; \n");
			break;
		}
		memset(pool->threads, 0, sizeof(pthread_t) * max_thr_num);
		
		/* malloc for queue in TM */
		pool->task_queue = 
			(threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_max_size);
		if (pool->task_queue == NULL) {
			//printf("ERROR: malloc task queue false; \n");
			break;
		}
		memset(pool->task_queue, 0, sizeof(threadpool_task_t) * queue_max_size);
		
		/* init mutex and cond */
		if (pthread_mutex_init(&(pool->lock), NULL) != 0			||
			pthread_mutex_init(&(pool->thread_counter), NULL) != 0	||
			pthread_cond_init(&(pool->queue_not_full), NULL) != 0	||
			pthread_cond_init(&(pool->queue_not_empty), NULL) != 0	||
			pthread_cond_init(&(pool->queue_empty), NULL) != 0	
			) {
			//printf("ERROR: init mutex or cond false; \n");
			break;
		}
		
		/* start min threads */
		for(int i = 0; i < min_thr_num; i++) {
			pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool);
			//printf("start thread 0x%x... \n", (unsigned int)pool->threads[i]);
		}
		
		/* start manager thread */
		pthread_create(&(pool->admin_tid), NULL, admin_thread, (void *)pool);
		
		return pool;
	}while(0);
	
	/*else free space */
	threadpool_free(pool);
	return NULL;
}

/* public */
/* add a task into queue */
int 
threadpool_add_task(threadpool_t *pool, void *(*function)(void *arg), void *arg) {
	pthread_mutex_lock(&(pool->lock));
	
	while ((!pool->shutdown) && (pool->queue_size == pool->queue_max_size)) {
		pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
	}
	
	if (pool->shutdown) {
		pthread_mutex_unlock(&(pool->lock));
		return -1;
	}
	
	/* reset arg */
	if (pool->task_queue[pool->queue_rear].arg != NULL) {
		//free(pool->task_queue[pool->queue_rear].arg);
		pool->task_queue[pool->queue_rear].arg = NULL;
	}
	
	/* add */
	pool->task_queue[pool->queue_rear].function = function;
	pool->task_queue[pool->queue_rear].arg = arg;
	pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
	pool->queue_size ++;
	
	pthread_cond_signal(&(pool->queue_not_empty));
	pthread_mutex_unlock(&(pool->lock));
	
	return 0;
}

/* public */
/* execute a task in queue*/
void *
threadpool_thread(void *threadpool) {
	threadpool_t *pool = (threadpool_t *)threadpool;
	threadpool_task_t task;
	
	while (true) {
		pthread_mutex_lock(&(pool->lock));
		
		/* waiting if queue empty, else return */
		while ((!pool->shutdown) && (pool->queue_size == 0)) {
			//printf("thread 0x%x is waiting... \n", (unsigned int)pthread_self());
			pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
			
			/* recycle thread */
			if (pool->wait_exit_thr_num > 0) {
				pool->wait_exit_thr_num --;
				if (pool->live_thr_num > pool->min_thr_num) {
					//printf("thread 0x%x is exiting \n", (unsigned int)pthread_self());
					pool->live_thr_num --;
					pthread_mutex_unlock(&(pool->lock));
					pthread_exit(NULL);
				}
			}
		}
		
		if (pool->shutdown) {
			pthread_mutex_unlock(&(pool->lock));
			//printf("pool has been shutdown \n");
			pthread_exit(NULL);
		}
		
		/* dequeue */
		task.function = pool->task_queue[pool->queue_front].function;
		task.arg = pool->task_queue[pool->queue_front].arg;
		
		pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
		pool->queue_size --;
		
		/* notify all threads waiting on this cond*/
		/* make them complete*/
		pthread_cond_broadcast(&(pool->queue_not_full));
		
		pthread_mutex_unlock(&(pool->lock));
		
		/* start execute */
		//printf("thread 0x%x start working \n", (unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num ++;
		pthread_mutex_unlock(&(pool->thread_counter));
		
		/* do execute */
		(*(task.function))(task.arg);
		
		/* end execute */
		//printf("thread 0x%x end working \n", (unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num --;
		/* all joined */
		if(pool->busy_thr_num == 0) {
			pthread_cond_signal(&(pool->queue_empty));
		}
		pthread_mutex_unlock(&(pool->thread_counter));
	}
	
	pthread_exit(NULL);
}

/* public */
int
threadpool_destroy(threadpool_t *pool) {
	if (pool == NULL)
		return -1;
	pool->shutdown = 1;
	
	pthread_join(pool->admin_tid, NULL);
	
	for (int i = 0; i < pool->live_thr_num; i++) {
		pthread_cond_broadcast(&(pool->queue_not_empty));
	}
	
	for (int i = 0; i < pool->live_thr_num; i++) {
		pthread_join(pool->threads[i], NULL);
	}
	
	threadpool_free(pool);
	return 0;
}

/* public */
/* free pool */
int 
threadpool_free(threadpool_t *pool) {
	if (pool == NULL)
		return -1;
	if (pool->task_queue)
		free(pool->task_queue);
	if (pool->threads) {
		free(pool->threads);
		pthread_mutex_lock(&(pool->lock));
		pthread_mutex_destroy(&(pool->lock));
		pthread_mutex_lock(&(pool->thread_counter));
		pthread_mutex_destroy(&(pool->thread_counter));
		pthread_cond_destroy(&(pool->queue_not_empty));
		pthread_cond_destroy(&(pool->queue_not_full));
	}
	free(pool);
	pool = NULL;
	
	return 0;
}

/* public */
/* join */
int
threadpool_join(threadpool_t *threadpool) {
	threadpool_t *pool = threadpool;
	pthread_mutex_lock(&(pool->lock));
	pthread_cond_wait(&(pool->queue_empty), &(pool->lock));
	pthread_mutex_unlock(&(pool->lock));
	return 0;
}


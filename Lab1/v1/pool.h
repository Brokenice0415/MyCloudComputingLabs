#ifndef __POOL_H__
#define __POOL_H__

/* cycle time of admin */
const int DEFAULT_TIME = 5;
/* min num of waiting tasks*/
const int MIN_WAIT_TASK_NUM = 5;
/* num of added threads per adding*/
const int DEFAULT_THREAD_NUM = 5;

/* task */
typedef struct {
	void *(*function)(void *);
	void *arg;
} threadpool_task_t;

/* threadpool manager TM*/
struct threadpool_t {
	/* lock TM */
	pthread_mutex_t lock;
	/* lock num */
	pthread_mutex_t thread_counter;
	pthread_cond_t queue_not_full;
	pthread_cond_t queue_not_empty;
	pthread_cond_t queue_empty;
	
	pthread_t *threads;
	pthread_t admin_tid;
	threadpool_task_t *task_queue;
	
	/* config of pool */
	int min_thr_num;
	int max_thr_num;
	int live_thr_num;
	int busy_thr_num;
	int wait_exit_thr_num;
	
	/* config of task queue */
	int queue_front;
	int queue_rear;
	int queue_size;
	
	/* num of task in queue */
	int queue_max_size;
	
	/* status of pool */
	int shutdown;
};

/* private */
int
is_thread_alive(pthread_t tid);

/* private */
/* admin thread */
void *
admin_thread(void *threadpool);

/* public */
/* create threadpool */
threadpool_t *
threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);

/* public */
/* add a task into queue */
int 
threadpool_add_task(threadpool_t *pool, void *(*function)(void *arg), void *arg);

/* public */
/* execute a task in queue*/
void *
threadpool_thread(void *threadpool);

/* public */
int
threadpool_destroy(threadpool_t *pool);

/* public */
/* free pool */
int 
threadpool_free(threadpool_t *pool);

/* public */
/* join */
int
threadpool_join(threadpool_t *threadpool);

#endif

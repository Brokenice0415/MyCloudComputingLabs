#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <pthread.h>

#include "sudoku_arity.h"
#include "pool.h"
#include "sthread.h"

using namespace std;

threadpool_t *thp;

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
	bool show_time = false;
	int min_thread_num = 10, 
		max_thread_num = 100, 
		max_queue_num = 100;
	for(int j = 1; j < argc; j ++) {
		if (argv[j][0] == '-') {
			if (argv[j][1] == 't') {
				show_time = true;
				break;
			} else if (argv[j][1] == 'm') {
				j++;
				if (j >= argc) {
					printf("ERROR: num of min threads needs\n");
					return -1;
				} else {
					min_thread_num = atoi(argv[j]);
				}
				break;
			} else if (argv[j][1] == 'M') {
				j++;
				if (j >= argc) {
					printf("ERROR: num of max threads needs\n");
					return -1;
				} else {
					max_thread_num = atoi(argv[j]);
				}
				break;
			} else if (argv[j][1] == 'q') {
				j++;
				if (j >= argc) {
					printf("ERROR: num of max queues needs\n");
					return -1;
				} else {
					max_queue_num = atoi(argv[j]);
				}
				break;
			} else {
				printf("ERROR: unknown command: %s\n", argv[j]);
				return -1;
			}
		} else {
			printf("ERROR: unknown command: %s\n", argv[j]);
			return -1;
		}
		
	}
	
	thp = threadpool_create(min_thread_num, max_thread_num, max_queue_num);
	
	init_();
	
	char filename[100][100];
	int i = 0;
	
	while(scanf("%s", filename[i]) != EOF){
		if(show_time){
			int64_t start = now();
			
			threadpool_add_task(thp, read, (void*)filename[i++]);
			
			threadpool_join(thp);
			
			int64_t end = now();
			double sec = (end-start)/1000000.0;
			printf("%f sec\n", sec);
		} else {
			threadpool_add_task(thp, read, (void*)filename[i++]);
			
			threadpool_join(thp);
		}
	}

	char puzzle[128];
	for(int j = 0; j < i; j ++) {
		FILE* fp = fopen(filename[j], "r");
		while(fgets(puzzle, sizeof(char)*128, fp) != NULL) {
			printf("%s", puzzle);
		}
		fclose(fp);
	}
	
	threadpool_destroy(thp);

	return 0;
}


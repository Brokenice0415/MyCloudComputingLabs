#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <pthread.h>

#include "pool.h"
#include "thread.h"

using namespace std;

int total_solved = 0;
int total = 0;

threadpool_t *thp;

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
	thp = threadpool_create(10, 100, 100);
	
	init_();
	
	char filename[100];
	
	while(scanf("%s", filename) != EOF){
	
		threadpool_add_task(thp, read, (void*)filename);
		
	}
	
	threadpool_join(thp);
	
	threadpool_destroy(thp);

	return 0;
}


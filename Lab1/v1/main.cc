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

int main(int argc, char* argv[])
{
	thp = threadpool_create(10, 100, 100);
	
	init_();
	
	char filename[100];
	
	while(scanf("%s", filename) != EOF){
	
		threadpool_add_task(thp, read, (void*)filename);
		
		threadpool_join(thp);
		
	}
	
	threadpool_destroy(thp);

	return 0;
}

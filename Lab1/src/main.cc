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
	
	char filename[100][100];
	int i = 0;
	
	while(scanf("%s", filename[i]) != EOF){
	
		threadpool_add_task(thp, read, (void*)filename[i++]);
		
		threadpool_join(thp);
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


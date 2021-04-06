#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sudoku_arity.h"
#include "sthread.h"

using namespace std;

void init_(){
	sem_init(&total_guard, 0, 0);
	sem_init(&puzzle_guard, 0, 0);
	sem_init(&result_guard, 0, 0);
	total = 0;
	sequence = 0;
}

void *read(void* argv) {

	char *filename = (char*)argv;
	
	puzzle_t* puzzle = (puzzle_t*)malloc(sizeof(puzzle_t));
	
	FILE* fp = fopen(filename, "r");

	while (fgets(puzzle->p, sizeof(char)*128, fp) != NULL) {
		
		if (strlen(puzzle->p) >= N) {
					
			threadpool_add_task(thp, work, (void *)puzzle);
			sem_wait(&puzzle_guard);
			
			//waiting for thread-work to notify
			sem_wait(&total_guard);
			total += 1;

		}
		
	}
	
	fclose(fp);
	
	free(puzzle);
	
	return NULL;
}

void *work(void* args) {
	
	puzzle_t* puzzle = (puzzle_t*)args;
	int p[N];
	for(int i = 0; i < N; i++) {
		p[i] = puzzle->p[i] - '0';
		assert(0 <= p[i] && p[i] <= NUM);
	}
	
	sem_post(&puzzle_guard);
	
	Arity a(p);
	
	if (a.solve(0)) {
		
		if (!a.solved()){
		  	assert(0);
		}
		
		puzzle_t *result = a.get_result();
		
		//wait until this' sequence
		unique_lock<mutex> lock(m);
		cv.wait(lock, [&](){return total == sequence;});
		
		//notify thread-read
		sem_post(&total_guard);
		
		sequence += 1;
		
		/*
		FILE *fp = fopen(output, "a+");
	
		fprintf(fp, "%s\r\n", result->p);
		
		fclose(fp);
		*/
		printf("%s\r\n", result->p);
		
		cv.notify_all();
		
		free(result);
		
	}
	
	return NULL;
}

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "sudoku_arity.h"
#include "thread.h"

using namespace std;

void init_(){
	sem_init(&total_guard, 0, 1);
	sem_init(&solve_guard, 0, 1);
	sem_init(&puzzle_guard, 0, 0);
	sem_init(&result_guard, 0, 0);
	sem_init(&can_save, 0, 1);
}

void *read(void* argv) {
	
	FILE* fp = fopen((char*)argv, "r");
	  
  	puzzle_t* puzzle = (puzzle_t*)malloc(sizeof(puzzle_t));
  
	while (fgets(puzzle->p, sizeof(char)*128, fp) != NULL) {
		
		if (strlen(puzzle->p) >= N) {
					
			threadpool_add_task(thp, work, (void *)puzzle);
			sem_wait(&puzzle_guard);
			
			sem_wait(&total_guard);
			total += 1;
			sem_post(&total_guard);
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
	
		sem_wait(&solve_guard);
		total_solved += 1;
		sem_post(&solve_guard);
		
		if (!a.solved()){
		  	assert(0);
		}
		
		puzzle_t *result = a.get_result();
		
		/*
		threadpool_add_task(thp, save, (void *)(result));
		sem_wait(&result_guard);
		*/
		
		FILE *fp = fopen("solved", "a+");
	
		fprintf(fp, "%s\n", result->p);
		
		fclose(fp);
		
		free(result);
		
	}
	
	return NULL;
}

/*
void *save(void* args) {
	
	char result[82];
	memcpy(result, ((puzzle_t*)args)->p, sizeof(result));

	sem_post(&result_guard);
	
	result[81] = 0;
	
	FILE *fp = fopen("solved", "a+");
	
	fprintf(fp, "%s\n", result);
	
	fclose(fp);
	
	return NULL;
	
}
*/

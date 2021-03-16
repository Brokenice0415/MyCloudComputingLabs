#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <pthread.h>

#include "sudoku_basic.h"

pthread_mutex_t m;
pthread_mutex_t solved_m;

int total_solved = 0;
int total = 0;

struct puzzle_t{
	char p[128];
};

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

void *work(void* args) {
	puzzle_t* puzzle = (puzzle_t*)args;
	int p[N];
	for(int i = 0; i < N; i++) {
		p[i] = puzzle->p[i] - '0';
		assert(0 <= p[i] && p[i] <= NUM);
	}
	pthread_mutex_unlock(&m);
	
	Basic b(p);
	
	if (b.solve(0)) {
		pthread_mutex_lock(&solved_m);
		++total_solved;
		pthread_mutex_unlock(&solved_m);
		if (!b.solved()){
		  	assert(0);
		}
	}
	
	return NULL;
}

int main(int argc, char* argv[])
{
	
	pthread_mutex_init(&m, NULL);
	pthread_mutex_init(&solved_m, NULL);

	FILE* fp = fopen(argv[1], "r");

	pthread_t p[1010];
	  
	int64_t start = now();
  	puzzle_t* puzzle = (puzzle_t*)malloc(sizeof(puzzle_t));
  
	while (fgets(puzzle->p, sizeof(char)*128, fp) != NULL) {
		if (strlen(puzzle->p) >= N) {
					
			pthread_mutex_lock(&m);
			pthread_create(&p[total], NULL, work, (void *)puzzle);
			
			++total;
		}
	}

	for (int i = 0; i < total; i++) {
		pthread_join(p[i], NULL);
	}

	int64_t end = now();
	double sec = (end-start)/1000000.0;
	printf("%f sec %f ms each %d total %d\n", sec, 1000*sec/total, total_solved, total);

	return 0;
}


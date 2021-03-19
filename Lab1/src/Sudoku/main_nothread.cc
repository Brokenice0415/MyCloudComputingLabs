#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "sudoku_arity.h"

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

int main(int argc, char* argv[])
{

	FILE* fp = fopen(argv[1], "r");
	  
	int64_t start = now();
  	puzzle_t* puzzle = (puzzle_t*)malloc(sizeof(puzzle_t));
  
	while (fgets(puzzle->p, sizeof(char)*128, fp) != NULL) {
		if (strlen(puzzle->p) >= N) {

			int p[N];
			for(int i = 0; i < N; i++) {
				p[i] = puzzle->p[i] - '0';
				assert(0 <= p[i] && p[i] <= NUM);
			}
			
			Arity a(p);
			
			if (a.solve(0)) {
				++total_solved;
				if (!a.solved()){
				  	assert(0);
				}
			}
			
			++total;
		}
	}

	int64_t end = now();
	double sec = (end-start)/1000000.0;
	printf("%f sec %f ms each %d total %d\n", sec, 1000*sec/total, total_solved, total);

	return 0;
}


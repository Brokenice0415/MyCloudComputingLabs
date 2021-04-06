#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sudoku_arity.h"
#include "sthread.h"

using namespace std;

void init_(){
	sem_init(&work_guard, 0, 0);
	sem_init(&file_guard, 0, 1);
}

void *read(void* argv) {

	char *filename = (char*)argv;
	
	file_t *f = (file_t*)malloc(sizeof(file_t));
	
	int i = 0;
	while(filename[i]!='\0'){
		f->name[i] = filename[i];
		i++;
	}
	f->name[i] = '\0';
	
	char tmp[128];
	
	FILE* fp = fopen(filename, "r+");
	
	f->pos = ftell(fp);

	while (fgets(tmp, sizeof(char)*128, fp) != NULL){
			
		threadpool_add_task(thp, work, (void *)f);
		sem_wait(&work_guard);
		f->pos = ftell(fp);
		
	}
	
	fclose(fp);
	
	free(f);
	
	return NULL;
}

void *work(void* args) {

	file_t *f = (file_t*)args;
	
	FILE* fp = fopen(f->name, "r+");
	
	int pos = f->pos;
	
	sem_post(&work_guard);
	
	char puzzle[128];
	
	sem_wait(&file_guard);
	fseek(fp, pos, 0);
	assert(fgets(puzzle, sizeof(char)*128, fp) != NULL);
	sem_post(&file_guard);

	int p[N];
	for(int i = 0; i < N; i++) {
		p[i] = puzzle[i] - '0';
		assert(0 <= p[i] && p[i] <= NUM);
	}
	
	
	Arity a(p);
	
	if (a.solve(0)) {
		
		if (!a.solved()){
		  	assert(0);
		}
		
		puzzle_t *result = a.get_result();
		
		sem_wait(&file_guard);
		//back to read position
		fseek(fp, pos, 0);
		
		fprintf(fp, "%s\r", result->p);
		sem_post(&file_guard);
		
		fclose(fp);

		
		free(result);
		
	}
	
	return NULL;
}

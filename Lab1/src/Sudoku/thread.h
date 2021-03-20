#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <semaphore.h>

#include "pool.h"

extern int total_solved;
extern int total;
extern threadpool_t *thp;

static sem_t total_guard, solve_guard, puzzle_guard, result_guard, can_work, can_save;

void init_();

void *read(void *args);
void *work(void *args);
void *save(void *args);

#endif

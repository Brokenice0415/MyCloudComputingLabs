#ifndef STHREAD_H
#define STHREAD_H

#include <pthread.h>
#include <semaphore.h>

#include <thread>

#include "pool.h"

extern threadpool_t *thp;

static sem_t work_guard, file_guard;

void init_();

void *read(void *args);
void *work(void *args);

#endif

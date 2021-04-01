#ifndef STHREAD_H
#define STHREAD_H

#include <pthread.h>
#include <semaphore.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "pool.h"

extern threadpool_t *thp;

static sem_t total_guard, puzzle_guard, result_guard;
static std::condition_variable cv;
static std::mutex m;

static int total;
static int sequence;

static char output[100] = "solved";

void init_();

void *read(void *args);
void *work(void *args);

#endif

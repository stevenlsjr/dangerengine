/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 9/12/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSWORKQUEUE_H
#define DANGERENGINE_SLSWORKQUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include "slsmacros.h"

#ifndef SLS_N_WORK_THREADS
#   define SLS_N_WORK_THREADS 10
#endif // !SLS_N_WORK_THREADS

typedef struct slsWorkScheduler slsWorkScheduler;
typedef struct slsTask slsTask;
typedef struct slsTaskQueue slsTaskQueue;

struct slsTaskQueue {
  slsTask *head;
  slsTask *end;
};

typedef void *(*slsWorkFn)(slsTask *task, void *user_data);

struct slsWorkScheduler {
  pthread_t gl_thread;
  pthread_t threads[SLS_N_WORK_THREADS];

  slsTaskQueue waiting;
};

struct slsTask {
  slsWorkFn fn;
  void *pdata;
  void *rdata;

  slsTask *next;
};

slsWorkScheduler *sls_workscheduler_init(slsWorkScheduler *self) SLS_NONNULL(1);
slsWorkScheduler *sls_workscheduler_dtor(slsWorkScheduler *self) SLS_NONNULL(1);

slsTask * sls_make_task(slsWorkFn fn, void *data);

void sls_task_enqueue(slsTaskQueue *queue, slsTask *task)  SLS_NONNULL(1);
slsTask *sls_task_dequeue(slsTaskQueue *queue) SLS_NONNULL(1);



#endif //DANGERENGINE_SLSWORKQUEUE_H

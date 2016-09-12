/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 9/12/16, Steven
 * 
 **/
#include "slsworkqueue.h"

/**
 * @brief: finds the last node in list given by queue->head
 * @param queue: queue with a head node in place
 */
static void queue_reset_nodes(slsTaskQueue *queue);

slsWorkScheduler *sls_workscheduler_init(slsWorkScheduler *self)
{
  *self = (slsWorkScheduler) {};
  return self;
}

slsWorkScheduler *sls_workscheduler_dtor(slsWorkScheduler *self)
{
  slsTaskQueue *q = &self->waiting;
  slsTask *iter = q->head;
  while (iter) {
    slsTask *next = iter->next;
    free(iter);

    iter = next;
  }
  return self;
}

slsTask *sls_make_task(slsWorkFn fn, void *data)
{
  slsTask *self = malloc(sizeof(slsTask));
  sls_checkmem(self);
  self->fn = fn;
  self->pdata = data;

  self->next = NULL;

  return self;
error:
  assert(0);
  return NULL;
}

void sls_task_enqueue(slsTaskQueue *queue, slsTask *task)
{

  if (!queue->head && !queue->end) {
    queue->head = task;
    queue->end = task;
    return;
  } else if (queue->head && queue->end) {
    // enqueue into END of list
    slsTask *old_end = queue->end;
    old_end->next = task;
    queue->end = task;
  } else {
    queue_reset_nodes(queue);
    if (!queue->head || !queue->end) { sls_log_fatal("could not construct queue"); }
    sls_task_enqueue(queue, task);
  }

}

slsTask *sls_task_dequeue(slsTaskQueue *queue)
{
  if (!queue->head && !queue->end) {
    return NULL;
  } else if (queue->head && queue->end) {

    slsTask *deq = queue->head;
    // dequeue from HEAD of list
    queue->head = deq->next;
    deq->next = NULL;

    if (deq == queue->end) {
      queue->end = NULL;
      queue->head = NULL;
    }
    return deq;

  } else {
    queue_reset_nodes(queue);
    if (!queue->head || !queue->end) { sls_log_fatal("could not construct queue"); }
    return sls_task_dequeue(queue);
  }
}

void queue_reset_nodes(slsTaskQueue *queue)
{
  if (!queue->head) { return; }
  for (slsTask *itor = queue->head; itor; itor = itor->next) {
    if (!itor->next) {
      queue->end = itor;
    }
  }

}

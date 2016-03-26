/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSPOOL_H
#define DANGERENGINE_SLSPOOL_H

#include <stdlib.h>
#include <slsmacros.h>
#include "intrusivelist.h"
#include "callbacks.h"

typedef struct slsPool slsPool;
typedef struct slsReleaseUnit slsReleaseUnit;

struct slsReleaseUnit {
  slsFreeFn free_fn;
  void *data;
  slsReleaseUnit *next;
};

/**
 * @brief a memory pool type
 * for preserving memory allocations.
 * @detail As an added feature, the pool should
 * also be able to register generic cleanup callbacks
 * to make this a general-purpose autorelease pool
 */
struct slsPool {


  slsPool *parent;
  slsPool *child_top;

  slsReleaseUnit *release_list;

  size_t unit_size;
  size_t block_size;

  SLS_INTRUSIVE_FIELDS(slsPool, siblings);

};

slsPool *sls_pool_create(slsPool *parent) SLS_NONNULL(1);

slsPool *sls_pool_clear(slsPool *pool) SLS_NONNULL(1);

slsPool *sls_pool_delete(slsPool *pool) SLS_NONNULL(1);

void *sls_pool_register_freefn(slsFreeFn *fn, void *data);

void *sls_palloc(slsPool *pool, size_t size) SLS_NONNULL(1);

void *sls_palloc_arena_hint(slsPool *pool, size_t size, size_t arena_hint) SLS_NONNULL(1);


#endif //DANGERENGINE_SLSPOOL_H

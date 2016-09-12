/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSPOOL_H
#define DANGERENGINE_SLSPOOL_H

#include "callbacks.h"
#include "intrusivelist.h"
#include <slsmacros.h>
#include <stdlib.h>

typedef struct slsPool slsPool;
typedef struct slsPool_p slsPool_p;
typedef struct slsReleaseUnit slsReleaseUnit;

struct slsReleaseUnit {
  slsFreeFn free_fn;
  void *data;
};

/**
 * @brief An all-purpose memory management
 * structure, comparable to APR's apr_pool_t.
 * TODO: support both auto-release callbacks and pre-allocated memory arenas
 */
struct slsPool {

  slsPool *parent;
  slsPool_p *priv;

  size_t unit_size;
  size_t block_size;

  SLS_INTRUSIVE_FIELDS(slsPool) siblings;

};

slsPool *sls_pool_create(slsPool *parent);

slsPool *sls_pool_clear(slsPool *pool) SLS_NONNULL(1);

slsPool *sls_pool_delete(slsPool *pool) SLS_NONNULL(1);

void *sls_pool_register_freefn(slsFreeFn *fn, void *data);

void *sls_palloc(slsPool *pool, size_t size) SLS_NONNULL(1);



#endif //DANGERENGINE_SLSPOOL_H

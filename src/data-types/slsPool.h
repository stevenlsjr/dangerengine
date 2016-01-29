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

typedef struct slsPool slsPool;

slsPool *sls_pool_create(slsPool *parent) SLS_NONNULL(1);
slsPool *sls_pool_clear(slsPool *pool) SLS_NONNULL(1);
slsPool *sls_pool_delete(slsPool *pool) SLS_NONNULL(1);

void *sls_palloc(slsPool *pool, size_t size) SLS_NONNULL(1);
void *sls_palloc_arena_hint(slsPool *pool, size_t size, size_t arena_hint) SLS_NONNULL(1);




#endif //DANGERENGINE_SLSPOOL_H

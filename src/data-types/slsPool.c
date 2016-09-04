/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 *
 **/

#include "slsPool.h"

struct slsPool_p {
};

static void sls_pool_insertchild(slsPool* parent, slsPool* child);

static slsPool* sls_pool_unlink(slsPool* pool);

slsPool*
sls_pool_create(slsPool* parent)
{
  return NULL;
}

static void
sls_pool_insertchild(slsPool* parent, slsPool* child)
{
}

slsPool*
sls_pool_clear(slsPool* pool)
{
  return NULL;
}

slsPool*
sls_pool_delete(slsPool* pool)
{
  return NULL;
}

static slsPool*
sls_pool_unlink(slsPool* pool)
{
  return pool;
}

void*
sls_palloc(slsPool* pool, size_t size)
{
  return NULL;
}

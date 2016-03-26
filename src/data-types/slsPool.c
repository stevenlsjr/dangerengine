/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 * 
 **/


#include "slsPool.h"
#include <stdlib.h>
#include <slsmacros.h>
#include <limits.h>

static
void sls_pool_insertchild(slsPool *parent, slsPool *child);
static
slsPool *sls_pool_unlink(slsPool *pool);


slsPool *sls_pool_create(slsPool *parent)
{
  slsPool *pool = calloc(1, sizeof(slsPool));
  sls_checkmem(pool);

  pool->parent = parent;
  if (parent) {
    sls_pool_insertchild(parent, pool);
  }


  return pool;

  error:
  if (pool) { sls_pool_delete(pool); }
  return NULL;
}

static
void sls_pool_insertchild(slsPool *parent, slsPool *child)
{
  child->siblings.next = NULL;
  child->siblings.prev = NULL;
  if (!parent->child_top) {
    parent->child_top = child;
    return;
  }
  slsPool *second = parent->child_top;
  parent->child_top = child;
  child->siblings.next = second;
  second->siblings.prev = child;


}

slsPool *sls_pool_clear(slsPool *pool)
{
  while (pool->child_top) {
    slsPool *top = pool->child_top;
    pool->child_top = top->siblings.next;
    sls_pool_delete(top);
  }

  // TODO: signal memory availible
  return pool;
}

slsPool *sls_pool_delete(slsPool *pool)
{
  sls_pool_unlink(pool);
  sls_pool_clear(pool);
#if 0
  for (int i=0; i<N_POOL_ARENAS; ++i) {
    if (pool->arenas[i]) {free(pool->arenas[i]); pool->arenas[i] = NULL;}
  }

#endif


  free(pool);
  return NULL;
}
static
slsPool *sls_pool_unlink(slsPool *pool)
{
  slsPool *pprev = pool->siblings.prev;
  slsPool *nnext = pool->siblings.next;

  if (pprev) {
    pprev->siblings.next = nnext;
  }
  if (nnext) {
    nnext->siblings.prev = pprev;
  }
  pool->siblings.next = NULL;
  pool->siblings.prev = NULL;

  pool->parent = NULL;

  return  pool;
}


void *sls_palloc(slsPool *pool, size_t size)
{
  return sls_palloc_arena_hint(pool, size, 0);
}

void *sls_palloc_arena_hint(slsPool *self, size_t size, size_t arena_hint)
{

  assert(size > 0);
  void *ptr = NULL;

  if (arena_hint < 2 * size) {
    arena_hint = size * 4;
  }

  bool found = false;
  // find valid region
#if 0
  for (int i=0; i<N_POOL_ARENAS; ++i) {
    if (!self->arenas[i]) {

      self->arenas[i] = calloc(arena_hint, sizeof(char));
      ptr = self->arenas[i];
      self->arena_alloc_size[i] = arena_hint;
      self->mem_used[i] = size;
      found = true;

      break;
    } else if (self->arena_alloc_size[i] > self->mem_used[i] + size){

      ptr = self->arenas[i] + self->mem_used[i];
      self->mem_used[i] += size;
      found = true;
      break;
    }

  }

#endif


  if (!ptr) {
    slsPool *next = sls_pool_create(self);
    ptr = sls_palloc_arena_hint(next, size, arena_hint);
  }


  return ptr;
}

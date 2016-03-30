/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 * 
 **/


#include "slsPool.h"
#include <stretchy_buffer.h>

struct slsPool_p {
  slsPool **children_sb;
  slsReleaseUnit *release_unit_sb;

};

static
void sls_pool_insertchild(slsPool *parent, slsPool *child);

static
slsPool *sls_pool_unlink(slsPool *pool);


slsPool *sls_pool_create(slsPool *parent)
{
  slsPool *pool = calloc(1, sizeof(slsPool));
  sls_checkmem(pool);

  pool->priv = calloc(1, sizeof(slsPool_p));
  pool->priv->children_sb = NULL;
  pool->priv->release_unit_sb = NULL;

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
  slsPool_p *priv = parent->priv;
      sb_push(priv->children_sb, child);

}

slsPool *sls_pool_clear(slsPool *pool)
{
  slsPool_p *priv = pool->priv;
  assert(priv);

  int len;
  if (priv->children_sb) {
    len = sb_count(priv->children_sb);
    assert(len >= 0);
    for (int i = 0; i < len; ++i) {
      sls_pool_delete(priv->children_sb[i]);
    }
        sb_free(priv->children_sb);

  }

  if (priv->release_unit_sb) {
    len = sb_count(priv->release_unit_sb);

    for (int i = 0; i < len; ++i) {
      slsReleaseUnit *ru = priv->release_unit_sb + i;
      if (ru->data && ru->free_fn) {
        ru->free_fn(ru->data);
      }
    }
    sb_free(priv->release_unit_sb);
    priv->children_sb = NULL;

  }


  return pool;
}

slsPool *sls_pool_delete(slsPool *pool)
{
  sls_pool_clear(pool);

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

  return pool;
}


void *sls_palloc(slsPool *pool, size_t size)
{
  slsPool_p *p = pool->priv;
  assert(p);
  // TODO prefer using memory units
  slsReleaseUnit ru = {
      .free_fn = free,
      .data = malloc(size)
  };

      sb_push(p->release_unit_sb, ru);

  return ru.data;
}


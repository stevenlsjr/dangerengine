/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/23/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSPOOL_P_H
#define DANGERENGINE_SLSPOOL_P_H

#ifndef N_POOL_ARENAS
#define N_POOL_ARENAS 32
#endif


struct slsPool {

  int refcount;

  slsPool *parent;
  slsPool *child_top;


  char *arenas[N_POOL_ARENAS];
  size_t arena_alloc_size[N_POOL_ARENAS];
  size_t mem_used[N_POOL_ARENAS];

  SLS_INTRUSIVE_FIELDS(slsPool, siblings);
};

#endif //DANGERENGINE_SLSPOOL_P_H

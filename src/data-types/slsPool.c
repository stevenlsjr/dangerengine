/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/17/16, Steven
 * 
 **/
#include "slsPool.h"
#include "linkedlist.h"
#include <stdlib.h>

struct slsPool {

  int refcount;

  slsPool *parent;
  slsLinkedList *children;


};

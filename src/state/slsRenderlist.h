/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/23/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSRENDERLIST_H
#define DANGERENGINE_SLSRENDERLIST_H

#include <slsutils.h>
#include <data-types/intrusivelist.h>

typedef struct slsRenderNode slsRenderNode;
typedef struct slsRenderNode_p slsRenderNode_p;


struct slsRenderNode {
  SLS_INTRUSIVE_FIELDS(slsRenderNode, iter);

  slsRenderNode_p *pimpl;
};

#endif //DANGERENGINE_SLSRENDERLIST_H

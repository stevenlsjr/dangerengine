//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_SLSGRAPH_H
#define DANGERENGINE_SLSGRAPH_H

#include "../renderer/slsmodel.h"
#include <kazmath/kazmath.h>

typedef struct slsGraphNode slsGraphNode;

struct slsGraphNode {
  slsModel *model;
  kmMat4 transform;

  slsGraphNode *(*init)(slsGraphNode *self, 
                        slsModel *model, 
                        kmMat4 const *transform);
  void (*dtor)(slsGraphNode *self);
  void (*add_child)(slsGraphNode *parent, slsGraphNode *child);)

  slsGraphNode *parent;
  slsGraphNode **children;
  size_t n_children;
};

void sls_graph_add_child(slsGraphNode *parent, slsGraphNode *child);

#endif //DANGERENGINE_SLSGRAPH_H

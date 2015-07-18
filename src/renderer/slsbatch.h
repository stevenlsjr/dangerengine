//
// Created by Steven on 7/17/15.
//

#ifndef DANGERENGINE_SLSBATCH_H
#define DANGERENGINE_SLSBATCH_H

#include "slsmodel.h"

#include "../data-types/linkedlist.h"

typedef struct slsBatch slsBatch;
typedef struct slsBatchArray slsBatchArray;

typedef enum {
  SLS_BATCH_BATCHES,
  SLS_BATCH_MODELS
} slsBatchType;

/**
 * @brief a n-degree tree for
 * sorting mesh objects by common opengl
 * state
 */
struct slsBatch {
  size_t n_elements;
  slsBatchType node_type;
  slsLinkedList *nodes;
};

slsBatch *sls_batch_new(slsModel **models, size_t n_models);

void sls_batch_dtor(slsBatch *self);


#endif //DANGERENGINE_SLSBATCH_H


//
// Created by Steven on 7/17/15.
//

#ifndef DANGERENGINE_LINKEDLIST_H
#define DANGERENGINE_LINKEDLIST_H

#include <stddef.h>
#include "callbacks.h"

typedef struct slsLinkedList slsLinkedList;
typedef struct slsListNode slsListNode;

struct slsLinkedList {
  slsListNode *head;
  slsCallbackTable callbacks;
};

struct slsListNode {
  slsListNode *prev;
  slsListNode *next;

  void *data;

  slsCallbackTable *callbacks;
};

slsLinkedList *sls_linked_list_new(slsCallbackTable const *callbacks);

slsLinkedList *sls_linked_list_init(slsLinkedList *self,
                                    slsCallbackTable const *callbacks);


/**
 * @brief List destructor, destroying all nodes contained
 *
 * @return Deinitialized list object. If allocated on the heap, it can be freed
 */
slsLinkedList *sls_linked_list_dtor(slsLinkedList *self);

slsListNode *sls_list_node_new(void *data,
                               slsListNode *prev,
                               slsListNode *next,
                               slsCallbackTable *callbacks);


void sls_list_node_dtor(slsListNode *self);

void sls_list_node_insert_ahead(slsListNode *self, slsListNode *new_node);

slsListNode *sls_list_node_remove_ahead(slsListNode *self);



/**
 * @brief Macro for constructing a linked list for-loop
 * @param start An initialized list node to begin iteration
 * @param itor A variable with type (slsListNode *). It will be used in iteration
 */
#define SLS_LINKEDLIST_FOREACH(start, itor)  \
  for ((itor) = (start); (itor) != NULL; (itor) = (itor)->next)

#endif //DANGERENGINE_LINKEDLIST_H


//
// Created by Steven on 7/17/15.
//
/**
 * @file linkedlist.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea. 
**/

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


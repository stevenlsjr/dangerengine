//
// Created by Steven on 7/17/15.
//

/**
 * @file linkedlist.c
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#include "linkedlist.h"
#include "slsutils.h"
#include <assert.h>

slsLinkedList* sls_linked_list_init(slsLinkedList* self,
                                    slsCallbackTable const* callbacks)
{
  if (!self) {
    return NULL;
  }

  self->callbacks = callbacks ? *callbacks : (slsCallbackTable){};

  return self;
}

slsLinkedList* sls_linked_list_new(slsCallbackTable const* callbacks)
{
  slsLinkedList* list = NULL;
  list = calloc(sizeof(slsLinkedList), 1);
  sls_checkmem(list);

  return sls_linked_list_init(list, callbacks);

error:
  if (list) {
    sls_linked_list_dtor(list);
  }
  return NULL;
}

slsLinkedList* sls_linked_list_dtor(slsLinkedList* self)
{
  if (!self) {
    return NULL;
  }

  if (self->head) {
    slsListNode* head = self->head;
    slsListNode* last = NULL;
    while (!head->next) {
      slsListNode* next = sls_list_node_remove_ahead(head);
      if (last == next) {
        sls_log_err("linked list %p failed to remove its nodes\n"
                    "expect memory error",
                    self);
        break;
      }
    }
    head->next = NULL;
    sls_list_node_dtor(head);
    self->head = NULL;
  }

  return self;
}

slsListNode* sls_list_node_new(void* data,
                               slsListNode* prev,
                               slsListNode* next,
                               slsCallbackTable* callbacks)
{
  slsListNode* node = NULL;
  node = calloc(sizeof(slsListNode), 1);
  sls_checkmem(node);

  *node = (slsListNode){
    .data = data, .prev = prev, .next = next, .callbacks = callbacks
  };

  return node;

error:
  if (node) {
    sls_list_node_dtor(node);
  }
  return NULL;
}

void sls_list_node_dtor(slsListNode* self)
{
  if (!self) {
    return;
  }

  if (self->prev || self->next) {
    sls_log_warn(
      "destroyed list node %p is still linked to other nodes (%p %p)",
      self,
      self->prev,
      self->next);
    if (self->prev) {
      self->prev->next = NULL;
    }
    if (self->next) {
      self->next->prev = NULL;
    }
  }

  if (self->callbacks && self->callbacks->free_fn) {
    self->callbacks->free_fn(self->data);
  }

  free(self);
}

void sls_list_node_insert_ahead(slsListNode* self, slsListNode* new_node)
{
  if (!self || !new_node) {
    return;
  }
  slsListNode* next = self->next;
  new_node->next = next;
  self->next = new_node;
  new_node->prev = self;

  if (next) {
    next->prev = new_node;
  }
}

slsListNode* sls_list_node_remove_ahead(slsListNode* self)
{
  assert(self);
  slsListNode* removed = self->next;
  if (removed) {
    slsListNode* new_next = removed->next;
    removed->prev = NULL;
    removed->next = NULL;
    self->next = new_next;
    if (new_next) {
      new_next->prev = self;
    }
  }

  return removed;
}

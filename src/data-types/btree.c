/*
btree.c
Created by Steven Shea on 3/14/15.
for CMPS 1600, project 2
*/

#include "btree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



slsBTree *sls_btree_new(slsCopyFn copy_fn, slsFreeFn free_fn) {
  slsBTree const proto = {
    .copy_fn=copy_fn,
    .free_fn=free_fn,
    .head=NULL
  };
  slsBTree *tree = NULL;
  tree = sls_objalloc(&proto, sizeof(slsBTree));
  sls_checkmem(tree);

  return tree;
error:
  return NULL;
}

void sls_btree_destroy(slsBTree *tree) {
  if (tree) {
    if (tree->head) {
      sls_bnode_destroy(tree->head);
    }

    free(tree);
  } else {
    fprintf(stderr, "%s tree is NULL!\n", __func__);
  }
}

slsBNode *sls_bnode_new(slsBTree *tree, void const *val, slsBNode *left,
                        slsBNode *right) {
  slsBNode *node = NULL;
  if (!tree) {
    fprintf(stderr, "ERROR %s: tree must be nonnull!\n", __func__);
    return NULL;
  }
  node = calloc(sizeof(slsBNode), 1);
  if (node) {
    node->tree = tree;
    node->val = tree->copy_fn(val);
    if (left) {
      node->left = left;
      left->parent = node;
    }
    if (right) {
      node->right = right;
      right->parent = node;
    }
  } else {
    fprintf(stderr, "ERROR %s: malloc error!\n", __func__);
  }
  return node;
}

void sls_bnode_destroy(slsBNode *node) {
  if (node) {

    assert(node != node->parent);

    assert((!node->parent) || node->val != node->parent->val);

    /**
     * recursively destroy child nodes
     */
    if (node->left) {
      sls_bnode_destroy(node->left);
      node->left = NULL;
    }
    if (node->right) {
      sls_bnode_destroy(node->right);
      node->right = NULL;
    }

    /* try freeing node data */
    if (node->tree) {
      node->tree->free_fn(node->val);
      node->val = NULL;
    } else {
      fprintf(stderr, "WARNING %s: node->tree is null. Cannot free node data\n",
              __func__);
    }

    free(node);
  } else {
    fprintf(stderr, "WARNING %s: node is null\n", __func__);
  }
}

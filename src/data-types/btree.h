/**
Created by Steven Shea on 3/14/15.
for CMPS 1600, project 2

@file
@brief definitions for binary tree data type
*/

#ifndef _ANIMAL_BTREE_H_
#define _ANIMAL_BTREE_H_

#include <stdlib.h>
#include "../slsutils.h"
#include <sys/rbtree.h>

typedef struct slsBNode slsBNode;
typedef struct slsBTree slsBTree;

#include "callbacks.h"

struct StrNode {
  RB_ENTRY(StrNode) _entry;
};

/**
 * @brief enumeration used for determining
 * which child node to select. Usage TBD
 */
typedef enum slsChildSelector {
  SLS_CHILD_LEFT,
  SLS_CHILD_RIGHT
} slsChildSelector;


/**
 * @brief Color for red/black tree nodes
 */
typedef enum slsNodeColor {
  SLS_NODE_RED,
  SLS_NODE_BLACK
} slsNodeColor;

/**
 * @brief binary tree abstract data type
 * @details provides a wrapper type for
 * a tree made up of allocated nodes.
 * The structure also provides a crude
 * virtual table for the nodes, providing
 * callbacks necessary for their logic
 */
struct slsBTree {
  /**
   * callback for copying values
   * in nodes
   */
  slsCopyFn copy_fn;
  /**
   * callback for freeing values in nodes
   */
  slsFreeFn free_fn;

  /**
  * callback for copying values
  * in nodes
  */
  slsCopyFn key_copy_fn;
  /**
   * callback for freeing values in nodes
   */
  slsFreeFn key_free_fn;

  slsCmpFn key_cmp_fn;

  slsBNode *head;
};

/**
 * @brief binary tree node for use in slsBTree type
 * @details stores a pointer to arbitrary data, whose
 * memory is 'owned' by the binary tree. If the tree is
 * constructed correctly, all memory will be freed when
 * tree is destroyed
 */
struct slsBNode {
  /**
   * @brief pointer to slsBTree which contains the node (
   * tree holds function pointers for determining
   * node's behavior)
   */
  slsBTree *tree;

  /**
   * @brief parent node
   */
  slsBNode *parent;

  /**
   * @brief left node
   */
  slsBNode *left;

  /**
   * @brief right node
   */
  slsBNode *right;

  /**
   * @brief data stored in node
   */
  void *key;
  void *val;

  slsNodeColor color;
};

/**
 * @brief creates a new binary tree
 * @details [long description]
 *
 * @param compare_fn callback for comparing values in two nodes
 * uses same function signature as qsort
 * @param copy_fn callback for copying values
 * @param free_fn callback for freeing node values
 * @return [description]
 */
slsBTree *sls_btree_new(slsCopyFn key_copy_fn,
                        slsFreeFn key_free_fn,
                        slsCmpFn key_cmp_fn,
                        slsCopyFn val_copy_fn,
                        slsFreeFn val_free_fn);

/**
 * @brief destroys a binary tree and
 * all nodes it contains
 * @details [long description]
 *
 * @param tree [description]
 */

void sls_btree_destroy(slsBTree *tree);

/**
 * @brief creates new binary tree node
 * @details [long description]
 *
 * @param tree pointer to parent tree.
 * Must not be null
 * @param val pointer to value data
 * @param left (nullable) pointer to left node
 * @param right (nullable) pointer to right node
 * @return [description]
 */
slsBNode *sls_bnode_new(slsBTree *tree,
                        void const *val,
                        slsBNode *left,
                        slsBNode *right);

/**
 * @brief recursively destroys node and all child nodes
 * @details if node's btree contains a free function pointer,
 * it will call the function on each node's data field
 *
 * @param node [description]
 */



void sls_bnode_destroy(slsBNode *node);


#endif /*_ANIMAL_BTREE_H_*/

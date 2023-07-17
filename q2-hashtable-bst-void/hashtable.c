/////////////////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments > Assignment Information & Policies > Academic Integrity Policy)
/////////////////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Dami Peng
// login ID: dkpeng
//////////////////////////////////////////////////////////////////////////////////////////

// This is the implementation of the generic hash table ADT with binary search 
//   trees.

#include <stdlib.h>
#include "hashtable.h"
#include "cs136-trace.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See hashtable.h for documentation
const int HT_SUCCESS        = 0;
const int HT_ALREADY_STORED = 1;
const int HT_NOT_STORED     = 2;
// -----------------------------------------------------------------------

// bst node struct
struct bstnode {
  void *key;
  struct bstnode *left;
  struct bstnode *right;
};

// bst struct
struct bst {
  struct bstnode *root;
};

// See hashtable.h for documentation
struct hashtable {
  // hash length and size/capacity
  int len;
  int size;
  // hash function
  int (*hash_func)(const void *, int);
  // clone function
  void *(*key_clone)(const void *);
  // compare function
  int (*key_compare)(const void *, const void *);
  // key destroy
  void (*key_destroy)(void *);
  // print function
  void (*key_print)(const void *);
  // array of stored bst
  struct bst **data;
};

// bst_create() creates a new BST
// effects: allocates memory: call bst_destroy
struct bst *bst_create(void) {
  struct bst *t = malloc(sizeof(struct bst));
  t->root = NULL;
  return t;
}

// See hashtable.h for documentation
struct hashtable *ht_create(void *(*key_clone)(const void *),
                            int (*hash_func)(const void *, int), 
                            int hash_length,
                            int (*key_compare)(const void *, const void *),
                            void (*key_destroy)(void *),
                            void (*key_print)(const void *)) {
  // assert functions
  assert(key_clone);
  assert(hash_func);
  assert(key_compare);
  assert(key_destroy);
  assert(key_print);

  struct hashtable *temp = malloc(sizeof(struct hashtable));
  // finding length of hashtable
  int size = 1;
  // 2^hash_length
  for (int i = 0;i<hash_length;i++) {
    size *=2;
  }
  // set size
  temp->size = size;
  temp->len = hash_length;
  // setting function
  temp->key_clone = key_clone;
  temp->hash_func = hash_func;
  temp->key_compare = key_compare;
  temp->key_print = key_print;
  // allocating memory for data
  temp->data = malloc(size * sizeof(struct bst));
  // set to null?
  for (int i =0 ; i<size;i++) {
    temp->data[i] = bst_create(); 
  }
  return temp;
}

// free_bstnode(node, key_destroy) frees all resources allocated in node
// effects: invalidates node
// time O(n), where n is the len of tree
static void free_bstnode(struct bstnode *node) {
  if (node) {
    free_bstnode(node->left);
    free_bstnode(node->right);
    free(node->key);
    free(node);
  }
}

void bst_destroy(struct bst *data) {
  assert(data);
  free_bstnode(data->root);
  free(data);
}

// See hashtable.h for documentation
void ht_destroy(struct hashtable *ht) {
  assert(ht);
  for (int i = 0; i < ht->size; i++) {
    bst_destroy(ht->data[i]);
  }
  // free everything else
  free(ht->data);
  free(ht);
}

static struct bstnode *new_leaf(const void *key, void *(*key_clone)(const void *)) {
  assert(key);
  struct bstnode *leaf = malloc(sizeof(struct bstnode));
  void *temp = key_clone(key);
  leaf->key = temp;
  leaf->left = NULL;
  leaf->right = NULL;
  return leaf;
}


// See hashtable.h for documentation
int ht_insert(struct hashtable *ht, const void *key) {
  // insert posn
  int insert_posn = ht->hash_func(key, ht->len);
  // insert key
  struct bstnode *node = ht->data[insert_posn]->root;
  struct bstnode *parent = NULL;
  // while node is not null and key does not equal node
  while (node && ht->key_compare(key,node->key) !=0 ) {
    parent = node;
    // search left
    if (ht->key_compare(key,node->key) < 0) {
      node = node->left;
    } else {
    // search right
      node = node->right;
    }
  }
  // key already exists at node
  if (node != NULL) { 
    return HT_ALREADY_STORED;
    // empty tree
  } else if (parent == NULL) { 
    ht->data[insert_posn]->root = new_leaf(key,ht->key_clone);
  // insert left
  } else if (ht->key_compare(key,parent->key) < 0) {
    parent->left = new_leaf(key,ht->key_clone);
  // insert right
  } else {
    parent->right = new_leaf(key,ht->key_clone);
  }
  return HT_SUCCESS;
}

// See hashtable.h for documentation
int ht_remove(struct hashtable *ht, const void *key) {
  assert(ht);
   // insert posn
  int rm_posn = ht->hash_func(key, ht->len);
  // set target
  struct bstnode *target = ht->data[rm_posn]->root;
  struct bstnode *target_parent = NULL;

  // find the target (and its parent)
  while (target && ht->key_compare(key,target->key) != 0) {
    target_parent = target;
    // if target is to left
    if (ht->key_compare(key,target->key) < 0) {
      target = target->left;
    // if target is to right
    } else {
      target = target->right;
    }
  }
  // key not found
  if (target == NULL) {
    return HT_NOT_STORED; 
  }
  // find the node to "replace" the target
  struct bstnode *replacement = NULL;
  if (target->left == NULL) {
    replacement = target->right;
  } else if (target->right == NULL) {
    replacement = target->left;
  } else {
    // neither child is NULL:
    // find the replacement node and its parent
    replacement = target->right;
    struct bstnode *replacement_parent = target;
    while (replacement->left) {
      replacement_parent = replacement;
      replacement = replacement->left;
    }
    // update the child links for the replacement and its parent
    replacement->left = target->left;
    if (ht->key_compare(replacement_parent->key,target->key) != 0) {
      replacement_parent->left = replacement->right;
      replacement->right = target->right;
    }
  }
  // free the target, and update the target's parent
  free(target->key);
  free(target);
  if (target_parent == NULL) {
    ht->data[rm_posn]->root = replacement;
  // replace with right
  } else if (ht->key_compare(key, target_parent->key) > 0) {
    target_parent->right = replacement;
  // replace with left
  } else {
    target_parent->left = replacement;
  }

  return HT_SUCCESS;
}

// bstnode_print(node, first) Print the current node. Proceed by a comma
//   if it is not *first, otherwise makes other nodes not *first.
// requires : node and first are not NULL
// effects : prints output, may mutate *first
// time : O(1)
static void bstnode_print(struct bstnode *node, bool *first, int depth, void (*key_print)(const void *)) {
  assert(node);
  assert(first);
  if (*first) {
    *first = false;
  } else {
    printf(",");
  }
  printf("%d-",depth);
  key_print(node->key);
}

// bstnodes_print(node, first) Print the sub-tree rooted at node in
//   in order from smallest to largest. Procced by a comma if not *first, 
//   otherwise updates *first.
// requires : first is not NULL
// effects : prints output, may mutate first
// time : O(n)
static void bstnodes_print(struct bstnode *node, bool *first, int depth, void (*key_print)(const void *)) { 
  assert(first);
  if (node) {
    bstnodes_print(node->left, first, depth+1, key_print);
    bstnode_print(node, first, depth, key_print);
    bstnodes_print(node->right, first, depth+1, key_print);
  }
}


// See hashtable.h for documentation
void ht_print(const struct hashtable *ht) {
  // Your implementation goes here.
  // See string_public.expect and fancyline_public.expect for examples.
  for(int i = 0 ; i < ht->size; i++) {
    bool first = true;
    printf("%d: [",i);
    bstnodes_print(ht->data[i]->root, &first, 0, ht->key_print);
    printf("]\n"); 
  }
}
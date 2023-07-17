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

// This is the implementation of the hash tree ADT with linked lists.

#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "cs136-trace.h"
#include <string.h>
#include <assert.h>

// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See hashtable.h for documentation
const int HT_SUCCESS        = 0;
const int HT_ALREADY_STORED = 1;
const int HT_NOT_STORED     = 2;
// -----------------------------------------------------------------------

// See hashtable.h for documentation
struct hashtable {
  // hash length and size/capacity
  int len;
  int size;
  // hash function
  int (*hash_func)(const char *, int);
  struct llist **data;
};

// linked list node struct
struct llnode {
  char *item;
  struct llnode *next;
};

// linked list struct
struct llist {
  struct llnode *front;
};

// list_create() creates a new, empty list
// effects: allocates memory: call list_destroy
struct llist *list_create(void) {
  struct llist *lst = malloc(sizeof(struct llist));
  lst->front = NULL;
  return lst;
}

// See hashtable.h for documentation
struct hashtable *ht_create(int (*hash_func)(const char *, int),
                            int hash_length) {
  // allocate memory to dummy hashtable structure 
  struct hashtable *temp = malloc(sizeof(struct hashtable));
  // finding length of hashtable
  int size = 1;
  // 2^hash_length
  for (int i = 0;i<hash_length;i++) {
    size *=2;
  }
  // set size
  temp->size = size;
  // set capacity
  temp->len = hash_length;
  // set hashfunction into hashtable
  temp->hash_func = hash_func;
  // allocate the array of llist
  temp->data = malloc(sizeof(struct llist) * temp->size);
  // set all llist to null
  for (int i =0;i < size; i++) {
    temp->data[i] = list_create();
  }
  // return
  return temp;
}

// list_destroy() destroys list
// effects: frees memory: call list_destroy
void list_destroy(struct llist *lst) {
  assert(lst);
  // set current node as front node
  struct llnode *curnode = lst->front;
  // set next node as null
  struct llnode *nextnode = NULL;
  
  // while the list still has nodes connecting to it
  while (curnode) {
    // set next node 
    nextnode = curnode->next;
    // free front node
    free(curnode->item);
    free(curnode);
    // move on to next node
    curnode = nextnode;
  }
  // free lst
  free(lst);
}

// See hashtable.h for documentation
void ht_destroy(struct hashtable *ht) {
  assert(ht);
  // free every llist in data
  for (int i = 0;i<ht->size;i++) {
    list_destroy(ht->data[i]);
  }
  // free hashtable
  free(ht->data);
  free(ht);
}

// add_front(i, lst) adds i to the front of lst
// effects: modifies lst
void add_front(const char *str, struct llist *lst) {
  struct llnode *newnode = malloc(sizeof(struct llnode));
  // length of string
  int len_str = strlen(str);
  // create the string and put into pos
  char *temp = malloc(len_str+1);
  strcpy(temp,str);
  newnode->item = temp;
  newnode->next = lst->front;
  lst->front = newnode;
}

// See hashtable.h for documentation
int ht_insert(struct hashtable *ht, const char *str) {
  // insertion position
  int insert_posn = ht->hash_func(str,ht->len);
  // set current node
  struct llnode *curnode = ht->data[insert_posn]->front;
  // if no colision
  if (!curnode) {
    add_front(str,ht->data[insert_posn]);
    return HT_SUCCESS;
  }
  // else check if string already exists in llist
  while(curnode) {
    if (strcmp(curnode->item,str) == 0) {
      return HT_ALREADY_STORED;
    }
    curnode = curnode->next;
  }
  // if node does not exist in node then add front
  add_front(str,ht->data[insert_posn]);
  return HT_SUCCESS;

}

// See hashtable.h for documentation
int ht_remove(struct hashtable *ht, const char *str) {
  // remove psn
  int remove_posn = ht->hash_func(str,ht->len);

  // if there is no string in hashtable key
  if (!ht->data[remove_posn]->front) {
    return HT_NOT_STORED;
  }

  // if front of list is str then remove and return
  if (strcmp(ht->data[remove_posn]->front->item,str) == 0) {
    struct llnode *old_front = ht->data[remove_posn]->front;
    ht->data[remove_posn]->front = ht->data[remove_posn]->front->next;
    free(old_front->item);
    free(old_front);
    return HT_SUCCESS;
  }

  // loop through
  struct llnode *before = ht->data[remove_posn]->front;
  while (before->next && strcmp(before->next->item,str)!= 0) {
    before = before->next;
  }
  // if next is null then return
  if (before->next == NULL){
    return HT_NOT_STORED;
  }
  // if in then remove the old node and add new pointer
  struct llnode *old_node = before->next;
  before->next = before->next->next;
  free(old_node->item);
  free(old_node);
  return HT_SUCCESS;
}

// list_print(lst) prints list nodes
void list_print(const struct llist *lst) {
  assert(lst);
  struct llnode *curnode = lst->front;
  if (curnode == NULL) {
    printf("\n");
    return;
  }
  while (curnode) {
    printf("%s", curnode->item);
    curnode = curnode->next;
    if (curnode) {
      printf(" -> ");
    }
  }
  printf("\n");
}

// See hashtable.h for documentation
void ht_print(const struct hashtable *ht) {
  // Your implementation goes here.
  // See public.expect for an example.
  for (int i =0;i<ht->size;i++){
    printf("%d: ",i);
    list_print(ht->data[i]);
  }
}
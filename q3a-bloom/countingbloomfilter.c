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

// This is the implementation of the counting Bloom filter ADT.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "countingbloomfilter.h"

// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See countingbloomfilter.h for documentation
const int CBF_SUCCESS = 0;
const int CBF_FAILURE = 1;
// -----------------------------------------------------------------------

// hashtable struct
struct hashtable {
  // hash length and size/capacity
  int len;
  int size;
  // hash function
  int (*hash_func)(const char *, int);
  int *val;
};


// See countingbloomfilter.h for documentation
struct countingbloomfilter {
  // hash_len & size of the has functions
  int len;
  int size;
  int nof_tables;
  struct hashtable **ht_arr;
};

// ht_create(hash_func, hash_length) creates a new string hash table with
//   hash_func as hash function and hash_length the length of the result
//   of hash_func (in bits).
// effects: allocates heap memory; client must call ht_destroy
// requires: hash_length must be positive
// time: O(n), where n is the length of the hash table
struct hashtable *ht_create(int (*hash_func)(const char *, int),
                            int hash_length, int size) {
  // allocate memory to dummy hashtable structure 
  struct hashtable *temp = malloc(sizeof(struct hashtable));
  // allocate the array of llist
  temp->len = hash_length;
  temp->size = size;
  temp->hash_func = hash_func;
  // set array of counters 
  temp->val = malloc(sizeof(int) * size);
  for (int i =0 ; i < size; i++) {
     temp->val[i] = 0;
  }
  // return
  return temp;
}

// See countingbloomfilter.h for documentation
struct countingbloomfilter *cbf_create(int (**hash_funcs)(const char *, int),
                                       int hash_len, int nof_tables) { 
  // allocate memory to dummy hashtable structure 
  struct countingbloomfilter *temp = malloc(sizeof(struct countingbloomfilter));
  // finding length of hashtable
  int size = 1;
  // 2^hash_length
  for (int i = 0;i<hash_len;i++) {
    size *=2;
  }
  // sets nof_tables
  temp->nof_tables = nof_tables;
  // sets len & size
  temp->len = hash_len;
  temp->size = size;
  // malloc hashtable arrays
  temp->ht_arr = malloc(nof_tables * sizeof(struct hashtable));
  // create hashtables
  for (int i = 0; i < nof_tables; i++) {
    // create ht
    temp->ht_arr[i] = ht_create(hash_funcs[i], hash_len, size);
  }
  return temp;
}

// ht_destroy(ht) frees all resources allocated by the hashtable ht.
// effects: invalidates ht
// time: O(1)
void ht_destroy(struct hashtable *ht) {
  assert(ht);
  // free everything else
  free(ht->val);
  free(ht);
}

// See countingbloomfilter.h for documentation
void cbf_destroy(struct countingbloomfilter *cbf) {
  assert(cbf);
  // free hashtables
  for (int i = 0; i < cbf->nof_tables; i++) {
     ht_destroy(cbf->ht_arr[i]);
  }
  free(cbf->ht_arr);
  free(cbf);
}

// See countingbloomfilter.h for documentation
void cbf_insert(struct countingbloomfilter *cbf, const char *key) {
  assert(cbf);
  assert(key);
  // changing values according to hash value determined by key
  for (int i = 0; i < cbf->nof_tables; i++) {
    // get temp hash_val
    int hash_val = cbf->ht_arr[i]->hash_func(key,cbf->len);
    // set counter + 1
    cbf->ht_arr[i]->val[hash_val] += 1;
  }

}

// See countingbloomfilter.h for documentation
bool cbf_lookup(const struct countingbloomfilter *cbf, const char *key) {
  assert(cbf);
  assert(key);
  // loop through all hash tables
  for (int i = 0 ; i< cbf->nof_tables; i++) {
    // get temp has_val
    int hash_val = cbf->ht_arr[i]->hash_func(key,cbf->len);
    // check if each hash_table has a counter stored in index
    if (cbf->ht_arr[i]->val[hash_val] == 0 ) {
      return false;
    }
  }
  return true;
}

// See countingbloomfilter.h for documentation
int cbf_remove(struct countingbloomfilter *cbf, const char *key) {
  // if key cannot be removed
  if (!cbf_lookup(cbf,key)) {
     return CBF_FAILURE;
  }
  // if key can be removed
  for (int i = 0 ; i < cbf->nof_tables; i++) {
    int hash_val = cbf->ht_arr[i]->hash_func(key,cbf->len);
    // remove counter
    cbf->ht_arr[i]->val[hash_val] -=1;
  }
  return CBF_SUCCESS;
}

// See countingbloomfilter.h for documentation
void cbf_print(const struct countingbloomfilter *cbf) {
  // Your implementation goes here.
  // Use printf("%3d\n", foo); for printing counter foo.
  for (int i = 0 ; i < cbf->nof_tables; i++) {
    printf("table %d:\n", i);
    for (int j = 0; j < cbf->size; j++) {
      printf("  %d\n", cbf->ht_arr[i]->val[j]);
    }
  }
}
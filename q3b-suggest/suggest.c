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

// This is the implementation of the suggest module.

#include "suggest.h"
#include "triple-s.h"
#include "countingbloomfilter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////// 
// do not modify this constant:
const int SUGGEST_MAX_STRLEN = 63;
/////////////////////////////////////////////////////////////////////////////

// time: O(n^2) 
struct triple_s *suggest(const struct countingbloomfilter *dict, 
                         const char *word) {
  assert(dict);
  // create triple s adt
  struct triple_s *sss = sss_create();
  // create a dup of word
  // set str length
  int len_str = strlen(word);

  // single deletions
  for (int i = 0 ; i < len_str; i++ ) {
    char *dup = malloc(len_str);
    for (int j = 0 ; j < i; j++) {
      dup[j] = word[j];
    }
    // skip i 
    for (int j = i; j < len_str-1; j++) {
      dup[j] = word[j+1];
    }
    dup[len_str-1] = '\0';
    // if dup is in dict
    if (cbf_lookup(dict,dup)) {
        sss_add(sss,dup);
    }
    free(dup);
  }
  // single insertions
  for (int i = 0; i <= len_str; i++) {
    for (int k = 'a' ; k <= 'z'; k++) {
      // len of str + 1 + null terminator
      char *dup = malloc(len_str + 2);
      for (int j = 0 ; j < i; j++) {
        dup[j] = word[j];
      }
      dup[i] = k;
      for (int j = i; j < len_str; j++) {
        dup[j+1] = word[j];
      }
      dup[len_str + 1] = '\0';
      if (cbf_lookup(dict,dup)) {
        sss_add(sss,dup);
      }
      free(dup);
    }
  }

  // single subsitutions
  for (int i = 0 ; i < len_str; i++) { 
    // for j = a j < z
    for (int j = 'a' ; j <= 'z'; j++) {
      char *dup = malloc(len_str+1);
      strcpy(dup,word);
      // turn char at index i into next letter | a -> b
      dup[i] = j;
      // if modified word exists in dictionary then add
      if (cbf_lookup(dict,dup)) {
        sss_add(sss,dup);
      }
      free(dup);
    }
  }

  // adjacent swaps
  for (int i = 0 ; i < len_str -1 ; i++) {
    char *dup = malloc(len_str+1);
    strcpy(dup,word);
    // swap chars at a[i] and a[i+1]
    char temp = dup[i];
    dup[i] = dup[i+1];
    dup[i+1] = temp;
    // if modified word exists in dict. 
    if (cbf_lookup(dict,dup)) {
      sss_add(sss,dup);
    }
    free(dup);
  } 

  return sss;
}

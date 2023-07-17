// This is an I/O test client for the suggest module

#include <assert.h>
#include <stdio.h>
#include "words.h"
#include "suggest.h"
#include "hash.h"
#include "triple-s.h"

int main(void) {

  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  int nof_tables = -1;
  assert(scanf("%d", &nof_tables) == 1);
  assert(nof_tables <= 5);
  assert(2 <= hash_length && hash_length <= 16);
  
  int (*hash_funcs[5])(const char *, int) = { hash_str_0, hash_str_1, 
    hash_str_2, hash_str_3, hash_str_4 };

  struct countingbloomfilter *dict = words_create(hash_funcs, hash_length,
    nof_tables);
  char word[64] = "";
  while (scanf("%s", word) == 1) {
    printf("%s:", word);
    bool found = cbf_lookup(dict, word);
    if (!found) {
      struct triple_s *suggestions = suggest(dict, word);
      if (sss_count(suggestions)) {
        for (int i = 0; i < sss_count(suggestions); ++i) {
          printf(" %s", sss_get(suggestions, i));
        }
      } else {
        printf(" NONE");
      }
      sss_destroy(suggestions);
    } else {
      printf(" OK");
    }
    printf("\n");
  }
  cbf_destroy(dict);
}

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cs136-trace.h"
#include "read.h"
#include "hash.h"
#include "countingbloomfilter.h"

int main(void) {

  const int CREATE = lookup_symbol("COUNTINGBLOOMFILTER");
  const int INSERT = lookup_symbol("INSERT");
  const int LOOKUP = lookup_symbol("LOOKUP");
  const int REMOVE = lookup_symbol("REMOVE");
  const int PRINT = lookup_symbol("PRINT");

  int cmd = read_symbol();
  assert(cmd == CREATE);
  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  int nof_tables = -1;
  assert(scanf("%d", &nof_tables) == 1);
  assert(nof_tables <= 5);
  assert(2 <= hash_length && hash_length <= 16);

  int (*hash_funcs[5])(const char *, int) = { hash_str_0, hash_str_1, 
    hash_str_2, hash_str_3, hash_str_4 };

  struct countingbloomfilter *cbf = cbf_create(hash_funcs, hash_length, 
    nof_tables);

  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == INSERT) {
      char *input = read_line();
      cbf_insert(cbf, input);
      printf("key \"%s\" inserted into the counting Bloom filter.\n", input);
      free(input);
    } else if (cmd == LOOKUP) {
      char *input = read_line();
      bool status = cbf_lookup(cbf, input);
      if (status) {
        printf("key \"%s\" is in the counting Bloom filter.\n", input);
      }
      else {
        printf("key \"%s\" is not in the counting Bloom filter.\n", input);
      }
      free(input);
    } else if (cmd == REMOVE) {
      char *input = read_line();
      int status = cbf_remove(cbf, input);
      if (status == CBF_SUCCESS) {
        printf("SUCCESS: key \"%s\" removed from the counting Bloom filter.\n",
          input);
      } else {
        printf(
          "WARNING: key \"%s\" not removed from the counting Bloom filter.\n",
          input);
      }
      free(input);
    } else if (cmd == PRINT) {
      cbf_print(cbf);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }

  cbf_destroy(cbf);
}
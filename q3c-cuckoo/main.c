#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "cs136-trace.h"
#include "hash.h"
#include "read.h"
#include "cuckoofilter.h"

int main(void) {

  const int CREATE = lookup_symbol("CUCKOOFILTER");
  const int INSERT = lookup_symbol("INSERT");
  const int LOOKUP = lookup_symbol("LOOKUP");
  const int REMOVE = lookup_symbol("REMOVE");
  const int PRINT = lookup_symbol("PRINT");

  int cmd = read_symbol();
  assert(cmd == CREATE);
  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  int nof_entries = -1;
  assert(scanf("%d", &nof_entries) == 1);
  int fp_length = -1;
  assert(scanf("%d", &fp_length) == 1);
  int max_num_kicks = -1;
  assert(scanf("%d", &max_num_kicks) == 1);
  assert(2 <= hash_length && hash_length <= 16);

  struct cuckoofilter *cf = cuckoo_create(hash_str, hash_length, nof_entries,
    fp_length, max_num_kicks);

  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == INSERT) {
      char *input = read_line();
      int status = cuckoo_insert(cf, input);
      if (status == CUCKOO_SUCCESS) {
        printf("SUCCESS: key \"%s\" inserted into the cuckoo filter.\n", input);
        free(input);
      } else {
        printf("ERROR: key \"%s\" not inserted into the cuckoo filter.\n", 
          input);
        free(input);
        break;
      }
    } else if (cmd == LOOKUP) {
      char *input = read_line();
      bool status = cuckoo_lookup(cf, input);
      if (status) {
        printf("key \"%s\" is in the cuckoo filter.\n", input);
      }
      else {
        printf("key \"%s\" is not in the cuckoo filter.\n", input);
      }
      free(input);
    } else if (cmd == REMOVE) {
      char *input = read_line();
      int status = cuckoo_remove(cf, input);
      if (status == CUCKOO_SUCCESS) {
        printf("SUCCESS: key \"%s\" removed from the cuckoo filter.\n", input);
      } else {
        printf("WARNING: key \"%s\" not removed from the cuckoo filter.\n",
               input);
      }
      free(input);
    } else if (cmd == PRINT) {
      cuckoo_print(cf);
    } else {
      printf("ERROR: unknown command\n");
      break;
    }
  }

  cuckoo_destroy(cf);
}
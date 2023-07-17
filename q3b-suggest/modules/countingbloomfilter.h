#include <stdbool.h>

// CBF_SUCCESS indicates successful execution of the cbf_remove function.
extern const int CBF_SUCCESS;
// CBF_FAILURE indicates failed execution of the cbf_remove function.
extern const int CBF_FAILURE;

// a counting Bloom filter for storing strings
struct countingbloomfilter;

// requires: all functions require valid (non-NULL) parameters

// cbf_create(hash_funcs, hash_len, nof_tables) creates a new Bloom filter for
//   strings with hash_funcs as the hash functions, hash_len the length of
//   the result of a hash function (in bits), and nof_tables the number of
//   used hash tables.
// effects:  allocates heap memory; client must call cbf_destroy
// requires: hash_len and nof_tables must be positive
// time:     O(n * m), where n is the number of hash tables and m is the 
//           length of a hash table
struct countingbloomfilter *cbf_create(int (**hash_funcs)(const char *, int),
                                       int hash_len, int nof_tables);

// cbf_destroy(cbf) frees all resources allocated by counting Bloom filter cbf.
// effects: invalidates cbf
// time: O(n), where n is the number of hash tables in cbf
void cbf_destroy(struct countingbloomfilter *cbf);

// cbf_insert(cbf, key) inserts string key into counting Bloom filter cbf.
// effects: modifies cbf
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(n * m), where n is the number of hash tables in cbf and m is
//       the length of key
void cbf_insert(struct countingbloomfilter *cbf, const char *key);

// cbf_lookup(cbf, key) returns true if the set membership test for string key
//   in counting Bloom filter cbf succeeds and false otherwise.
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(n * m), where n is the number of hash tables in cbf and m is
//       the length of key
bool cbf_lookup(const struct countingbloomfilter *cbf, const char *key);

// cbf_remove(cbf, key) removes string key from counting Bloom
//   filter cbf. The function returns:
//   * CBF_SUCCESS: if key was removed from cbf, or
//   * CBF_FAILURE: if key was not removed from cbf.
// effects: modifies cbf (only if CBF_SUCCESS)
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(n * m), where n is the number of hash tables in cbf and m is
//       the length of key
int cbf_remove(struct countingbloomfilter *cbf, const char *key);

// cbf_print(cbf) prints the content of counting Bloom filter cbf to the 
//   console.
// effects: creates output
// time: O(n * m), where n is the number of hash tables in cbf and m their
//       lengths
void cbf_print(const struct countingbloomfilter *cbf);

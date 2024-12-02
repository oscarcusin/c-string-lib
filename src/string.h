#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdio.h>

typedef struct string string;

// Creates and allocates a new empty string
// Returns a pointer to the new string, NULL on failure
string * string_new();

// Creates and allocates a new string from unsigned char buffer BUFF of size SIZE
// Returns a pointer to the new string, NULL on failure
string * string_new_from_buff(unsigned char * buff, size_t size);

// Creates and allocates a new string from null-terminated char buffer CSTR
// Returns a pointer to the new string, NULL on failure
string * string_new_from_cstr(char * cstr);

// Creates and allocates a new string from file FILE
// Returns a pointer to the new string, NULL on failure
string * string_new_from_file(FILE * file);

// Creates and allocates a copy of string S
// Returns a pointer to the new string, NULL on failure
string * string_copy(string * s);

// Creates and allocates a new string from the substring of string S starting at index START and ending at index END
// Returns a pointer to the new string, NULL on failure
string * string_substring(string * s, size_t start, size_t end);

// Deletes string S, freeing its memory
void string_delete(string * s);

// Returns the length of string S
size_t string_length(string * s);

// Returns the capacity of string S
size_t string_capacity(string * s);

// Returns a pointer to the raw data of string S
unsigned char * string_raw(string * s);

// Outputs string S to file FILE
void string_output(string * s, FILE * file);

// Returns -1 if S1 < S2, 0 if S1 == S2, 1 if S1 > S2
int string_compare(string * s1, string * s2);

// Computes the Levenshtein edit distance between strings S1 and S2
// Cost of each operation is INSERT_COST, DELETE_COST, and REPLACE_COST
// Returns the edit distance, -1 on failure
int string_edit_distance(string * s1, string * s2, int insert_cost, int delete_cost, int replace_cost);

// Inserts character C at index INDEX in string S
// Returns 0 on success, 1 on failure
int string_insert_char(string * s, unsigned char c, size_t index);

// Inserts string OTHER at index INDEX in string S
// Returns 0 on success, 1 on failure
int string_insert_str(string * s, string * other, size_t index);

// Appends character C to string S
// Returns 0 on success, 1 on failure
int string_append_char(string * s, unsigned char c);

// Appends string OTHER to string S
// Returns 0 on success, 1 on failure
int string_append_str(string * s, string * other);

// Erases LENGTH characters starting at index INDEX in string S
// Returns 0 on success, 1 on failure
int string_erase(string * s, size_t index, size_t length);

// Computes the border array B of string S
// B must be of size at least string_length(S)
// Supports strings of length up to INT_MAX
// Returns 0 on success, 1 on failure
int string_border_array(string * s, int b[]);

// Computes the suffix array SA of string S using the SA-IS algorithm
// SA must be of size at least string_length(S)
// Supports strings of length up to INT_MAX
// Returns 0 on success, 1 on failure
int string_suffix_array(string * s, int sa[]);

// Computes the longest common prefix array LCP of string S using the Phi-algorithm
// SA must be the suffix array of S
// LCP must be of size at least string_length(S)
// Supports strings of length up to INT_MAX
// Returns 0 on success, 1 on failure
int string_lcp_array(string * s, int sa[], int lcp[]);

// Callback function type for string search functions
// CTX can be used to pass additional data to the callback
// INDEX is the index of the match in the searched string
typedef void (* match_callback_t) (void * ctx, size_t index);

// Searches for string P in string S using naive search
// If provided, calls CB with CTX and the index of each match
// Returns the number of matches
size_t string_search_naive(string * s, string * p, match_callback_t cb, void * ctx);

// Searches for string P in string S using the Knuth-Morris-Pratt algorithm
// If provided, calls CB with CTX and the index of each match
// Returns the number of matches
size_t string_search_kmp(string * s, string * p, match_callback_t cb, void * ctx);

// Searches for string P in string S using the Sunday variant of the Boyer-Moore algorithm
// If provided, calls CB with CTX and the index of each match
// Returns the number of matches
size_t string_search_bms(string * s, string * p, match_callback_t cb, void * ctx);

// Searches for string P in string S using the Rabin-Karp algorithm
// If provided, calls CB with CTX and the index of each match
// Returns the number of matches
size_t string_search_rk(string * s, string * p, match_callback_t cb, void * ctx);

// Sorts the strings S of length SIZE using the MSD Radix Sort algorithm
// Returns 0 on success, 1 on failure
int string_sort_msd(size_t size, string * s[size]);

#endif

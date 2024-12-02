# C String Library

A simple yet efficient dynamic string library in C, providing a set of functions to create, manipulate and manage dynamic strings conveniently. It includes advanced features such as various string search algorithms, string sorting and more.

## Functions

Return types and documentation are omitted for brevity. Please refer to [string.h](src/string.h) for detailed documentation.

- **String Creation**:
  - `string_new()`
  - `string_new_from_buff(unsigned char * buff, size_t size)`
  - `string_new_from_cstr(char * cstr)`
  - `string_new_from_file(FILE * file)`
  - `string_copy(string_t * s)`
  - `string_substring(string_t * s, size_t start, size_t end)`
- **String Deletion**:
  - `string_delete(string_t * s)`
- **String Properties**:
  - `string_length(string_t * s)`
  - `string_capacity(string_t * s)`
  - `string_raw(string_t * s)`
- **String Comparison**:
  - `string_compare(string_t * s1, string_t * s2)`
  - `string_edit_distance(string_t * s1, string_t * s2, int insert_cost, int delete_cost, int replace_cost)`
- **String Manipulation**
  - `string_insert_char(string * s, unsigned char c, size_t index)`
  - `string_insert_str(string * s, string * other, size_t index)`
  - `string_append_char(string * s, unsigned char c)`
  - `string_append_str(string * s, string * other)`
  - `string_erase(string * s, size_t index, size_t length)`
- **String Analysis**: Border Array, Suffix Array, LCP Array
  - `string_border_array(string * s, int b[])`
  - `string_suffix_array(string * s, int sa[])`
  - `string_lcp_array(string * s, int sa[], int lcp[])`
- **String Search**: Naive, Knuth-Morris-Pratt, Sunday (Boyer-Moore), Rabin-Karp
  - `string_search_naive(string * s, string * p, match_callback_t cb, void * ctx)`
  - `string_search_kmp(string * s, string * p, match_callback_t cb, void * ctx)`
  - `string_search_bms(string * s, string * p, match_callback_t cb, void * ctx)`
  - `string_search_rk(string * s, string * p, match_callback_t cb, void * ctx)`
- **String Sorting**: MSD Radix Sort w/ Insertion Sort
  - `string_sort_msd(size_t size, string * s[size])`

## Usage

The library is not runnable by itself, rather it is meant to be included in other projects. Two methods are provided to use the library:

### As a static library

Compile the library using `make lib` from within [src/](src/), which will create a static library `libstring.a`. You can then include the library in your project by adding the flags `-L<path/to/libsting.a> -lstring` to your compiler. Include [string.h](src/string.h) in your source files.

### As source files

Alternatively, you can use the library from within [src/](src/) directly, using [src/main.c](src/main.c) as the entry point.

- `make main` to compile with -O3 optimization.
- `make debug` to compile with -g debugging information.
- `make run` to compile (if necessary) and run.
- `make run-debug` to compile (if necessary) and `gdb` run.

The [makefile](src/makefile) is only tested on Linux and may not work on other systems.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.
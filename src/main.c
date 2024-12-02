#include <stdio.h>
#include "string.h"

int main(int argc, char * argv[]) {
    string * s = string_new_from_cstr("Hello, World!");
    string_output(s, stdout);
    putc('\n', stdout);
    string_delete(s);
    return 0;
}

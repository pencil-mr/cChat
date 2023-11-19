#include "build.h"
#include <stdio.h>

int main(void) {
    dstring cflags = {};
    dstring_init(&cflags);
    dstring_append_string(&cflags, "-Wall ");
    dstring_append_null(&cflags);

    dstring src = {};
    dstring_init(&src);
    dstring_append_string(&src, "src/main.c ");
    dstring_append_null(&src);

    dstring command = {};
    dstring_init(&command);
    dstring_append_string(&command, "gcc ");
    dstring_append_string(&command, cflags.items);
    dstring_append_string(&command, "-o chatServer ");
    dstring_append_string(&command, src.items);
    dstring_append_null(&command);
    printf("%s", command.items);
    system(command.items);
}

#ifndef BUILD_H
#define BUILD_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define BASE_SIZE 8

#define DA_TEMPLATE(TYPE) \
struct { \
    size_t size; \
    size_t alloc_size; \
    TYPE*  items; \
}

#define DA_INIT(TYPE) { \
    da->size       = 0; \
    da->alloc_size = 0; \
    da->items      = NULL; \
}

#define DA_append(TYPE) { \
    if (da->alloc_size == 0) { \
        da->alloc_size = BASE_SIZE; \
        da->items = malloc(BASE_SIZE * sizeof(TYPE)); \
    } \
    if (da->alloc_size < da->size + 1) { \
        da->alloc_size = da->alloc_size * 2; \
        da->items = realloc(da->items, sizeof(TYPE) * da->alloc_size); \
    } \
    *(da->items + da->size) = item; \
    da->size = da->size + 1; \
}

typedef DA_TEMPLATE(char) dstring;

void dstring_init(dstring* da) DA_INIT(char)
void dstring_append(dstring* da, char item) DA_append(char)
void dstring_free(dstring* da) {
    free(da->items);
    da->items = NULL;
}
void dstring_append_string(dstring* dstr, char* string) {
    for (int i = 0; i < strlen(string); ++i) {
        dstring_append(dstr, *(string+i));
    }
}
void dstring_append_null(dstring* dstr) {
    dstring_append(dstr, '\0');
}

#endif // BUILD_H

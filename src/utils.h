#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

typedef struct {
    size_t cap;
    nfds_t size;
    struct pollfd *items;
} pollfds;

void append(pollfds* array, struct pollfd item) {
    if (array->cap == 0) {
        array->cap = 8;
        array->items = malloc(array->cap*sizeof(struct pollfd));
    } else if (array->size >= array->cap) {
        array->cap *= 2;
        array->items = realloc(array->items, array->cap);
    }
    array->items[array->size++] = item;
}

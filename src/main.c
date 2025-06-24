#include "utils.h"

const int PORT = 8080;

#define ADDRESS "0.0.0.0"

int handle_event(struct pollfd *pfd) {
    if (pfd->revents & POLLIN) {
    }
    else if (*pfd->revents & POLLOUT) {
        char buf[1024];
        memset(buf, '\0', 1024);
        int len = read(pfd->, buf, 1024);
        while (len > 1) {
            len = read(conn_fd, buf, 1024);
            write(serv_fd, buf, len);
        }
    }
}

int main() {
    struct sockaddr_in serv_addr = {0};
    struct sockaddr_in conn_addr = {0};
/*
    TODO: possibly can run out of memory if there are too many connnections
    change to a different way of storing pollfd in the future
    maybe epoll would be better since it probably won't be run on anything other than linux
*/
    pollfds fds = {0};
    int rp = 0;
    size_t it = 0;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    int serv_len = sizeof(serv_addr);

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0) {
        perror("ERROR: socket init");
        goto end;
    }

    append(&fds, (struct pollfd){STDIN_FILENO, POLLIN, 0});
    append(&fds, (struct pollfd){serv_fd, POLLIN, 0});

    if (bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len) < 0) {
        perror("ERROR: socket bind");
        goto end;
    }

    if (fcntl(serv_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("ERROR: fcntl");
        goto end;
    }

    if (listen(serv_fd, 16) < 0) {
        perror("ERROR: socket listen");
        goto end;
    }
    printf("Listening to port %d ...\n", PORT);

    unsigned int conn_len = sizeof(conn_addr);

    rp = poll(fds.items, fds.size, -1);
    while (rp > 0) {
        for (it = 0; it < fds.size; ++it) {
            if (fds.items[it].revents != 0) {
                handle_event(&fds.items[it]);
                --rp;
            }
        }
    }

    int conn_fd = accept(serv_fd, (struct sockaddr*)&conn_addr, &conn_len);
    append(&fds, (struct pollfd){conn_fd, POLLOUT, 0});
    for (size_t i = 3; i < fds.size; ++i) {
        close();
    }

end:
    close(serv_fd);
    free(fds.items);
}

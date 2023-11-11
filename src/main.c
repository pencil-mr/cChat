#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 8000;

void handle_connect(int conn_fd) {
    char buf[1024];
    read(conn_fd, buf, 1024);
    prinf("%s", buf);
    close(conn_fd);
}

int main() {
    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    int serv_len = sizeof(serv_addr);

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0) {
        perror("ERROR: sock init");
        goto end;
    }

    if (bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len) < 0) {
        perror("ERROR: sock bind");
        goto end;
    }

    if (listen(serv_fd, 32) < 0) {
        perror("ERROR: sock bind");
        goto end;
    }

    struct sockaddr_in conn_addr = {0};

    unsigned int conn_len = sizeof(conn_addr);

    int conn_fd = accept(serv_fd, (struct sockaddr*)&serv_addr, &conn_len);
    if (conn_fd < 0) {
        perror("ERROR: connection accept");
        close(conn_fd);
        goto end;
    }
    handle_connect(conn_fd);
end:
    close(serv_fd);
}

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

const int PORT = 8080;

void handle_connect(int conn_fd, char* addr) {
    char buf[1024] = {'\0'};
    int len = read(conn_fd, buf, 1024);
    if (len > 2) {
        printf("Info: %s sent message %s\n", addr, buf);
        handle_connect(conn_fd, addr);
    }
    close(conn_fd);
    printf("Info: %s disconnected.\n", name);
}

int main() {
    struct sockaddr_in serv_addr = {};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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

    if (listen(serv_fd, 2) < 0) {
        perror("ERROR: sock bind");
        goto end;
    }
    printf("Listening to port %d ...\n", PORT);

    struct sockaddr_in conn_addr = {0};

    unsigned int conn_len = sizeof(conn_addr);

    int conn_fd = accept(serv_fd, (struct sockaddr*)&conn_addr, &conn_len);
    if (conn_fd < 0) {
        perror("ERROR: connection accept");
        close(conn_fd);
        goto end;
    }
    struct in_addr urip = conn_addr.sin_addr;
    char name[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &urip, name, INET_ADDRSTRLEN);
    printf("Info: %s connected.\n", name);
    handle_connect(conn_fd, name);
end:
    close(serv_fd);
}

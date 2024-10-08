#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error_handling(char *message) {
    perror(message);
    exit(1);
}

void handle_client(int clnt_sock) {
    char message[BUFFER_SIZE];
    int str_len;

    while (1) {
        str_len = read(clnt_sock, message, sizeof(message) - 1);
        if (str_len == -1) {
            perror("read() error");
            close(clnt_sock);
            return;
        }

        if (str_len == 0) {
            printf("Client disconnected.\n");
            close(clnt_sock);
            return;
        }

        message[str_len] = '\0';
        printf("Message from client: %s\n", message);

        if (strcmp(message, "terminate\n") == 0) {
            printf("Client requested termination.\n");
            close(clnt_sock);
            return;
        }

        if (write(clnt_sock, message, str_len) == -1) {
            perror("write() error");
            close(clnt_sock);
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            perror("accept() error");
            continue;
        }

        printf("Connected to client: %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        handle_client(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

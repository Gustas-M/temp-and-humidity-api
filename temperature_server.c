#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <libbmp280.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_sock, buffer, sizeof(buffer) - 1);

    if (strncmp(buffer, "GET /get_temp", 13) == 0) {
        float temp = read_temperature();
        char response[128];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n%.2f C\n", temp);
        write(client_sock, response, strlen(response));

    } else if (strncmp(buffer, "GET /get_press", 14) == 0) {
        float press = read_pressure();
        char response[128];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n%.2f Pa\n", press);
        write(client_sock, response, strlen(response));

    } else {
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found\n";
        write(client_sock, not_found, strlen(not_found));
    }

    close(client_sock);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 4) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        handle_client(client_sock);
    }

    close(server_fd);
    return 0;
}


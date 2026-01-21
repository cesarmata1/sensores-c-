#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TCP_PORT 6002

int main() {
    int sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[50];
    socklen_t addr_len = sizeof(client_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sock, 5);

    printf("Actuador de Temperatura esperando conexiones...\n");

    while (1) {
        client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Activar climatizador para  %s grados\n", buffer);
        close(client_sock);
    }

    close(sock);
    return 0;
}

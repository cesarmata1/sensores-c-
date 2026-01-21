#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define UDP_PORT 5005
#define TCP_IP "172.20.10.2"

#define TCP_PORT_LUZ 6001
#define TCP_PORT_TEMP 6002

int main() {
    int sock_udp;
    struct sockaddr_in server_addr, client_addr;
    char buffer[50];
    socklen_t addr_len = sizeof(client_addr);

    sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock_udp, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        recvfrom(sock_udp, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        buffer[strcspn(buffer, "\n")] = 0;
        
        char tipo[20];
        int medida;
        sscanf(buffer, "%[^,],%d", tipo, &medida);
        printf("Gestor recibio: %s - %d\n", tipo, medida);

        if ((strcmp(tipo, "temperatura") == 0 && (medida < 19 || medida > 24)) || 
            (strcmp(tipo, "luz") == 0 && medida < 200)) {
            
            int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in actuador_addr;
            actuador_addr.sin_family = AF_INET;
            actuador_addr.sin_port = htons((strcmp(tipo, "temperatura") == 0) ? TCP_PORT_TEMP : TCP_PORT_LUZ);
            actuador_addr.sin_addr.s_addr = inet_addr(TCP_IP);

            if (connect(sock_tcp, (struct sockaddr*)&actuador_addr, sizeof(actuador_addr)) < 0) {
                perror("Error al conectar con el actuador");
                close(sock_tcp);
                continue;
            }

            send(sock_tcp, buffer, strlen(buffer), 0);
            printf("Gestor envio a Actuador %s: %d\n", tipo, medida);
            close(sock_tcp);
        }
    }

    close(sock_udp);
    return 0;
}

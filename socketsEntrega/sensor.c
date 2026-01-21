#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "172.20.10.2"
#define SERVER_PORT 5005

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[50];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    srand(time(NULL));

    while (1) {
        
        int tipo;
        int medida;
        char tipoMedida[20];

        printf(" Selecione el tipo que quiere (1 = Temp | 2 = luz | 3 = salir) : " );
        scanf("%d",&tipo);

        if (tipo == 1) {
            medida =  (rand() % 16 + 15);
            strcpy(tipoMedida,"temperatura");
        } else if (tipo == 2) {
            medida = (rand() % 201 + 100);
            strcpy(tipoMedida,"luz");
        } else if (tipo == 3)
        {
            printf("cerrando\n");
            break;
        } else 
        {
            printf("error\n");
            continue;
        }
        
        sprintf(message, "%s,%d", tipoMedida, medida);
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        printf("Sensor envio: %s\n", message);
    }
    
    close(sock);
    return 0;
}

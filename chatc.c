#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h> 


int main() {
    int client_socket, port;
    struct sockaddr_in server_address;

    printf("Enter server port number: ");
    scanf("%d", &port);

    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    char buffer[1024];

    while (1) {
        printf("Client (type 'exit' to quit): ");
        scanf("%s", buffer);
        send(client_socket, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client is closing...\n");
            break;
        }

        recv(client_socket, buffer, sizeof(buffer), 0);
        if (strcmp(buffer, "exit") == 0) {
            printf("Server has left the chat.\n");
            break;
        }
        printf("Server: %s\n", buffer);
    }

    
    close(client_socket);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
int main() {
    int server_socket, client_socket, port, len;
    struct sockaddr_in server_address, client_address;

    printf("Enter port number: ");
    scanf("%d", &port);

    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr =inet_addr("127.0.0.1");

    
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", port);

    len = sizeof(client_address);
    char buffer[1024];

    
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &len);
    if (client_socket < 0) {
        perror("Connection failed");
        exit(1);
    }

    while (1) {
        recv(client_socket, buffer, sizeof(buffer), 0);
        if (strcmp(buffer, "exit") == 0) {
            printf("Client has left the chat.\n");
            break;
        }
        printf("Client: %s\n", buffer);

        printf("Server (type 'exit' to quit): ");
        scanf("%s", buffer);
        send(client_socket, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Server is closing...\n");
            break;
        }
    }

    
    close(client_socket);
    close(server_socket);

    return 0;
}

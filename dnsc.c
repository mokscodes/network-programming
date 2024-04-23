#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    while (1) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(8081);
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        char domain[100]; // Buffer to store domain name
        printf("Enter the domain name to search (or type 'exit' to quit): ");
        scanf("%s", domain);

        if (strcmp(domain, "exit") == 0) {
            printf("Exiting...\n");
            break; // Exit the loop if user types 'exit'
        }

        sendto(sock, domain, strlen(domain), 0, (struct sockaddr*)&server_address, sizeof(server_address));

        char buffer[1024];
        int len = sizeof(server_address);
        int n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&server_address, &len);
        buffer[n] = '\0';
        printf("Server's response: %s\n", buffer);

        close(sock);
    }

    return 0;
}
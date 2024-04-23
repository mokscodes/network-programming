#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in serveraddr, cliaddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(8081);
    
    if (bind(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    int len, n;
    len = sizeof(cliaddr);
    
    while (1) {
        char buffer[1024];
        n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        
        printf("Client request for domain: %s\n", buffer);
        
        char *response_ip;
        if (strcmp(buffer, "google.com") == 0) {
            response_ip = "142.250.180.78"; // Arbitrary IP for google.com
        } else if (strcmp(buffer, "yahoo.com") == 0) {
            response_ip = "74.6.143.25"; // Arbitrary IP for yahoo.com
        } else if (strcmp(buffer, "youtube.com") == 0) {
            response_ip = "216.58.214.14"; // Arbitrary IP for youtube.com
        } else {
            response_ip = "0.0.0.0"; // Default response if domain is not recognized
        }
        
        sendto(sockfd, response_ip, strlen(response_ip), 0, (struct sockaddr *)&cliaddr, len);
        
        // Prompt for domain name again
        printf("Waiting for next domain...\n");
    }
    
    // Control never reaches here
    return 0;
}
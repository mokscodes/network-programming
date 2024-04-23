#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

int main() {
    struct addrinfo hints, *server;
    int r, x, sockfd, clientfd;
    struct sockaddr_storage client_address; // corrected data type
    socklen_t client_len;
    const int buffer_size = 1024;
    char buffer[buffer_size];
    const char *http_data =
        "HTTP/1.1 200 OK\r\n"
        "Connection:close\r\n"
        "Content-Type:text/html\r\n\r\n"
        "<body bgcolor='yellow'>"
        "<h1>hello from your server shashi!</h1>"
        "</body>";

    /* configure the server to use localhost, port 8080, TCP */
    printf("configuring server...\n");
    memset(&hints, 0, sizeof(struct addrinfo)); /* use memset_s() */
    hints.ai_family = AF_INET;                /* IPv4 connection */
    hints.ai_socktype = SOCK_STREAM;           /* TCP, streaming */
    hints.ai_flags = AI_PASSIVE;              /* for bind() */

    r = getaddrinfo(NULL, "8080", &hints, &server);
    if (r != 0) {
        perror("failed");
        exit(1);
    }
    puts("done");

    /* create the socket to use for communications */
    printf("Assign a socket...\n");
    sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (sockfd == -1) {
        perror("failed");
        exit(1);
    }
    puts("done");

    /* binds the server to a socket */
    printf("binding socket...\n");
    r = bind(sockfd, server->ai_addr, server->ai_addrlen);
    if (r == -1) {
        perror("failed");
        exit(1);
    }
    puts("done");

    /* listening for incoming connections */
    printf("listening...\n");
    r = listen(sockfd, 1);
    if (r == -1) {
        perror("failed");
        exit(1);
    }
    puts("done");

    /* accepting a new connection */
    printf("accepting new connection\n");
    client_len = sizeof(client_address);
    clientfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
    if (clientfd == -1) {
        perror("failed");
        exit(1);
    }
    printf("connected on file descriptor %d\n", clientfd);

    /* client now connected */
    r = recv(clientfd, buffer, buffer_size, 0);
    if (r > 0) {
        printf("received %d bytes:\n---\n", r);
        for (x = 0; x < r; x++)
            putchar(buffer[x]);
    }

    /* send the response */
    r = send(clientfd, http_data, strlen(http_data), 0);
    if (r < 1) {
        perror("send failed");
        exit(1);
    }
    printf("sent %d bytes\n", r);

    /* close the client socket */
    close(clientfd);

    /* free allocated memory */
    freeaddrinfo(server);

    /* close the socket */
    close(sockfd);
    puts("socket closed, done");
    return (0);
}

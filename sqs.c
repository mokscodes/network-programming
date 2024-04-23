#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BUFFER_SIZE 1024

int main(){
    int number, numberSquare, server_socket, client_socket, bind_status, listen_status;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE] = {0};

    //Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Defining the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr =inet_addr("127.0.0.1");

    //Binding the Socket and IP 
    bind_status = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if(bind_status < 0 ){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    //Listen to the incoming server
    listen_status = listen(server_socket, 5);
    if(listen_status < 0){
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    //Accepting the client connection
    int len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&len);
    if(client_socket < 0){
        perror("Accepting failed!");
        exit(EXIT_FAILURE);
    }

    printf("New connection from IP: %s and PORT : %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    number = atoi(buffer);
    
    numberSquare = number * number;

    // Convert squared number to string
    sprintf(buffer, "%d", numberSquare);

    // Send squared number to the client
    send(client_socket, buffer, sizeof(buffer), 0);

    printf("Sent squared number : %d\n", numberSquare);

    close(client_socket);
    close(server_socket);

    return 0;
}
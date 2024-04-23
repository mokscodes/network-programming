#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BUFFER_SIZE 1024

int main(){

    int client_socket,connection_status;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

   //Create a socket
    client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //defining the address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect the client to server
    connection_status = connect(client_socket,(struct sockaddr *) &server_address,sizeof(server_address));
    if(connection_status < 0){
        perror("Connection failed to server...");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server...\n");
    
    printf("Enter the number to get squared :");
    fgets(buffer,BUFFER_SIZE,stdin);

    send(client_socket,buffer,strlen(buffer),0);
    printf("Number sent to server...");

    recv(client_socket,buffer,BUFFER_SIZE,0);
    printf("Number of the squared is : %s",buffer);

    //closing the socket
    close(client_socket);
    
    return 0;
}
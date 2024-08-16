#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024
#define nullptr ((void*)0)

int main()
{
    WSADATA wsa_data;
    //CHECKING FOR SUCCESS OF WSA INTIALIZATION
    int check = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if(check != 0){
        printf("wsastartup failed. error code : %d\n", WSAGetLastError());
        return 1;
    }

//CREATING SERVER SOCKET
    SOCKET client_socket;
    client_socket = INVALID_SOCKET;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//CHECK FOR SOCKETS SUCCESSFULL CREATION
    if(client_socket == INVALID_SOCKET)
    {

        printf("error in creating client socket: %d\n",WSAGetLastError());
        WSACleanup();
        return 0;
    }
    else
    {
            printf("client socket successfully created\n");
    }

     while(1)
    {

//CONNECTING TO THE SERVER
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

//CONNECTING TO THE SERVER
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("connection failed");
        WSACleanup();
        return 1;
    }
    printf("connected to the server");



        //SENDING THE DATA
    char snd_buffer[BUFFER_SIZE];
    printf("\nENTER DATA TO SEND TO THE SERVER");
    fgets(snd_buffer, BUFFER_SIZE, stdin);
    int sbyte_count = send(client_socket, snd_buffer, BUFFER_SIZE, 0);
    if(sbyte_count == SOCKET_ERROR)
    {
        printf("client sending error");
        return -1;
    }
    else
    {
        printf("client sent %d bytes",sbyte_count);
    }

//RECEIVING DATA FROM THE SERVER
    char rcv_buffer[BUFFER_SIZE];
    int rbyte_count = recv(client_socket, rcv_buffer, BUFFER_SIZE, 0);
    if(rbyte_count < 0)
    {
        printf("nothing received");
    }
    else
    {
        rcv_buffer[rbyte_count] = '\0'; //NULL TERMINATING TTHE RECVD DATA
        printf("data recieved:  %s\n", rcv_buffer);
    }
    }


return 0;
}

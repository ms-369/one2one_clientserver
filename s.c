#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024
#define nullptr ((void*)0)

void log_message(const char *message, char type)
{
    FILE *log_file = fopen("server_log.txt","a"); //CREATES AN APPEND ONLY FILE NAMED SERVER_LOG
    if(log_file == NULL)
    {
        perror("FAILED TO OPEN/CREATE FILE");
        return -1;
    }

    time_t present;
    time(present);
    struct tm *local = localtime(&present);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local);

    char* msg_type = ' ';
    switch(type)
    {
    case 'q':
        msg_type = "request";
        break;
    case 'a':
        msg_type = "response";
        break;
    default:
        msg_type = "terminated";
        break;
    }

    fprintf(log_file, "[%s] %s : %s\n", time_str, msg_type, message);
    fclose(log_file);
}

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
    SOCKET server_socket;
    server_socket = INVALID_SOCKET;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//CHECK FOR SOCKETS SUCCESSFULL CREATION
    if(server_socket == INVALID_SOCKET)
    {

        printf("error in creating socket: %d\n",WSAGetLastError());
        WSACleanup();
        return 0;
    }
    else
    {
            printf("server socket successfully created\n");
    }


//BINDING THE SOCKET TO AN IP ADDRESS AND PORT NUMBER
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if(bind(server_socket,  (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("server binding failed");
        WSACleanup();
        return 0;
    }
    else
    {
            printf("server binding successfulll");
    }

      while(1)
    {

//LISTENING FOR CONNECTIONS
    if(listen(server_socket,1) == SOCKET_ERROR)
    {
        printf("error listening on server port");
    }
    else
    {
        printf("\n server is listening on port %d", PORT);
    }

//ACCEPTING INCOMING CONNECTIONS
    SOCKET accept_socket;
    accept_socket = accept(server_socket, nullptr, nullptr);
//CHECKING FOR SUCCESSFULL CONNECTION
    if(accept_socket == INVALID_SOCKET)
    {
        printf("acception failed\n");
        WSACleanup();
        return -1;
    }
    else
    {
        printf("acception successfull");
    }



        //RECEVIN REQUEST FROM CLIENT
    char rcv_buffer[BUFFER_SIZE];
    int rbyte_count = recv(accept_socket, rcv_buffer, BUFFER_SIZE, 0);
    if(rbyte_count == SOCKET_ERROR)
    {
        printf("receiv failed");
    }
    else if(rbyte_count == 0)
    {
         printf("client disconnected");
         break;
    }
    else
    {
        rcv_buffer[rbyte_count] = '\0'; //NULL TERMINATING TTHE RECVD DATA
        printf("data recieved:  %s\n", rcv_buffer);
        log_message(rcv_buffer, 'q');
    }

//SENDING THE RESPONSE
    char snd_buffer[BUFFER_SIZE];
    printf("\nENTER DATA TO SEND");
    fgets(snd_buffer, BUFFER_SIZE, stdin);
    int sbyte_count = send(accept_socket, snd_buffer, BUFFER_SIZE, 0);
    if(sbyte_count == SOCKET_ERROR)
    {
        printf("server sending error");
        return -1;
    }
    else
    {
        printf("server sent %d bytes",sbyte_count);
        snd_buffer[rbyte_count] = '\0'; //NULL TERMINATING TTHE RECVD DATA
        log_message(snd_buffer, 'a');

    }
    }



    return 0;
}

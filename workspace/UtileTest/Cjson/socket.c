#include "socket.h"
#include "global.h"
#include "stdio.h"
int clientsocket(struct sockaddr_in *soc){
    if(m_tcp.clientSocket>CLIENT_SOCKET){
        printf("超过最大套接字数\r\n");
        perror("bigger than clientSocket_num");
    }
    if((m_tcp.clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    perror("socket");
    return 1;
    }

    if(connect(m_tcp.clientSocket, (struct sockaddr *)soc, sizeof(struct sockaddr_in)) < 0)
    {
    perror("connect");
    return 1;
    }
    return m_tcp.clientSocket;
}

#ifndef  _GLOBAL_H
#define  _GLOBAL_H
#include <stdbool.h>
/*
 * @Description:
 *   - 
 * @Author:  ylx
 * @Author:  golang
 * @CreatDate: 2021-04-30 19:55:08
*/
#define  DEBUG 1
#define IP "0.0.0.0"
#define SERVER_PORT 8999 
#define THREAD_POOL 1  //设置线程池大小
#define TCP_REC_SEND "tcp_rec_send"
#define CLIENT_SOCKET 200//设置最大套接字数
#define MAX_JSON 1024

//客户端只需要一个套接字文件描述符，用于和服务器通信
typedef  struct tcpclientstruct
{
    int clientSocket;
    const char *sendbuf;
    bool send_c;
    char recvbuf[200];
    bool rec_c;
    int id;
    int reclen;
    int sendlen; 

}tcpclientstruct;
tcpclientstruct m_tcp;


//payload 结构体
typedef struct bsstruct
{
    int machineid;
    char txtime[20];
    int sensorid;
    int workid;
    char owner[10];
    char macid[50];
    char action[10];
}bsstruct;






#endif // ! _GLOBAL_H
#include "mssage.h"
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include "global.h"


int sendmssage(int clientSocket,unsigned int id,const char * buf)
{
    char sendbuf[200];
    message m;
    memset(m.buf,'\0',sizeof(m.buf));   
    memcpy(m.buf,buf,strlen(buf));
    m.id=(id);
    m.datelen=strlen(buf);

    //清空包
    memset(sendbuf,'\0',sizeof(sendbuf));
    //    小端组包
    //id+datelen头
    sendbuf[7] = m.id >> 24;     
    sendbuf[6] = m.id >> 16;
    sendbuf[5] = m.id >> 8;
    sendbuf[4] = m.id;
    sendbuf[3] = m.datelen >> 24;
    sendbuf[2] = m.datelen >> 16;
    sendbuf[1] = m.datelen >> 8;
    sendbuf[0] = m.datelen;
    //发送内容
    memcpy(&sendbuf[8],m.buf,m.datelen);
    #if DEBUG
    // for (int i = 0; i < m.datelen+8; i++)
    // {
    //     printf("%d ，%d\r\n",sendbuf[i],i);    
    // }
    #endif
    send(clientSocket,sendbuf,m.datelen+8, 0);

    return strlen(buf);
}


int recmssage(int clientSocket,char * buf){
    int iDataNum = recv(clientSocket, buf, 200, 0); 
    buf[iDataNum] = '\0';
    // *(buf+iDataNum) = '\0';
    #if DEBUG
    for (int i = 0; i < iDataNum; i+=8)
    {
    printf("%c,%c,%c,%c,%c,%c,%c,%c\r\n"
    ,buf[i],buf[i+1],buf[i+2],buf[i+3],buf[i+4],buf[i+5],buf[i+6],buf[i+7]);    
    }
    // for (int i = 0; i < iDataNum; i++)
    // {
    // printf("%c",*(buf+i));    
    // }
    #endif  
    return iDataNum;
}
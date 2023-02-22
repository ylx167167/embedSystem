#ifndef  _MSSAGE_H
#define  _MSSAGE_H
/*
 * @Description: 
 *   - 
 * @Author:  ylx
 * @Author:  golang
 * @CreatDate: 2021-04-30 19:56:00
*/
typedef struct message{
         unsigned int  id;
         unsigned int  datelen;
         char buf[200];

 }message;


int sendmssage(int clientSocket,unsigned int id,const char * buf);

int recmssage(int clientSocket, char * buf);

#endif
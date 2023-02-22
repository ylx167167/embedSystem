#include "thread.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
pthread_t thread[2];  //两个线程

void *send_rec_mssage_thread(void * p){
    printf("线程1判断\n");
    while (1)
    {
    if(m_tcp.send_c ==true){
        // printf("m_tcp.send_c  :%d \r\n",m_tcp.send_c );
    pthread_mutex_lock(&mut);
    m_tcp.sendlen = sendmssage(m_tcp.clientSocket,m_tcp.id,m_tcp.sendbuf);
    // sleep(2);
    // m_tcp.reclen =  recmssage(m_tcp.clientSocket,m_tcp.recvbuf);

    printf("m_tcp.sendlen:%d",m_tcp.sendlen);
    m_tcp.send_c=false;
    // m_tcp.id=0;      
     memset((char *)&m_tcp.sendbuf,0,sizeof(m_tcp.sendbuf)); 

    }
    sleep(1);
    m_tcp.reclen =  recmssage(m_tcp.clientSocket,m_tcp.recvbuf);
    printf("m_tcp.reclen:%d",m_tcp.reclen);
    pthread_mutex_unlock(&mut);
    
    if(thread_pool[0].close==true){
    pthread_exit(NULL);
    }
    
    }
    
}

void thread_wait(){
    if (thread_pool[0].p!=0)
    {
        pthread_join(thread_pool[0].p,NULL);
        printf("线程1已退出\n");
    }

}

void thread_create(){
    pthread_mutex_init(&mut,NULL);
    int temp;
    printf("create_thread\n");    
    if((temp = pthread_create(&thread_pool[0].p, NULL, send_rec_mssage_thread, NULL)) != 0)  //comment2     
        {
            printf("线程1创建失败!\n");
        }
    else{
        strcpy(thread_pool[0].name,TCP_REC_SEND);
        thread_pool[0].close=false; 
        }
 
}



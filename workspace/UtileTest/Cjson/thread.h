#ifndef _THREAD_H
#define _THREAD_H
#include "mssage.h"
#include "global.h"
#include <pthread.h>
/*
 * @Description: 
 *   - 
 * @Author:  ylx
 * @Author:  golang
 * @CreatDate: 2021-04-30 19:55:33
*/
typedef struct thread_pool
{
    pthread_t p;  //两个线程
    char name[20];
    bool close;
}thread_parm;
thread_parm thread_pool[THREAD_POOL];

pthread_mutex_t mut;
void thread_create();
void thread_wait();
#endif // _THREAD_H

/*
 *Filename         :thread.cpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/18 16:48:29 */

#include "thread.hpp"

using namespace EmbedSystem;

// 每个部件继承一个Thread类，并由每一个部件创建一个线程池 将线程加入到线程池中
// 线程管理类
GTThread::GTThread(_IN_ GTThreadPool* tpool) {
  Init();
  if (tpool) {
    This(threadpool) = tpool;
  }
}

GTThread::~GTThread() {
  This(Exit());
  //
}

void GTThread::Init() {
  This(start_event) = true;
  This(exit_event) = false;
  This(busy_event) = false;
  This(run_overwrite) = false;
  This(threadpool) = NULL;
  This(task_args) = NULL;
  This(one_task) = NULL;
  This(exit_task) = NULL;
}

bool GTThread::IsBusy() { return This(busy_event); }
void GTThread::Exit() {
  This(exit_event) = true;
  This(busy_event) = true;
  // 线程池 修改线程数
  // 判断现在任务数是否和线程数相等
}
void GTThread::Start() {
  This(run_overwrite) = true;
  This(start_event) = true;
}
void GTThread::Stop() {
  This(run_overwrite) = false;
  This(busy_event) = false;
}

int work(void* argc) { return 0; }
bool GTThread::CreateThread() {
  if (!This(threadpool)) return false;  // 确保有线程池
  // // 将工作线程加入到线程池中 参数为该线程的整个类
  This(threadpool)->AddThreadFunc(&WorkThread, this);
  return true;
}
// typedef int EmbedSystem::Thread_Proc(void *)
int GTThread::WorkThread(void* args) {
  if (!args) return 0;
  GTThread* p = reinterpret_cast<GTThread*>(args);

  while (!p->exit_event) {
    p->busy_event = false;  // 线程不忙
    // 线程等待开始
    for (; !p->start_event;)
      ;
    // 判断线程是否退出
    if (p->exit_event) break;
    p->busy_event = true;    // 线程开始忙碌
    if (p->run_overwrite) {  // run函数是否重载
      do {
        p->run(p->task_args);
      } while (p->busy_event);
    } else {
      if (p->one_task) {
        p->one_task(p->task_args);
      }
    }
    if (p->exit_task) {
      p->exit_task(p->task_args);
    }
    p->busy_event = false;  // 线程结束忙碌
    p->one_task = NULL;
    p->exit_task = NULL;
    p->task_args = NULL;
    if (p->exit_event && p->threadpool) {
      // 线程池回收线程
    }
  }
  return 0;
}
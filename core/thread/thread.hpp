#ifndef _THREAD_HPP_
#define _THREAD_HPP_
#include "system/global.hpp"
#include "threadpool.hpp"
/*
 *Filename         :GTThread.hpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/17 11:28:58 */

// 单个线程
namespace EmbedSystem {
// 线程任务
typedef void (*ThreadTaskFun)(_IN_ void* arg);

class GTThread {
 private:
  GTThreadPool* threadpool;  // 所链接的线程池
  bool start_event;          // 线程运行事件
  bool exit_event;           // 退出时间
  bool busy_event;           // 忙时间
  bool run_overwrite;        // run函数是否重载
  void* task_args;           // 线程回调参数
  ThreadTaskFun one_task;    // 单个任务
  ThreadTaskFun exit_task;   // 单个任务

 private:
  static int WorkThread(void* arg);  // 工作线程 由这个线程控制线程状态

  std::function<int(void*)> func;
  Thread_Proc proc;

 public:
  GTThread(_IN_ GTThreadPool* pool);
  ~GTThread();

 public:
  void Init();  // Thread类初始化
  void Start();
  void Stop();
  void Exit();
  bool IsBusy();

 protected:
  virtual void run(_IN_ void* arg) {}  // 每个线程运行要重载这个函数

 public:
  bool CreateThread();  // 创建线程 并将线程处理函数传入其中
};

}  // namespace EmbedSystem

#endif

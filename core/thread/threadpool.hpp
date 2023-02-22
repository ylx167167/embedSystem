
/*
 *Filename         :threadpool.hpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/17 11:21:53 */

#ifndef _THREADPOOL_HPP_
#define _THREADPOOL_HPP_
#include "hipe.h"
typedef int(Thread_Proc)(void*);
// typedef std::function<int(void*)> Thread_Proc;

// 主进程 管理一个线程池，  每个线程管理一个线程池

/* 线程回调函数*/

namespace EmbedSystem {

class Thread;

// 封装Hipe
// 选中线程池类型
typedef enum ThreadPoolType {
  ThreadPool = 0,
  HIPE_STEADY,
  HIPE_BALANCE,
  HIPE_DYNAMIC,
} PoolType;

typedef struct DynamicManager {
  hipe::DynamicThreadPond* pond;
  int max_numb;
  int min_numb;
  bool* managerclosed;

} DynamicManager;

class GTThreadPool {
 private:
  PoolType ptype;
  int numb;
  bool managercloesd;
  int max_thread_numb;  // 最大线程数
  int min_thread_numb;  // 最小线程数
  hipe::BalancedThreadPond* balancepnd;
  hipe::DynamicThreadPond* dynamicpond;
  DynamicManager* dynamicManager;
  hipe::SteadyThreadPond* steadypond;
  // void GTThreadPoolmanager();
  GTThreadPool();
  GTThreadPool(EmbedSystem::PoolType type, int num = 0, int max_thread = 200,
               int min_thread = 2);
  ~GTThreadPool();
  // GTThreadPool(EmbedSystem::PoolType type, int numb = 1, int max_thread =
  // 200, int min_thread = 2); ~GTThreadPool();

 public:
  // 启动单例模式全局只能创建一个线程池
  static GTThreadPool* GetInstance();

  void Init(EmbedSystem::PoolType type, int num = 0, int max_thread = 200,
            int min_thread = 2);

 public:
  void AddThreadFunc(
      Thread_Proc* proc,
      void* args);  // 将线程任务加入到线程池中 线程状态由线程类管理 //
                    // 秉持一个任务一个线程的原则每完成一个任务关闭一个线程
  void AddThreadWorker();
  void DestroyThreadPool();
};
}  // namespace EmbedSystem

#endif
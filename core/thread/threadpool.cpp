

/*
 *Filename         :threadpool.cpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/17 22:13:20 */
#include "threadpool.hpp"

#include "system/global.hpp"

using namespace EmbedSystem;

GTThreadPool::GTThreadPool() : max_thread_numb(200), min_thread_numb(2) {}

GTThreadPool::~GTThreadPool() {
  This(managercloesd) = true;
  delete dynamicManager;
}

GTThreadPool *GTThreadPool::GetInstance() {
  static GTThreadPool *uniqueInstance = NULL;
  if (!uniqueInstance) {
    uniqueInstance = new GTThreadPool();
    return uniqueInstance;
  }
}

void GTThreadPool::Init(_IN_ EmbedSystem::PoolType type, _IN_ int num,
                        _IN_ int max_thread, _IN_ int min_thread) {
  This(ptype) = type;
  This(numb) = num;
  This(max_thread_numb) = max_thread;
  This(min_thread_numb) = min_thread;
  This(managercloesd) = false;
  This(dynamicManager) = NULL;
  This(dynamicpond) = NULL;
  switch (this->ptype) {
    case HIPE_DYNAMIC:
      This(dynamicpond) = new hipe::DynamicThreadPond(this->numb);
      This(dynamicpond)->waitForThreads();
      break;
    case HIPE_BALANCE:
      this->balancepnd = new hipe::BalancedThreadPond(this->numb);
      break;
    case HIPE_STEADY:
      this->steadypond =
          new hipe::SteadyThreadPond(this->numb, this->numb * 10);
      break;
    case ThreadPool:
      /* code */
      break;
    default:
      break;
  }
}

// 为线程池注入执行任务
void GTThreadPool::AddThreadFunc(Thread_Proc *proc, void *args) {
  switch (this->ptype) {
    case HIPE_DYNAMIC:

      LOG__INFO("Add Thread Func");
      This(dynamicpond)->submit(std::bind(proc, args));  // 将任务加入到线程池中
      This(dynamicpond)->addThreads(1);
      // hipe::util::sleep_for_seconds(5);  // 1s
      // This(dynamicpond)->delThreads(1);

      break;
    case HIPE_BALANCE:
      break;
    case HIPE_STEADY:
      break;
    case ThreadPool:
      /* code */
      break;
    default:
      break;
  }
}
// 增加一个线程
void GTThreadPool::AddThreadWorker() {
  // switch (this->ptype) {
  //   case HIPE_DYNAMIC:
  //     This(dynamicpond)->addThreads(1);
  //     This(dynamicpond)->waitForTasks();
  //     break;
  //   case HIPE_BALANCE:
  //     break;
  //   case HIPE_STEADY:
  //     break;
  //   case ThreadPool:
  //     break;
  //   default:
  //     break;
  // }
  LOG__INFO(this->dynamicpond->getTasksRemain());
  LOG__INFO(this->dynamicpond->getExpectThreadNumb());
  // int target_thread_number = 3;
  // this->dynamicpond->adjustThreads(target_thread_number);
  this->dynamicpond->delThreads(2);
  this->dynamicpond->waitForThreads();
  LOG__INFO(
      "thread-numb now: " << this->dynamicpond->getRunningThreadNumb());  //
  // This(dynamicpond)->waitForThreads();
}

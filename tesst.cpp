#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "Slam_/slam.h"
#include "Slam_/tcpp.h"
#include "hipe.h"
#include "mqttclient/mqtt_client.hpp"
#include "stdio.h"
#include "system/global.hpp"
#include "util.h"

namespace Appled {
int dd;
}
int testprint() {
  // while (1)
  // {

  std::cout << "haha" << std::endl;
  /* code */
  // }
  return 1;
}
int test2() {
  // 动态线程池
  hipe::DynamicThreadPond pond(6);
  LOG__INFO("thread-numb = " << pond.getRunningThreadNumb());
  LOG__INFO("tasks-remain =" << pond.getTasksRemain());
  // // tasks block in task queue
  // pond.submit([]
  //             { LOG__INFO("task 1 done"); });
  // pond.submit([]
  //             { LOG__INFO("task 2 done"); });
  // pond.submit([]
  //             { LOG__INFO("task 3 done"); });
  // LOG__INFO("\nNow sleep for two seconds and then add one thread ...");
  // // 2s hipe::util::sleep_for_seconds(2);
  // // tasks above start run
  // pond.addThreads(1);
  // pond.waitForTasks();
  // pond.delThreads(2);
  // pond.waitForThreads();
  return 1;
}

int main(int argc, char *argv[]) {
  openlog(SYSLOG_OUTPUT_TAG, LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);
  LOG__NOTICE("=====> Start App <=====");  // 输出应用开始信息
  // // cloud::mqtt_client g_client; // 定义一个mqtt客户端
  // std::string host = "192.168.199.133";
  // std::string port = "1883";
  // std::string clientid = "clientid";
  // /*启动客户端*/
  // std::cout << "[MQTT Client] listen starting" << std::endl;
  // cloud::mqtt_client *g_client = new cloud::mqtt_client(host, port,
  // clientid);
  test2();

  LOG_WARN("=====> Exit App <=====");
  exit(0);
  return 0;
}

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "Slam_/slam.h"
#include "Slam_/tcpp.h"
#include "hipe.h"
#include "mqttclient/mqtt_client.hpp"
#include "stdio.h"
#include "system/global.hpp"
#include "thread/threadpool.hpp"
#include "util.h"

namespace Appled {
int dd;
}
int testprint() {
  // while (1)
  // {

  std::cout << "haha" << std::endl;
  /* code */
  // }
  return 1;
}
static int a = 1;
void test(void *b) { LOG__INFO(a); }
void test2() {
  // 动态线程池
  EmbedSystem::GTThreadPool gtpool(EmbedSystem::HIPE_DYNAMIC, 50, 200,
                                   2);  // 加入任务

  gtpool.AddThreadFunc(test, NULL);
  gtpool.AddThreadFunc(test, NULL);
  gtpool.AddThreadFunc(test, NULL);
  gtpool.AddThreadFunc(test, NULL);
  gtpool.AddThreadFunc(test, NULL);
  gtpool.AddThreadFunc(test, NULL);
}

int main(int argc, char *argv[]) {
  openlog(SYSLOG_OUTPUT_TAG, LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);
  LOG__NOTICE("=====> Start App <=====");  // 输出应用开始信息

  test2();

  LOG_WARN("=====> Exit App <=====");
  exit(0);
  return 0;
}

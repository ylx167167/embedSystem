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
#include "stdio.h"
#include "system/global.hpp"
// #include "thread/thread.hpp"
#include "mqttclient/mqtt_client.hpp"
#include "thread/thread.hpp"
#include "thread/threadpool.hpp"
#include "util.h"
using namespace EmbedSystem;

int main(int argc, char *argv[]) {
  openlog(SYSLOG_OUTPUT_TAG, LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);
  LOG__NOTICE("=====> Start App <=====");  // 输出应用开始信息

  // test6();
  using namespace EmbedSystem;
  GTThreadPool *gtpool = GTThreadPool::GetInstance();
  gtpool->Init(EmbedSystem::HIPE_DYNAMIC, 0, 200, 2);
  // cloud::mqtt_client g_client; // 定义一个mqtt客户端
  std::string host = "192.168.199.133";
  std::string port = "188";
  std::string port2 = "1883";
  std::string clientid = "clientid";
  /*启动客户端*/
  std::cout << "[MQTT Client] listen starting" << std::endl;
  PahoMQTT::Mqtt_Client *g_client =
      new PahoMQTT::Mqtt_Client(host, port, clientid);

  exit(0);
  return 0;
}

#ifndef __MQTT_CLIENT_TO_CLOUD_HPP__
#define __MQTT_CLIENT_TO_CLOUD_HPP__

#include <mqtt/async_client.h>  // mqtt库头文件

#include "mqtt_handler.hpp"
#include "system/global.hpp"
#include "thread/thread.hpp"

namespace PahoMQTT {

class Mqtt_Client;
//! 处理云信息的函数
using messageHandler = std::function<void(const std::string &, mqtt::topic *)>;
using namespace EmbedSystem;
class Mqtt_Client : public GTThread {
 public:
  Mqtt_Client() : EmbedSystem::GTThread(NULL){};
  Mqtt_Client(std::string &url, std::string &port, std::string &client);
  ~Mqtt_Client();
  void set_message_handler(messageHandler cb);
  int mqtt_send(const char *topic, const std::string &message);
  int mqtt_topic(const char *topic, messageHandler cb);

 private:
  // static constexpr const std::string BROKER_HOST;
  std::string BROKER_HOST;
  mqtt::async_client *cli_;
  // mqtt::async_client cli_2;
  std::map<const char *, mqtt::topic *> topicmap;  // topic的map
 protected:
  void run(_IN_ void *args);
};
}  // namespace PahoMQTT

#endif

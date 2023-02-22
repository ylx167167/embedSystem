/*
 *Filename         :Mqtt_Client.cpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/15 22:12:08 */
#include "mqtt_client.hpp"

#include <iostream>
#include <string>

#include "parse/cJSON.h"

using namespace std;
namespace PahoMQTT {
//  : EmbedSystem::GTThread(NULL)
Mqtt_Client::Mqtt_Client(std::string &url, std::string &port,
                         std::string &client)
    : GTThread(EmbedSystem::GTThreadPool::GetInstance()) {
  std::string broker = url + ":" + port;
  BROKER_HOST = broker;
  This(cli_) = new mqtt::async_client("192.168.199.133:1883", client,
                                      mqtt::create_options(MQTTVERSION_5));
  /* 设置失联回调*/
  mqtt_topic("CommandTopic", CommandTopic_handle);
  This(cli_)->set_connection_lost_handler([this](const string &info) {
    std::cout << "mqtt connection lost <" << info << ">, reconnting"
              << std::endl;
    This(cli_)->reconnect();
  });
  This(cli_)->set_connected_handler([this](const string &info) {
    std::cout << "mqtt connected <" << info << ">" << std::endl;
    this->mqtt_send("CommandTopic", "online");
  });
  //! Set connect options and do connect
  auto opts = mqtt::connect_options_builder()
                  .mqtt_version(MQTTVERSION_5)
                  .clean_start(true)
                  .finalize();
  This(cli_)->connect(opts);
  CreateThread();
  Start();
}

/**
 * @brief       Publish message to topic，发布消息给topic
 * @param[in]   message The message payload
 */
int Mqtt_Client::mqtt_send(const char *topic, const std::string &message) {
  for (const auto &iter : This(topicmap)) {
    if (strcmp(topic, iter.first) == 1) {
      iter.second->publish(message);
      return 0;
    }
  }
  return -1;
}

/*
 * @brief      : 设置主题
 * @param[in]  : topic主题名称
 */
int Mqtt_Client::mqtt_topic(const char *name, messageHandler callback) {
  if (This(cli_) == NULL) {
    std::cout << "客户端未启动" << std::endl;
  }
  for (const auto &iter : This(topicmap)) {
    if (strcmp(name, iter.first) == 1) {
      return -1;
    }
  }
  mqtt::topic *topic = new mqtt::topic(*(This(cli_)), name, 0);
  topicmap.emplace(name, topic);

  return 0;
}  // namespace cloud

void Mqtt_Client::run(_IN_ void *args) { LOG__INFO(BROKER_HOST); }

Mqtt_Client::~Mqtt_Client() {
  for (const auto &iter : This(topicmap)) {
    delete iter.second;
  }
  This(cli_)->disconnect();
  This(cli_)->disable_callbacks();
  delete This(cli_);
}

}  // namespace PahoMQTT
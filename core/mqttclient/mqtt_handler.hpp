#ifndef _MQTT_HANDLER
#define _MQTT_HANDLER
#include <mqtt/topic.h> // mqtt库头文件
//
void CommandTopic_handle(const std::string &data, mqtt::topic *topic_);

#endif
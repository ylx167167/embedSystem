#ifndef _MODEL_HPP_
#define _MODEL_HPP_

/* json 部件名称*/
#define JsonmqttInfo "mqttInfo"
typedef struct MQTT_INFO {
  char* host;
  char* port;
  char* clienId;
  char* defaultTp; /* 默认订阅主题*/
} MQTT_INFO;

#endif
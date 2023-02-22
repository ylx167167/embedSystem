#include <cjson/cJSON.h>
#include <mqtt/async_client.h>  // mqtt库头文件

#include <iostream>
#include <map>
#include <string>
using namespace std;
/**
 * @brief               Handler on message from cloud
 * @param[in]   data    The message payload from cloud
 */
void CommandTopic_handle(const string &data, mqtt::topic *topic_) {
  char *active = "false";
  std::cout << "received data is: " << data << std::endl;
  char *cmd;
  char *param;
  char *method;

  cJSON *json = NULL;
  cJSON *item = NULL;

  json = cJSON_Parse(data.c_str());
  item = cJSON_GetObjectItem(json, "cmd");
  cmd = item->valuestring;
  method = (cJSON_GetObjectItem(json, "method"))->valuestring;
  // 处理ping命令
  if (strcmp(cmd, "ping") == 0) {
    cJSON_AddItemToObject(json, "ping", cJSON_CreateString("pong"));
    printf("cJSON_AddItemToObject(json,cJSON_CreateString())\r\n");
  }

  // 处理message命令
  if (strcmp(cmd, "message") == 0) {
    if (strcmp(method, "get") == 0) {
      cJSON_AddItemToObject(json, "message", cJSON_CreateString("Are you ok?"));
    } else {
      // param = cJSON_Print(cJSON_GetObjectItem(json,"param"));
      cJSON_AddItemToObject(json, "result", cJSON_CreateString("set success."));
    }
  }
  // 处理randnum命令
  if (strcmp(cmd, "randnum") == 0) {
    cJSON_AddItemToObject(json, "randnum", cJSON_CreateString("520.1314"));
  }
  // 处理collect命令
  if (strcmp(cmd, "collect") == 0) {
    if (strcmp(method, "get") == 0) {
      cJSON_AddItemToObject(json, "collect", cJSON_CreateString(active));
    } else {
      active = (cJSON_GetObjectItem(json, "param"))->valuestring;
    }
  }
  char *out = cJSON_PrintUnformatted(json);
  std::cout << topic_->get_name() << std::endl;
  printf("return :%s \r\n", cJSON_PrintUnformatted(json));
  topic_->publish(out);
}
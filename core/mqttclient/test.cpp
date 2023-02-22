// #include <iostream>
// #include <string>
// #include <map>
// #include <cjson/cJSON.h>
// #include "mqtt_client.hpp"

// using namespace std;

// int main(int argc, char **argv)
// {
//     // cloud::mqtt_client g_client; // 定义一个mqtt客户端
//     std::string host = "192.168.199.133";
//     std::string port = "1883";
//     std::string clientid = "clientid";
//     /*启动客户端*/
//     std::cout << "[MQTT Client] listen starting" << std::endl;
//     cloud::mqtt_client *g_client = new cloud::mqtt_client(host, port, clientid);
//     /*订阅主题 */
//     // g_client->mqtt_topic("CommandTopic", on_cloud_message);
//     // g_client->mqtt_topic("DataTopic", on_cloud_message);

//     // 启动线程：

//     // while (1)
//     // {
//     //     std::cout << "运行中..." << std::endl;
//     //     this_thread::sleep_for(3);
//     //     g_client->mqtt_send("CommandTopic", "online");
//     //     // g_client.send("连接上了往MQTT的haojuhu主题发送该信息..."); // 确保与mqtt broker server建立连接之后再publish!!!
//     // }
// }
// // g++ main.cpp mqtt_client.cpp -lpaho-mqttpp3 -lpaho-mqtt3a -lcjson -lm
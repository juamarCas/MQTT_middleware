#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <thread>
#include "MQTTClient.h"
#include "GPIO.h"
#include "Config.h"

#define CONNECT_MQTT 0

void Callback(char* data);

/*packet that comes from microcontroller*/
#pragma pack(1)
typedef struct Payloads {
    std::uint16_t moist_1;
    std::uint16_t moist_2;
    float         temp;
    float         hum;
}Payload, * PPayload;
#pragma pack()

int main(int argv, const char ** argc){
    std::string _host, _port, _protocol, _topic;
    std::string _dbHost;
    Config _configFile("./Config.txt");
    
    try{
        _host     = _configFile.GetConfigValue("MQTT", "host");
        _port     = _configFile.GetConfigValue("MQTT", "port");
        _protocol = _configFile.GetConfigValue("MQTT", "protocol"); 
        _topic    = _configFile.GetConfigValue("MQTT", "topic");
        _dbHost   = _configFile.GetConfigValue("DB", "host");         
    }catch(const char * error){
        std::cout<<error<<std::endl;
    }
    
    const std::string msg      = "Hello from PI!";

#if CONNECT_MQTT
    MQTTClient mqttClient (_host, _port, _protocol);
    mqttClient.Connect("sssss");
    mqttClient.PublishToTopic(msg, topic);
    mqttClient.SubscribeToTopic(_topic);
    std::thread mqtt_client_thread(
        [&mqttClient](){
            while (1) {
                mqttClient.StartListening([](const std::string& payload) {
                    std::cout << payload << std::endl;
                });
            }
        }
    );
    mqtt_client_thread.detach();
     while(1){
       
    }
#endif
   

    return 0;
}

/*
-receives data
-checksum
-confirmation receive packet
-error with data
-save to queue
-send event
-finish
*/






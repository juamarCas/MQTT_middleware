#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <thread>
#include "MQTTClient.h"
#include "GPIO.h"
#include "Config.h"
#include "ConfigData.h"

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
    std::string _host, _port, _protocol, _topic, _clientID;
    std::string _dbHost;
    Config _configFile("./Config.txt");
    
    try{
        _host     = _configFile.GetConfigValue(configvalue::MQTT_group, configvalue::host);
        _port     = _configFile.GetConfigValue(configvalue::MQTT_group, configvalue::port);
        _protocol = _configFile.GetConfigValue(configvalue::MQTT_group, configvalue::protocol);
        _topic    = _configFile.GetConfigValue(configvalue::MQTT_group, configvalue::topic);
        _clientID = _configFile.GetConfigValue(configvalue::MQTT_group, configvalue::clientID);  
        _dbHost   = _configFile.GetConfigValue(configvalue::DB_group, configvalue::host);
    }catch(const char * error){
        std::cout<<error<<std::endl;
        return 1;
    }
    
    const std::string msg = "Hello from PI!";

#if CONNECT_MQTT
    MQTTClient mqttClient (_host, _port, _protocol);
    mqttClient.Connect(_clientID);
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






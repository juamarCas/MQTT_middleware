#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <thread>
#include "MQTTClient.h"
#include "Serial.h"
#include "GPIO.h"

Serial serial("/dev/serial0", false, false, false, Serial::B8, Serial::BA9600);


int main(int argv, const char ** argc){
   
    const std::string host     = "esdata.ml";
    const std::string port     = "1883";
    const std::string protocol = "tcp";

    
    const std::string msg      = "Hello from PI!";
    const std::string topic    = "testtopic";

    MQTTClient mqttClient (host, port, protocol);
    mqttClient.Connect("sssss");
    mqttClient.PublishToTopic(msg, topic);
    mqttClient.SubscribeToTopic(topic);

    std::thread mqtt_client_thread(
        [&mqttClient](){
            mqttClient.StartListening([](const std::string& payload){
                std::cout<<payload<<std::endl;
            });
        }
    );

    mqtt_client_thread.detach();

    GPIO _gpio("23", "out");
    _gpio.GPIO_Write("1");
    _gpio.GPIO_Toggle();
  

    while(1){

    }

    return 0;
}



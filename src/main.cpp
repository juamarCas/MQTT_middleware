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
    const std::string topic    = "Room1/RPI";

    MQTTClient mqttClient (host, port, protocol);
    mqttClient.Connect("sssss");
    mqttClient.PublishToTopic(msg, topic);
    mqttClient.SubscribeToTopic(topic);

    GPIO _yellow("23", "out");
    GPIO _green("24", "out");
    GPIO _red("25", "out");


    std::thread mqtt_client_thread(
        [&mqttClient, &_red, &_yellow, &_green](){
            mqttClient.StartListening([&_red, &_yellow, &_green](const std::string& payload){
                std::cout<<payload<<std::endl;
                if (payload == "LED1") {
                    _yellow.GPIO_Toggle();
                }
                else if (payload == "LED2") {
                    _green.GPIO_Toggle();
                }
                else if (payload == "LED3") {
                    _red.GPIO_Toggle();
                }
            });
        }
    );

    mqtt_client_thread.detach();

    
  

    while(1){

    }

    return 0;
}



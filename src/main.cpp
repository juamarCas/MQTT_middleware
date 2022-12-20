#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <thread>
#include <nlohmann/json.hpp>
#include <ctime>
#include <sstream>
#include "MQTTClient.h"
#include "GPIO.h"
#include "Config.h"
#include "ConfigData.h"
#include "SQLiteManager.h"
#include "ThreadQueue.h"
#include "Queries.h"

#define CONNECT_MQTT 1
#define INSERT_DATA_DB 1

typedef struct Payloads {
    std::uint8_t moist;
    float         temp;
    float         hum;
    std::uint16_t soil_temp;
    std::uint8_t light;
}Payload;


const std::uint16_t id_envTemp      = 100;
const std::uint16_t id_envHum       = 101;
const std::uint16_t id_soilMoisture = 102;
const std::uint16_t id_soilTemp     = 103;
const std::uint16_t id_lightPerc    = 104;



int main(int argv, const char ** argc){
    std::string _host, _port, _protocol, _topic, _clientID;
    std::string _dbPath;
    Config _configFile("./Config.txt");
    try{
        _host     = _configFile.GetConfigValue(configdata::MQTT_group, configdata::host);
        _port     = _configFile.GetConfigValue(configdata::MQTT_group, configdata::port);
        _protocol = _configFile.GetConfigValue(configdata::MQTT_group, configdata::protocol);
        _topic    = _configFile.GetConfigValue(configdata::MQTT_group, configdata::topic);
        _clientID = _configFile.GetConfigValue(configdata::MQTT_group, configdata::clientID);  
        _dbPath   = _configFile.GetConfigValue(configdata::DB_group,   configdata::sqliteFile);
    }catch(const char * error){
        std::cout<<error<<std::endl;
        return 1;
    }

  
    
    SQLiteManager sqlite(_dbPath.c_str());
    if(!sqlite.OpenDatabase()){
        std::cout<<"Error opening database!"<<std::endl;
        return 1;
    }
    std::cout<<"Database opened succesfully!"<<std::endl;

    ThreadQueue<std::string> _tQueue(10);

#if CONNECT_MQTT
    MQTTClient mqttClient (_host, _port, _protocol);
    mqttClient.Connect(_clientID);
    mqttClient.SubscribeToTopic(_topic);

    std::thread mqtt_client_thread(
        [&mqttClient, &_tQueue](){        
                mqttClient.StartListening([&_tQueue](const char * payload) {
                    std::string _jsonRaw(payload);
                    _tQueue.SendValue(_jsonRaw);
                });           
        }
    );


    mqtt_client_thread.detach();

    Payload _p;
    std::string _dataReceptor;
    while(1){
       if(_tQueue.GetValue(_dataReceptor)){
            std::cout<<_dataReceptor<<std::endl;
            auto jsonParsed = nlohmann::json::parse(_dataReceptor);
            
            int moist = jsonParsed["moist"];
            int light = jsonParsed["light"];
            _p.temp  = jsonParsed["envTemp"];
            _p.hum   = jsonParsed["envHum"];
#if INSERT_DATA_DB
            query::SaveDataToRegister(id_envTemp, _p.temp, &sqlite);
            query::SaveDataToRegister(id_envHum, _p.hum, &sqlite);
            query::SaveDataToRegister(id_lightPerc, light, &sqlite);
            query::SaveDataToRegister(id_soilMoisture, moist, &sqlite);
#endif
       }
    }
#endif
   

    return 0;
}








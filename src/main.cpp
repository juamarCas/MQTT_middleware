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

#define CONNECT_MQTT 1

void Callback(char* data);

template <typename T>
bool AssembleQuery(std::uint16_t _sensorID, T _value, SQLiteManager * _sql);


/*packet that comes from microcontroller*/
#pragma pack(1)
typedef struct Payloads {
    std::uint8_t moist;
    float         temp;
    float         hum;
    std::uint16_t soil_temp;
    std::uint8_t light;
}Payload, * PPayload;
#pragma pack()

const std::uint16_t id_envTemp      = 100;
const std::uint16_t id_envHum       = 101;
const std::uint16_t id_soilMoisture = 102;
const std::uint16_t id_soilTemp     = 103;
const std::uint16_t id_lightPerc    = 104;

int callback(void *NotUsed, int argc, char **argv, char **azColName) {  
   return 0;
}

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
    // char * sql = "INSERT INTO Type(unit, name)" \
    //        "VALUES('%', 'Humidity');"
    
    const std::string msg = "Hello from PI!";

    ThreadQueue<std::string> _tQueue(10);

#if CONNECT_MQTT
    MQTTClient mqttClient (_host, _port, _protocol);
    mqttClient.Connect(_clientID);
    mqttClient.PublishToTopic(msg, _topic);
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
            
            _p.moist = jsonParsed["moist"];
            _p.light = jsonParsed["light"];
            _p.temp  = jsonParsed["envTemp"];
            _p.hum   = jsonParsed["envHum"];

            AssembleQuery(id_envTemp, _p.temp, &sqlite);
            AssembleQuery(id_envHum, _p.hum, &sqlite);
            AssembleQuery(id_lightPerc, _p.light, &sqlite);
            AssembleQuery(id_soilMoisture, _p.moist, &sqlite);
       }
    }
#endif
   

    return 0;
}

template <typename T>
bool AssembleQuery(std::uint16_t _sensorID, T _value, SQLiteManager * _sql){
    time_t now = time(0);
    char * date_time = ctime(&now);

    std::string date(date_time);
    std::size_t pos = date.find('\n');

    while(pos != std::string::npos){
        date.erase(pos, 1);
        pos = date.find('\n');
    }
    std::stringstream ss;

    ss << "INSERT INTO Register(date, value, sensor_id)\nVALUES("<<"\'"<<date <<"\'"<<","<<_value<<","<<_sensorID<<");";
    std::string q = ss.str();

    std::cout<<"Query: "<<std::endl;
    std::cout<<q<<std::endl;
    char * query = q.c_str();
    if(_sql->SendSQLSentence(query, callback)){
        std::cout<<"Value saved!"<<std::endl;
    }
    
}







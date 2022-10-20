#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <thread>
#include "MQTTClient.h"
#include "GPIO.h"
#include "Config.h"
#include "ConfigData.h"
#include <sqlite3.h>

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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {  
   return 0;
}

int main(int argv, const char ** argc){
    std::string _host, _port, _protocol, _topic, _clientID;
    std::string _dbPath;
    Config _configFile("./Config.txt");
    sqlite3 * db;
    int rc;

    char *zErrMsg = 0;
    char * sql;

    
    
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
    rc = sqlite3_open(_dbPath.c_str(), &db);

    if(rc){
        std::cout<<"Can\'t open DB!"<<std::endl;
        return 1;
    }

    std::cout<<"Connected succesfully!"<<std::endl;

    sql = "INSERT INTO Type(unit, name)" \
          "VALUES('c', 'Temperature');";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      std::cout<<zErrMsg<<std::endl;
      sqlite3_free(zErrMsg);
   } else {
      std::cout<<"Added data succesfully!"<<std::endl;
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






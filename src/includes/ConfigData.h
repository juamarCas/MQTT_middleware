#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <iostream>

namespace configvalue{
    const std::string MQTT_group = "MQTT";
    const std::string DB_group   = "DB";
    const std::string host       = "host";
    const std::string port       = "port";
    const std::string protocol   = "protocol";
    const std::string topic      = "topic";
    const std::string clientID   = "clientID";
}

#endif
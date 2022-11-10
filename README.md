# MQTT Middleware
Principal node that receives data from other devices and publish them with MQTT protocol and saves it in a SQLite db.

## Dependencies
* sqlite3 library: [SQlite C++](https://www.sqlite.org/download.html)
```
sudo apt-get install libsqlite3-dev
```
* pahoo MQTT library: [Pahoo MQTT C++](https://github.com/eclipse/paho.mqtt.cpp)
* JSON for modern C++: [JSON library](https://github.com/nlohmann/json)
## Config file
the config file is a txt contains all the information about the connections to the MQTT broker and the database</br>

This is an example of how is structured the configfile:
</br>
Example 1: </br>
```
!MQTT
-host = localhost
-port = 1883
-protocol = tcp
-topic  = test
-clientID = ssss

!DB
-file = /path/to/my/sqlite/dbfile.db
```
The file is read by the _Config_ class present in the _Config.h_ class.</br>

For more information about this config file in the following link: https://github.com/juamarCas/ConfigFile

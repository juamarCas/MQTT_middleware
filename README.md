# MQTT Middleware
Principal node that receives data from other devices and publish them with MQTT protocol and saves it in a SQLite db.

## Dependencies
* sqlite3 library: [SQlite C++](https://www.sqlite.org/download.html)
```
sudo apt-get install libsqlite3-dev
```
* pahoo MQTT library: [Pahoo MQTT C++](https://github.com/eclipse/paho.mqtt.cpp)

## Config file
the config file is a txt file used to stablish parameters and configuration into the software without the needing of recompiling it whenever these configurations are changed.</br>

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
Example 2: </br>
```
!MQTT
-host = localhost
-port = 1883
-protocol = tcp
-topic = test
-clientID = ssss
-user = exampleUser
-password = examplePassword

!MYSQL
-host = localhost
-user = user
-password = password
```

The file is separeted in two type of information:</br>
* Groups: are denoted with a "!" before its name, is useful to split parameters and makes posible to have </br>
parameters with the same name in different groups. 
* Parameters: are denoted with a "-" sufix, is the name the program will look for and get its value after the "=" simbol

NOTE: make a repository for the config file and a Wiki

The file is read by the _Config_ class present in the _Config.h_ class.

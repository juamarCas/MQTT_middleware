
INCLUDE   = ./includes
LIB       = 
LINK      = -lpaho-mqttpp3 -lpaho-mqtt3as -pthread
FLAGS     = -g -Wall

all: main.o 
	g++ $(FLAGS) -o app main.o MQTTClient.o Serial.o GPIO.o Config.o SQLiteManager.o -I$(INCLUDE) $(LINK) -lssl -l sqlite3

main.o: MQTTClient.o Serial.o GPIO.o Config.o SQLiteManager.o
	g++ -c main.cpp -I$(INCLUDE) -I./ -lssl -fpermissive -pthread

GPIO.o:
	g++ -c GPIO.cpp -I$(INCLUDE)

MQTTClient.o:
	g++ -c MQTTClient.cpp -I$(INCLUDE) $(LINK) -lssl

Serial.o:
	g++ -c Serial.cpp -I$(INCLUDE)

Config.o:
	g++ -c Config.cpp -I$(INCLUDE)

SQLiteManager.o:
	g++ -c SQLiteManager.cpp -I$(INCLUDE) -l sqlite3

clean: 
	rm -f ./app *.o
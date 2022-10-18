
INCLUDE   = ./includes
LIB       = 
LINK      = -lpaho-mqttpp3 -lpaho-mqtt3as -pthread 
FLAGS     = -g -Wall

all: main.o 
	g++ $(FLAGS) -o app main.o MQTTClient.o Serial.o GPIO.o Config.o -I$(INCLUDE) $(LINK) -lssl 

main.o: MQTTClient.o Serial.o GPIO.o Config.o
	g++ -c main.cpp -I$(INCLUDE) -lssl -fpermissive -pthread

GPIO.o:
	g++ -c GPIO.cpp -I$(INCLUDE)

MQTTClient.o:
	g++ -c MQTTClient.cpp -I$(INCLUDE) $(LINK) -lssl

Serial.o:
	g++ -c Serial.cpp -I$(INCLUDE)

Config.o:
	g++ -c Config.cpp -I$(INCLUDE)
		
clean: 
	rm -f ./app *.o
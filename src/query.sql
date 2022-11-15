--Type of sensor: temperature, humidity, ect...
CREATE TABLE IF NOT EXISTS "Type" (
	"id"	INTEGER NOT NULL UNIQUE,
	"unit"	TEXT NOT NULL,
	"name"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("id" AUTOINCREMENT)
);


--Sensor: actual sensor registration
--Each node has multiple sensor, example: an node measures
--temperature, humidity and light%, each has to have a respective ID
--it could be something like 100 for temp, 101 for hum, 102 for % light
--the first number indicating the node number for example
CREATE TABLE IF NOT EXISTS "Sensor" (
	"id"	INTEGER NOT NULL UNIQUE,
	"name"	TEXT NOT NULL,
	"typeId"	INTEGER NOT NULL,
	FOREIGN KEY("typeId") REFERENCES "Type"("id")
);

--register of data
CREATE TABLE IF NOT EXISTS "Register"(
	"id" INTEGER NOT NULL UNIQUE,
	"date" TEXT NOT NULL,
	"sensor_id" INTEGER NOT NULL,
	FOREIGN KEY("sensor_id") REFERENCES "Sensor"("id"),
	PRIMARY KEY("id" AUTOINCREMENT)
);

--data insertion

INSERT INTO Type(unit, name)
VALUES("C", "temperature");

INSERT INTO Type(unit, name)
VALUES("%H", "humidity");

INSERT INTO Type(unit, name)
VALUES("%", "Light percentage");

--data insertion to sensor
INSERT INTO Sensor(id, name, typeId)
VALUES(100, "Environment temperature", 1);

INSERT INTO Sensor(id, name, typeId)
VALUES(101, "Environment humidity", 2);

INSERT INTO Sensor(id, name, typeId)
VALUES(102, "Soil moisture", 2);

INSERT INTO Sensor(id, name, typeId)
VALUES(103, "Soil temperature", 1);

INSERT INTO Sensor(id, name, typeId)
VALUES(104, "Light percentage", 3);
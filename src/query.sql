--Type of sensor: temperature, humidity, ect...
CREATE TABLE "Type" (
	"id"	INTEGER NOT NULL UNIQUE,
	"unit"	TEXT NOT NULL,
	"name"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("id" AUTOINCREMENT)
);


--Sensor: actual sensor registration
CREATE TABLE "Sensor" (
	"id"	INTEGER NOT NULL UNIQUE,
	"name"	TEXT NOT NULL,
	"typeId"	INTEGER NOT NULL,
	FOREIGN KEY("typeId") REFERENCES "Type"("id"),
	PRIMARY KEY("id")
);

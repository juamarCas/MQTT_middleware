--Type of sensor: temperature, humidity, ect...
CREATE TABLE IF NOT EXISTS "Type" (
	"id"	INTEGER NOT NULL UNIQUE,
	"unit"	TEXT NOT NULL,
	"name"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("id" AUTOINCREMENT)
);


--Sensor: actual sensor registration
CREATE TABLE IF NOT EXISTS "Sensor" (
	"id"	INTEGER NOT NULL UNIQUE,
	"name"	TEXT NOT NULL,
	"typeId"	INTEGER NOT NULL,
	FOREIGN KEY("typeId") REFERENCES "Type"("id"),
	PRIMARY KEY("id" AUTOINCREMENT)
);

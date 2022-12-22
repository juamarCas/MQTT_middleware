#ifndef QUERIES_H
#define QUERIES_H
#include "SQLiteManager.h"
#include <iostream>

/*
This file intends to stablish all the functions that makes queries to the database
*/

namespace query
{
    int callback(void *NotUsed, int argc, char **argv, char **azColName) {  
        return 0;
    }


    template <typename T>
    bool SaveDataToRegister(std::uint16_t _sensorID, T _value, SQLiteManager * _sql){
        time_t now = time(0);
        char * date_time = ctime(&now);

        std::string date(date_time);
        std::size_t pos = date.find('\n');
        date.erase(pos, 1);
    // while(pos != std::string::npos){
    //     date.erase(pos, 1);
    //     pos = date.find('\n');
    // }
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
}


#endif
#ifndef SQLITE_MANAGER_H
#define SQLITE_MANAGER_H
#include <iostream>
#include <sqlite3.h>
class SQLiteManager{
    private:
    char * _path;
    sqlite3 * db;
    int rc;
    char *zErrMsg = 0;

    bool _isDatabaseOpen = false;

    public: 

    /**
    *
    *
    */
    SQLiteManager(char * path): _path(path){}

    ~SQLiteManager(){
        if(!_isDatabaseOpen) return;
        CloseDatabase();
    }

    /**
    *
    *
    */
    bool OpenDatabase();

    /**
    *
    *
    */
    bool CloseDatabase();
    
    /*
    *
    *
    */
    bool SendSQLSentence(char * sqlSentence, int(*callback)(void *, int, char **, char **));
};

#endif
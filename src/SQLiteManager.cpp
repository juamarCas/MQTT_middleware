#include "SQLiteManager.h"

bool SQLiteManager::OpenDatabase(){
    rc = sqlite3_open(_path, &db);  

    _isDatabaseOpen = false;
    if(rc == SQLITE_OK){
        _isDatabaseOpen = true;
    }
    
    return rc == SQLITE_OK;
}

bool SQLiteManager::SendSQLSentence(char * sqlSentence, int(*callback)(void *, int, char **, char **)){

    rc = sqlite3_exec(db, sqlSentence, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      std::cout<<zErrMsg<<std::endl;
      sqlite3_free(zErrMsg);
      return false;
   } else {
      return true;
   }
}


void SQLiteManager::CloseDatabase(){
    if(_isDatabaseOpen){
        sqlite3_close(db);
    }
}
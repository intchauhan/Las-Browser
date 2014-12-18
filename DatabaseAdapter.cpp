#include "DatabaseAdapter.h"
#include <QDebug>
#include "Globals.h"

static QSqlDatabase databasedb;

DatabaseAdapter::DatabaseAdapter()
{
    databasedb = QSqlDatabase::addDatabase("QSQLITE");

}

void DatabaseAdapter::openDatabase(){
    databasedb.setDatabaseName(gLocalDbLocation);
    databasedb.close();
    if(databasedb.open()){
        qDebug()<<"Database opened by DatabaseAdapter::openDatabase() function";
    }else{
        qDebug()<<"Database FAILED to be opened by DatabaseAdapter::openDatabase() function";
    }
}

void DatabaseAdapter::closeDatabase(){
    if(databasedb.isOpen()){
        databasedb.close();
        qDebug()<<"Database closed by DatabaseAdapter::closeDatabase() function";
    }
}

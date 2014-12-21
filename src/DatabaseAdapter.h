#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDatabase>

class DatabaseAdapter
{
public:
    DatabaseAdapter();
    void openDatabase();
    void closeDatabase();
    QSqlDatabase databasedb;
};

#endif // DATABASEADAPTER_H

#ifndef DATABASECOPIER_H
#define DATABASECOPIER_H
#include <QString>

class DatabaseCopier
{
public:
    DatabaseCopier();
    void DatabaseCopier::copyToLocal();
    void DatabaseCopier::setLocalDatabaseLocation();
    void DatabaseCopier::pushToDevice();
    void DatabaseCopier::backupDb(QString location);
};

#endif // DATABASECOPIER_H

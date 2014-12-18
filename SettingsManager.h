#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include <QString>

class SettingsManager
{
public:
    SettingsManager();
    bool loadOpenDatabaseShowAtStartup();
    void setOpenDatabaseShowAtStartup(bool toSet);

    QString loadSetSDKLocationLocation();
    void setSetSDKLocationLocation(QString location);
};

#endif // SETTINGSMANAGER_H

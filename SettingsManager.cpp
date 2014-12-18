#include "SettingsManager.h"
#include <QString>
#include <QSettings>

SettingsManager::SettingsManager()
{
}


bool SettingsManager::loadOpenDatabaseShowAtStartup(){
    QSettings settings("intchauhan","lasBrowser");
    settings.beginGroup("OpenDatabaseUi");
    bool toReturn = settings.value("showAtStartup",true).toBool();
    settings.endGroup();
    return toReturn;
}

void SettingsManager::setOpenDatabaseShowAtStartup(bool toSet){
    QSettings settings("intchauhan","lasBrowser");
    settings.beginGroup("OpenDatabaseUi");
    settings.setValue("showAtStartup",toSet);
    settings.endGroup();
    return;
}

void SettingsManager::setSetSDKLocationLocation(QString location){
    QSettings settings("intchauhan","lasBrowser");
    settings.beginGroup("SetSDKLocation");
    settings.setValue("location",location);
    settings.endGroup();
}

QString SettingsManager::loadSetSDKLocationLocation(){
    QSettings settings("intchauhan","lasBrowser");
    settings.beginGroup("SetSDKLocation");
    QString toReturn = settings.value("location","").toString();
    settings.endGroup();
    return toReturn;
}

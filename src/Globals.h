#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QSqlDatabase>
#include <QProcess>
#include "OpenDatabaseUi.h"

extern QString gVersionNumber;

extern QObject *gMainWindowPointer;
extern OpenDatabaseUi *gOpenDatabaseUiPointer; 

extern QString gSdkLocation;
extern QString gDeviceId;
extern QString gPackageName;
extern QString gDatabaseName;
extern QString gLocalDbLocation;
extern QString gRemoteDbLocation;
extern QString gDbAndroidDir; //full location of folder in the android device where db is stored
extern QString gDbLocalFullLocation;

extern bool isDatabaseOpenedInSoftware;
extern QString gTypeOfDatabaseOpened;

extern QProcess process;

#endif // GLOBALS_H

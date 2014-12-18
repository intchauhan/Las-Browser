#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T15:16:46
#
#-------------------------------------------------

QT       += core gui sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqliteExp
TEMPLATE = app
RC_FILE = lasbrowser.rc


SOURCES += main.cpp\
    SetPackageName.cpp \
    SetDatabaseName.cpp \
    CreateTable.cpp \
    SetSDKLocation.cpp \
    SetDeviceNumber.cpp \
    InsertNewRecord.cpp \
    CopyDb.cpp \
    ExecuteSQL.cpp \
    AdbShellProcessor.cpp \
    DataCopier.cpp \
    DatabaseCopier.cpp \
    MainWindow.cpp \
    BackupToLocal.cpp \
    CreateTableFromOptions.cpp \
    DatabaseAdapter.cpp \
    DeleteTableFromOptions.cpp \
    About.cpp \
    CheckForUpdates.cpp \
    OpenDatabaseUi.cpp \
    CreateNewDatabase.cpp \
    SettingsManager.cpp \
    UpdateManager.cpp

HEADERS  += \
    SetPackageName.h \
    SetDatabaseName.h \
    CreateTable.h \
    SetSDKLocation.h \
    SetDeviceNumber.h \
    InsertNewRecord.h \
    CopyDb.h \
    ExecuteSQL.h \
    AdbShellProcessor.h \
    Globals.h \
    todo.h \
    DatabaseCopier.h \
    MainWindow.h \
    BackupToLocal.h \
    CreateTableFromOptions.h \
    DatabaseAdapter.h \
    DeleteTableFromOptions.h \
    About.h \
    CheckForUpdates.h \
    OpenDatabaseUi.h \
    CreateNewDatabase.h \
    SettingsManager.h \
    UpdateManager.h

FORMS    += \
    SetPackageName.ui \
    SetDatabaseName.ui \
    CreateTable.ui \
    SetSDKLocation.ui \
    SetDeviceNumber.ui \
    InsertNewRecord.ui \
    MainWindow.ui \
    BackupToLocal.ui \
    CreateTableFromOptions.ui \
    DeleteTableFromOptions.ui \
    About.ui \
    CheckForUpdates.ui \
    OpenDatabaseUi.ui \
    CreateNewDatabase.ui

RESOURCES += \
    icons.qrc \
    files.qrc

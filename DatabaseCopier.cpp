#include "DatabaseCopier.h"
#include "Globals.h"
#include <QDebug>

DatabaseCopier::DatabaseCopier()
{
QString localDbLo = "";
}


void DatabaseCopier::copyToLocal(){
    QString fullLocation = gDbAndroidDir + gDatabaseName;
    process.setWorkingDirectory(gSdkLocation);
    //deep refresh
    process.start("cmd /c adb kill-server");
    process.waitForFinished();
    process.start("cmd /c adb start");
    process.waitForFinished();
    QString command = "cmd /c adb -s "+ gDeviceId + " shell \"mkdir //sdcard//androidsqliteExplorer//";
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;

    command = "cmd /c adb -s "+ gDeviceId + " shell \"mkdir //sdcard//androidsqliteExplorer//" + gPackageName + "//";
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;

    //copying db to device's sdcard
    command = "cmd /c adb -s " + gDeviceId + " shell \"run-as " + gPackageName + " cat " + gRemoteDbLocation + " > //sdcard//androidsqliteExplorer//" + gPackageName + "//" + gDatabaseName;

    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;


    //changing permissions of pkg dir inside androidSqliteExplorer
    command = "cmd /c adb -s " + gDeviceId + " shell \"chmod 777 //sdcard//androidsqliteExplorer//" + gPackageName;
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;

    //changing permissions of the database file itself residing at sdcard
    command = "cmd /c adb -s " + gDeviceId + " shell \"chmod 777 //sdcard//androidsqliteExplorer//" + gPackageName + "//" + gDatabaseName;
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;


    //pulling the database from sdcard to computer's local storage
    command = "cmd /c adb -s " + gDeviceId + " pull //sdcard//androidsqliteExplorer//" +gPackageName + "//" + gDatabaseName + " C://androidsqliteExplorer//" + gPackageName + "//"+gDatabaseName;
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;
}

void DatabaseCopier::pushToDevice(){
    //deep refresh
    process.start("cmd /c adb kill-server");
    process.waitForFinished();
    process.start("cmd /c adb start");
    process.waitForFinished();
    QString command = "cmd /c adb -s "+ gDeviceId + " shell \"mkdir //sdcard//androidsqliteExplorer//";
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;


    command = "cmd /c adb -s " + gDeviceId + " push C://androidsqliteExplorer//" + gPackageName + "//"+ gDatabaseName + " " + gRemoteDbLocation;
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;
}

void DatabaseCopier::setLocalDatabaseLocation(){

}

void DatabaseCopier::backupDb(QString location){
    qDebug()<<"func exec void DatabaseCopier::backupDb(QString location)";
    qDebug()<<"location: "<<location;
	
    QString command = "cmd /c adb -s " + gDeviceId + " pull //sdcard//androidsqliteExplorer//" +gPackageName + "//" + gDatabaseName + " " +location;
    qDebug()<<"Command to exec: "<<command;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;
}

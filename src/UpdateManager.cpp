#include "UpdateManager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

//FILE NOT IN USE

UpdateManager::UpdateManager(QObject *checkForUpdates)
{
    checkForUpdatesObject = checkForUpdates;
    thisObject = this;
}

void UpdateManager::run(){
    exec();
    QObject *checkForUpdatesObject1 = checkForUpdatesObject;
    qDebug()<<"func exec UpdateManager::run()";
    QNetworkAccessManager *qnam = new QNetworkAccessManager(this);
    connect(qnam,SIGNAL(finished(QNetworkReply*)),this,SLOT(readRead(QNetworkReply*)),Qt::DirectConnection);
    qnam->get(QNetworkRequest(QUrl("http://intchauhan.com")));
}

void UpdateManager::readRead(QNetworkReply *reply){
    qDebug()<<"func exec void UpdateManager::readRead(QNetworkReply *reply)";
    QByteArray myData;
    myData = reply->readAll();
    qDebug()<<myData;
    QString data = myData;
    //emit updateManagerFinishedSignal(data);
}

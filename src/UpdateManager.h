#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H
#include <QThread>
#include <qDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>

//FILE NOT IN USE

class UpdateManager : public QThread
{
public:
    UpdateManager(QObject *checkForUpdates);
    void run();
    QObject *checkForUpdatesObject;
    QObject *thisObject;

signals:
    void dataReadyRead(QByteArray);
    void updateManagerFinishedSignal(QString data);

public slots:
    void readRead(QNetworkReply*);

};

#endif // UPDATEMANAGER_H

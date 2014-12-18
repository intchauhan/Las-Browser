#ifndef CHECKFORUPDATES_H
#define CHECKFORUPDATES_H

#include <QDialog>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class CheckForUpdates;
}

class CheckForUpdates : public QDialog
{
    Q_OBJECT

public:
    explicit CheckForUpdates(QWidget *parent = 0);
    ~CheckForUpdates();
    QString updateUrl;

signals:
    void dataReadyRead(QByteArray);

public slots:
    void readRead(QNetworkReply *reply);

private slots:
    void on_btnCancel_clicked();

    void on_btnDownload_clicked();

private:
    Ui::CheckForUpdates *ui;
    QNetworkAccessManager *qnam = new QNetworkAccessManager(this);

};

#endif // CHECKFORUPDATES_H

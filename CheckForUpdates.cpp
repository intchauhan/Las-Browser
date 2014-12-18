#include "CheckForUpdates.h"
#include "ui_CheckForUpdates.h"
#include "Globals.h"
#include <QStringList>
#include <QDesktopServices>

CheckForUpdates::CheckForUpdates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckForUpdates)
{
    ui->setupUi(this);
         this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
         setWindowTitle("Check For Updates");
         ui->lbStatus->setVisible(false);
         ui->btnDownload->setVisible(false);
         connect(qnam,SIGNAL(finished(QNetworkReply*)),this,SLOT(readRead(QNetworkReply*)));
         updateUrl = "";

         QString urlString;
         urlString = "http://lasbrowser.intchauhan.com/update.php?version=" + gVersionNumber;
         //urlString = "http://localhost/lasbrowser/update.php?version=" + gVersionNumber; //uncomment for debugging
         qDebug()<<urlString;
         qnam->get(QNetworkRequest(QUrl(urlString)));
}

CheckForUpdates::~CheckForUpdates()
{
    delete ui;
}

void CheckForUpdates::readRead(QNetworkReply *reply)
{
    ui->lbChecking->setVisible(false);
    ui->progressBar->setVisible(false);
    QByteArray replyData;
    replyData = reply->readAll();
    QString replyString = replyData;
    qDebug()<<replyString;

    QStringList replyStringList = replyString.split(" ");

    ui->lbStatus->setVisible(true);
    ui->btnCancel->setText("OK");
    if(replyStringList.at(0) != "yes"){
        qDebug()<<"Connection Problem. Please check manually";
        ui->lbStatus->setText("Connection Error. Please check manually: http://lasbrowser.intchauhan.com/#download");
    }else{
        if(replyStringList.at(1) == "error"){
            ui->lbStatus->setText("Server Error. Please check manually: http://lasbrowser.intchauhan.com/#download");
        }

        if(replyStringList.at(1) == "current"){
            ui->lbStatus->setText("You have the latest version of Las Browser.");
        }else{
            ui->lbStatus->setText("Version" +replyStringList.at(2) + " available, it's highly recommended that you download this update.");
            ui->btnDownload->setVisible(true);
            updateUrl = replyStringList.at(3);
        }
    }
}

void CheckForUpdates::on_btnCancel_clicked()
{
    close();
}

void CheckForUpdates::on_btnDownload_clicked()
{
    qDebug()<<"func exec void CheckForUpdates::on_btnDownload_clicked()";
    QDesktopServices::openUrl(QUrl(updateUrl));

}

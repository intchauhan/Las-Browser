#include "BackupToLocal.h"
#include "ui_BackupToLocal.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include "Globals.h"
#include "DatabaseCopier.h"

BackupToLocal::BackupToLocal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackupToLocal)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Las Browser");
    ui->btnBackup->setEnabled(false);
}

BackupToLocal::~BackupToLocal()
{
    delete ui;
}

void BackupToLocal::on_btnBrowse_clicked()
{
    location = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                     "/home",
                                                                     QFileDialog::ShowDirsOnly
                                                                     | QFileDialog::DontResolveSymlinks);
    if(location.trimmed()!=""){
        if(location.at(location.size()-1) == '/' || location.at(location.size()-1) == '\\') {
    ui->leDestination->setText(location + gDatabaseName);
        }else{
            ui->leDestination->setText(location + "/" + gDatabaseName);
        }
    }
}

void BackupToLocal::on_btnBackup_clicked()
{
    QString locationText = ui->leDestination->text();
    QFile file(locationText);
    if(file.exists()){
        qDebug()<<"file already exists";//ask for overwrite
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        QString messageString = "File Already Exists, Overwrite?";
        message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        message.setInformativeText(messageString);
        //code to increse default size of the messagebox
        QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)message.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        int ret = message.exec();
        switch (ret) {
          case QMessageBox::Cancel:
              qDebug()<<"Cancel was clicked";
              return;
              break;
    }
    }
    DatabaseCopier *databaseCopier = new DatabaseCopier();
    databaseCopier->backupDb(locationText);

    close();
}

void BackupToLocal::on_leDestination_textChanged(const QString &arg1)
{
    if(ui->leDestination->text().trimmed()!=""){
        ui->btnBackup->setEnabled(true);
    }else{
        ui->btnBackup->setEnabled(false);
    }
}

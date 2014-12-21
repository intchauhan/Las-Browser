#include "SetSDKLocation.h"
#include "ui_SetSDKLocation.h"
#include "Globals.h"
#include "SetDeviceNumber.h"
#include "SettingsManager.h"

QString gSdkLocation;
QString gDbCopyLocation = "test.db";

SetSDKLocation::SetSDKLocation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSDKLocation)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Set Android SDK Location");
    ui->cbRemember->setChecked(true);

    SettingsManager *settingsManager = new SettingsManager();
    ui->leLocation->setText(settingsManager->loadSetSDKLocationLocation());

}

SetSDKLocation::~SetSDKLocation()
{
    delete ui;
}

void SetSDKLocation::on_pushButton_clicked()
{
   ui->leLocation->setText(QFileDialog::getExistingDirectory(this,"Show",QCoreApplication::applicationDirPath()));
}

void SetSDKLocation::on_buttonBox_accepted(){} //removing this is causing an error

void SetSDKLocation::on_btnOk_clicked()
{
    QString enteredLocation = ui->leLocation->text();
    QString adbLocation = enteredLocation + "/platform-tools/adb.exe";
    qDebug()<<"adbLocation: " + adbLocation;
    //verifying location
    QFile adb(adbLocation);
    if(!adb.exists()){
        qDebug()<<"adb.exe Not Found";
        ui->lbError->setText("SDK Location incorrect. Please recheck.");
    }else{
        qDebug()<<"adb.exe Found";
        //storing location if cbRemember is checked
        if(ui->cbRemember->isChecked()){
            SettingsManager *settingsManager = new SettingsManager();
            settingsManager->setSetSDKLocationLocation(ui->leLocation->text());

        }
            //gSdkLocation is the location of the folder where adb.exe is present
                gSdkLocation = ui->leLocation->text()+"/platform-tools/";

                SetDeviceNumber *setDeviceNumber = new SetDeviceNumber();
                connect(setDeviceNumber,SIGNAL(setDeviceNumberIdSignal(bool)),this,SLOT(setDeviceNumberIdSlot(bool)));
                setDeviceNumber->setModal(true);
                setDeviceNumber->show();
                setDeviceNumber->raise();
                setDeviceNumber->activateWindow();

                close();

    }
}



void SetSDKLocation::on_btnCancel_clicked()
{
        close();
}

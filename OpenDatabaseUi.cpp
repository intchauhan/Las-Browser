#include "OpenDatabaseUi.h"
#include "ui_OpenDatabaseUi.h"
#include "SetDeviceNumber.h"
#include "SetSDKLocation.h"
#include "Globals.h"
#include "SettingsManager.h"
#include <QFileInfo>
#include <QPixmap>
#include <QSize>
#include <QFileDialog>
#include <QSqlDatabase>

OpenDatabaseUi *gOpenDatabaseUiPointer;

OpenDatabaseUi::OpenDatabaseUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDatabaseUi)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Open Database");

    //setting images on buttons
    QPixmap pixmap;
    pixmap.load(":icons/androidLogo.png");
    QIcon buttonIcon(pixmap);
    ui->btnAndroidDb->setIcon(buttonIcon);
    QSize qSize(120,88);
    ui->btnAndroidDb->setIconSize(qSize);

    QPixmap pixmap2;
    pixmap2.load(":icons/pcImage.png");
    QIcon buttonIcon2(pixmap2);
    ui->btnLocalDb->setIcon(buttonIcon2);
    ui->btnLocalDb->setIconSize(qSize);

    gOpenDatabaseUiPointer = this;

    //checkbox pre-checking
    SettingsManager *settingManager = new SettingsManager();
    ui->checkBox->setChecked(!settingManager->loadOpenDatabaseShowAtStartup());
}

OpenDatabaseUi::~OpenDatabaseUi()
{
    delete ui;
}

void OpenDatabaseUi::on_btnAndroidDb_clicked()
{
    //if sdk location not set show SetSdkLocation ui, else show SetDeviceNumber
    if(gSdkLocation.trimmed()==""){
        SetSDKLocation *setSDKLocation = new SetSDKLocation();
        setSDKLocation->setModal(true);
        setSDKLocation->show();
        setSDKLocation->raise();
        setSDKLocation->activateWindow();
    }else{
    SetDeviceNumber *setDeviceNumber = new SetDeviceNumber;
    connect(setDeviceNumber,SIGNAL(setDeviceNumberIdSignal(bool)),this,SLOT(setDeviceNumberIdSlot(bool)));
    setDeviceNumber->setModal(true);
    setDeviceNumber->show();
    setDeviceNumber->raise();
    setDeviceNumber->activateWindow();
    }
}

void OpenDatabaseUi::on_btnLocalDb_clicked()
{
    QString dbFileLocation = QFileDialog::getOpenFileName(this);
    qDebug()<<"chosen file: "<<dbFileLocation;
    //checking if such file exists
    QFileInfo checkDbFile(dbFileLocation);
    if(checkDbFile.exists() && checkDbFile.isFile()){
        qDebug()<<"db File does exists and is a file";

        gLocalDbLocation = dbFileLocation;
        gDatabaseName = checkDbFile.fileName();
        openDatabaseUiNonAndroidDbSelectedSignal();
        close();
    }
    }


void OpenDatabaseUi::on_btnCancel_clicked()
{
    close();
}

//storing checkbox preference
void OpenDatabaseUi::closeEvent(QCloseEvent *event)
{
    SettingsManager *settingManager = new SettingsManager();
    settingManager->setOpenDatabaseShowAtStartup(!ui->checkBox->isChecked());

}

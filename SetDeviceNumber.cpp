#include "SetDeviceNumber.h"
#include "ui_SetDeviceNumber.h"
#include "SetPackageName.h"
#include <QDebug>
#include <QProcess>
#include <QByteArray>

QString gDeviceId;
QProcess process;

SetDeviceNumber::SetDeviceNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDeviceNumber)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Choose Device");

    doJob();
}

SetDeviceNumber::~SetDeviceNumber()
{
    delete ui;
}

void SetDeviceNumber::doJob(){
    displayListOnlwDeviceEmulatorList(getAllConnectedDevicesEmulatorsList());

}

QStringList SetDeviceNumber::getAllConnectedDevicesEmulatorsList(){

    QString output;


    process.setWorkingDirectory(gSdkLocation);
    QString tempCommand = "cmd /c adb devices";
    qDebug()<<"command to execute: " + tempCommand;
    process.start(tempCommand);
    process.waitForFinished();
    output = process.readAllStandardOutput();

    QStringList toSetInTable = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    //deleting every line that is and before line starting with "list"
    for(int i = 0;i<toSetInTable.count();i++){
        if(toSetInTable.value(i).mid(0,4) != "List"){
            toSetInTable.takeFirst();
            i--;
        }else {
            toSetInTable.takeFirst();
            break;
        }
    }
    return toSetInTable;
}

void SetDeviceNumber::displayListOnlwDeviceEmulatorList(QStringList toSetInTable){
    ui->lwDeviceEmulatorList->clear();
    ui->lwDeviceEmulatorList->addItems(toSetInTable);
}

//SLOT
void SetDeviceNumber::on_btnRefresh_clicked()
{
    displayListOnlwDeviceEmulatorList(getAllConnectedDevicesEmulatorsList());
}

void SetDeviceNumber::on_btnDeepRefresh_clicked()
{
    //deep refreshing = restarting adb and then refreshing
    QProcess refreshProcess;
    process.start("cmd /c adb kill-server");
    process.waitForFinished();
    process.start("cmd /c adb start");
    process.waitForFinished();

    displayListOnlwDeviceEmulatorList(getAllConnectedDevicesEmulatorsList());

}

void SetDeviceNumber::on_lwDeviceEmulatorList_itemDoubleClicked(QListWidgetItem *item)
{
    QString selectedDevice = ui->lwDeviceEmulatorList->currentItem()->text();
    qDebug()<<selectedDevice;
    QStringList temp = selectedDevice.split(QRegExp("[\r\n\t]"),QString::SkipEmptyParts);
    gDeviceId= temp.at(0);
    qDebug()<<"gDeviceId: "<<gDeviceId;

    //open set package name
    SetPackageName *setPackageName = new SetPackageName();
    connect(setPackageName,SIGNAL(setPackageNameDoneSignal(bool)),this,SLOT(setPackageNameDoneSlot(bool)));
    setPackageName->setModal(true);
    setPackageName->show();
    setPackageName->raise();
    setPackageName->activateWindow();
	
    close();
}

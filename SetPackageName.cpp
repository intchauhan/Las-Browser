#include "SetPackageName.h"
#include "ui_SetPackageName.h"
#include "Globals.h"
#include "SetDatabaseName.h"
#include <QProcess>
#include <QDebug>

bool pkgNamesAccessible=true;

//global
QString gPackageName;

SetPackageName::SetPackageName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPackageName)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Set Package Name");
    setFixedSize(670,160);

    ui->lbDoubleClickInstruction->hide();
    ui->lwPackages->hide();
    ui->lbError->hide();
}



SetPackageName::~SetPackageName()
{
    delete ui;
}

void SetPackageName::on_btnNext_clicked()
{
    if(ui->lePackageName->text().trimmed()!=""){
        doFinalJob(ui->lePackageName->text().trimmed());
    }
}

void SetPackageName::on_btnListPkgOne_clicked()
{
    showPackages();
}

void SetPackageName::on_btnListPkgTwo_clicked()
{
    showPackages();
}

void SetPackageName::showPackages(){
    ui->lwPackages->show();
    ui->btnListPkgOne->hide();
    ui->btnListPkgTwo->hide();
    ui->lbDoubleClickInstruction->show();
    setFixedSize(670,277);

    //showing packages
    process.setWorkingDirectory(gSdkLocation);

    QString command = "cmd \c adb -s " + gDeviceId + " shell \"ls //data//data";
    qDebug()<<"Command to execute: "<<command;
    process.start("cmd /c adb shell \"ls //data//data ");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList toSetInTable = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    //removing lines starting from *, which are not package names
    for(int i = 0;i<toSetInTable.count();i++){
        if(toSetInTable.value(i).at(0)== '*'){
            toSetInTable.takeFirst();
            i--;
        }else {
            break;
        }
    }
    if(toSetInTable.isEmpty()){
        ui->lwPackages->addItem("Not enough permissions, maybe? Try typing the package manually.");
        pkgNamesAccessible = false;

    }else{
        ui->lwPackages->addItems(toSetInTable);
    }



}

void SetPackageName::on_lwPackages_doubleClicked(const QModelIndex &index)
{
    if(pkgNamesAccessible){
        QString selectedPackage = ui->lwPackages->currentItem()->text().trimmed();
        ui->lePackageName->setText(selectedPackage);
        //doFinalJob(selectedPackage);
    }
}

void SetPackageName::doFinalJob(QString packageName){
    qDebug()<<"func exec void SetPackageName::doFinalJob(QString packageName)";
    qDebug()<<"packageName recived: "<<packageName;

    process.setWorkingDirectory(gSdkLocation);

    /*uncomment this section for better stability but poor speed
    //deep refreshing
    process.start("cmd /c adb kill-server");
    process.waitForFinished();
    process.start("cmd /c adb start");
    process.waitForFinished();
    */

    //validating
    QString command = "cmd /c adb -s " + gDeviceId + " shell \"run-as " + packageName + " ls //data//data//"+packageName+"//""";
    qDebug()<<"To execute: " + command;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;
    QStringList outputs = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    //removing lines starting from *, which are not package names
    for(int i = 0;i<outputs.count();i++){
        if(outputs.value(i).at(0)== '*'){
            outputs.takeFirst();
            i--;
        }else {
            break;
        }
    }

    QString firstLine = outputs.at(0);
    QStringList firstLineList = firstLine.split(" ");

    if(firstLineList.value(firstLineList.count()-1) == "debuggable" ){
        qDebug()<<"NOT DEBUGGABLE";
        ui->lbError->setText("Package " + packageName + " is NOT DEBUGGABLE!");
        ui->lbError->show();
    }else if(firstLineList.value(firstLineList.count()-1)=="unknown"){
        qDebug()<<"Cannot Access the Package " + packageName + ", Please check name";
        ui->lbError->setText("CANNOT ACCESS the Package" + packageName + ", Please check name");
        ui->lbError->show();
    }else if(firstLineList.value(0)=="run-as"){
        qDebug()<<"Cannot Access the Package!";
        ui->lbError->setText("CANNOT ACCESS " + packageName);
        ui->lbError->show();
    }//discover and add more cases, if there are!
    else{
        qDebug()<<"packageName is more or less valid, we are good to go!";
        //if we are here, package name is validated
        gPackageName = packageName;

        SetDatabaseName *setDatabaseName = new SetDatabaseName();
        connect(setDatabaseName,SIGNAL(setDatabaseNameDoneSignal()),gMainWindowPointer,SLOT(setDatabaseNameDoneSlot()));
        setDatabaseName->setModal(true);
        setDatabaseName->show();
        setDatabaseName->raise();
        setDatabaseName->activateWindow();

        close();
    }
}

#include "SetDatabaseName.h"
#include "ui_SetDatabaseName.h"
#include <QProcess>
#include <QDebug>
#include "Globals.h"
#include <QStandardPaths>

bool dbNamesAccessible = true;

QString gLocalDbLocation;
QString gRemoteDbLocation;

QString gDatabaseName;
QString gDbAndroidDir;

SetDatabaseName::SetDatabaseName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDatabaseName)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Choose Database");

    ui->lbDbLocationStarting->setText("Database Directory Location: .../"+ gPackageName + "/");
    ui->leRestOfLocation->setText("databases/");

    ui->lbDoubleClickInstruction->hide();
    ui->lwDatabases->hide();
    ui->lbError->hide();
    setFixedSize(655,150);
}

SetDatabaseName::~SetDatabaseName()
{
    delete ui;
}

void SetDatabaseName::on_btnListDbOne_clicked()
{
    showDbs();
}

void SetDatabaseName::on_btnListDbTwo_clicked()
{
    showDbs();
}

void SetDatabaseName::showDbs(){
    ui->lwDatabases->show();
    ui->btnListDbOne->hide();
    ui->btnListDbTwo->hide();
    ui->lbDoubleClickInstruction->show();
    setFixedSize(655,310);

    QString command = "cmd /c adb -s " + gDeviceId + " shell \"run-as " + gPackageName + " ls //data//data//"+gPackageName+"//databases/""";
    qDebug()<<"Command to execute: "<<command;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    qDebug()<<"output"<<output;
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

    //TO DO UNCOMMENT AND MAKE THIS CODE WORK
   /*QString firstLine = toSetInTable.at(0);
    QStringList firstLineList = firstLine.split(" ");

    if(firstLineList.value(firstLineList.count()-1) == "directory" ){
        qDebug()<<"Invalid directory";
        toSetInTable.empty();

    }else if(firstLineList.value(firstLineList.count()-1)=="unknown"){
        qDebug()<<"Invalid Package";
        toSetInTable.empty();
    }else if(firstLineList.value(0)=="run-as"){
        qDebug()<<"Cannot Access the Package!";
        toSetInTable.empty();
    }//discover and add more cases, if there are!
    */

    if(toSetInTable.isEmpty()){
        ui->lwDatabases->addItem("Not enough permissions, maybe? Type the database location and manually.");
        dbNamesAccessible = false;

    }else{
        ui->lwDatabases->addItems(toSetInTable);
    }

}

void SetDatabaseName::on_lwDatabases_doubleClicked(const QModelIndex &index)
{
    if(dbNamesAccessible){
        QString dbName = ui->lwDatabases->currentItem()->text().trimmed();
        ui->leRestOfLocation->setText("databases/");
        ui->leDbName->setText(dbName);
        //doFinalJob(dbName);
    }
}



void SetDatabaseName::on_btnNext_clicked()
{
    if(ui->leDbName->text().trimmed()!="" && ui->leRestOfLocation->text().trimmed()!=""){
        doFinalJob(ui->leDbName->text().trimmed());
    }
}

void SetDatabaseName::doFinalJob(QString dbName){
    qDebug()<<"dbName received: "<<dbName;
    //TODO validation

    gDatabaseName = dbName;
    gLocalDbLocation = "C://androidsqliteExplorer//" + gPackageName + "//" + gDatabaseName;

    QString restOfLocation = ui->leRestOfLocation->text();
    //removing starting / and
    while(1){
        if(restOfLocation.at(0)!='/' && restOfLocation.at(0)!='\\'){
            break;
        }
        restOfLocation = restOfLocation.remove(0,1);
    }
    // removing ending / and
    while(1){
        if(restOfLocation.at(restOfLocation.size()-1)!='/' && restOfLocation.at(restOfLocation.size()-1)!='\\'){
            break;
        }
        restOfLocation = restOfLocation.remove(restOfLocation.size()-1,1);
    }
    qDebug()<<"restOfLocation: " << restOfLocation;

    //changing \\ to /
    for(int i = 0;i<restOfLocation.size();i++){
        if(restOfLocation.at(i)=="\\"){
            restOfLocation.replace(i,1,"/");
        }
    }

    //single / to //
    for(int i = 0;i<restOfLocation.size();i++){
        if(restOfLocation.at(i)=="/"){
            restOfLocation.insert(i,"/");
            i++;
        }
    }

    qDebug()<<"restOfLocation: " << restOfLocation;

    gRemoteDbLocation = "//data//data//" + gPackageName + "//" + restOfLocation + "//" + ui->leDbName->text().trimmed();

    qDebug()<<"gRemoteDbLocation: "<<gRemoteDbLocation;
    setDatabaseNameDoneSignal();
    close();//comment for debugging

}

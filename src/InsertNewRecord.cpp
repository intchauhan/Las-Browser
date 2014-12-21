#include "InsertNewRecord.h"
#include "ui_InsertNewRecord.h"
#include <QScrollArea>
#include <QWidgetList>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QLineEdit>
#include <QMessageBox>
#include <QMap>
#include <QMapIterator>
#include "Globals.h"

QSqlDatabase db2;
int numberOfRowsNewRecord = 1;
int columnCount = 0;
QList<QLineEdit*> lineEditList;
QString tableNameInsertNewRecord = "";
QStringList headertblInsert;
QStringList sqlgeneratedInsertStatements;
QString gDbLocalFullLocation;


InsertNewRecord::InsertNewRecord(QWidget *parent, QString tableName, QSqlDatabase *db):
    QDialog(parent),
    ui(new Ui::InsertNewRecord)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    numberOfRowsNewRecord = 1;
    columnCount = 0;
    tableNameInsertNewRecord="";

    this->setWindowTitle("Add New Records");
    tableNameInsertNewRecord = tableName;
    QString tempForlbInsert = "Inserting into '" + tableNameInsertNewRecord +"'";
    ui->lbInsertIntoTableName->setText(tempForlbInsert);
    db2 = *db;

    //setting defaults and views
    QStringList numbers;
    numbers.push_back("1");
    numbers.push_back("2");
    numbers.push_back("5");
    numbers.push_back("10");
    numbers.push_back("15");
    numbers.push_back("20");
    numbers.push_back("50");
    numbers.push_back("100");
    ui->cbNumberOfRecords->addItems(numbers);
    ui->cbNumberOfRecords->setCurrentIndex(0);

    generateInitialTableLayout();

    for(int i=0;i<columnCount;i++){
        QLineEdit *leLineEdit = new QLineEdit(this);
        QString tempi =  QString::number(i);
        QString tempObjectName = "0_" + tempi;
        leLineEdit->setObjectName(tempObjectName);
        ui->tblInsert->setCellWidget(0,i,leLineEdit);
        lineEditList.push_back(leLineEdit);
        qDebug()<<"Object name of the last leLineEdit"<<tempObjectName;
    }


}

void InsertNewRecord::openDatabase(){
   db2.setDatabaseName(gLocalDbLocation);
    if(db2.open()){
    qDebug()<<"Database opened by openDatabase function";
    }else{
        qDebug()<<"Database FAILED to opened by openDatabase() function";
        }

}

void InsertNewRecord::closeDatabase(){
    if(db2.isOpen()){
        db2.close();
        qDebug()<<"Database closed by closeDatabase() function";
}
}


void InsertNewRecord::generateInitialTableLayout(){
    qDebug()<<"func exec void InsertNewRecord::generateInitialTableLayout()";
    openDatabase();

    QSqlQuery initialQuery (db2);
    QString selectStatement = "select * from " + tableNameInsertNewRecord;
    initialQuery.exec(selectStatement);
    ui->tblInsert->clear();
    QSqlRecord rec;
    QSqlField fld;
    rec = initialQuery.record();
    columnCount = rec.count();
    ui->tblInsert->setColumnCount(columnCount);
    QString fieldName;
    int e = 0;
    while(e<rec.count()){
        fld = rec.field(e);
        fieldName = rec.fieldName(e);
        headertblInsert.push_back(fieldName);
        e++;
    }
    this->ui->tblInsert->setHorizontalHeaderLabels(headertblInsert);
    db2.close();
}


void InsertNewRecord::generateRow(){
    qDebug()<<"function exec void InsertNewRecord::generateRow();";
    numberOfRowsNewRecord++;
    for(int i=0;i<columnCount;i++){
    QLineEdit *leLineEdit = new QLineEdit(this);
    QString tempi =  QString::number(i);
    QString tempNumberOfRowsNewRecord = QString::number(numberOfRowsNewRecord);
    QString tempObjectName = tempNumberOfRowsNewRecord + "_" + tempi;
    leLineEdit->setObjectName(tempObjectName);
    qDebug()<<"Object name of the last leLineEdit"<<tempObjectName;
    ui->tblInsert->setCellWidget(numberOfRowsNewRecord-1,i,leLineEdit);
    lineEditList.push_back(leLineEdit);
    }



}

void InsertNewRecord::deleteLastRow(){

    for(int i=0;i<columnCount;i++){
        lineEditList.takeLast();
    }
    numberOfRowsNewRecord--;
}



InsertNewRecord::~InsertNewRecord()
{
    delete ui;
}

void InsertNewRecord::on_cbNumberOfRecords_currentTextChanged(const QString &cbRecordsSelectedString)
{
    qDebug()<<"function exec void InsertNewRecord::on_cbNumberOfRecords_currentTextChanged(const QString &arg1)";
    qDebug()<<cbRecordsSelectedString;
    int toBeNumberOfRows = cbRecordsSelectedString.toInt();
    ui->tblInsert->setRowCount(toBeNumberOfRows);
    while(toBeNumberOfRows!=numberOfRowsNewRecord){
        if(toBeNumberOfRows<numberOfRowsNewRecord){
            deleteLastRow();
        }else{
            generateRow();
        }
    }

}

//SLOT
void InsertNewRecord::on_btnAddRecords_clicked()
{
    qDebug()<<"func exec void InsertNewRecord::on_btnAddRecords_clicked()";

    //checking if the form is totally empty
    bool valid=false;
    for(int i=0;i<lineEditList.count();i++){
        if(lineEditList.value(i)->text().trimmed()!=""){
            valid = true;
            break;
        }
    }
    if(!valid){
        QString message = "No data is filled.";
        QMessageBox::warning(this, tr("Error"),message,QMessageBox::Ok);
        return;
    }

    //removing all unfilled lineEdits from lineEditList
    int i = 0;
    while(i<lineEditList.count()){
        QString tempObjectName = lineEditList.value(i)->objectName().trimmed();
        if(lineEditList.value(i)->text().trimmed()==""){
            qDebug()<<"lineEditList.value(i)="<<lineEditList.value(i)->text().trimmed();
            lineEditList.removeAt(i);
        }else{
            i++;
        }
    }

    QMap <QString,QString> columnsAndValues;

    QString workingRowNumber = lineEditList.value(0)->objectName();
    QStringList tempWorkingRowNumber = workingRowNumber.split("_");
    workingRowNumber = tempWorkingRowNumber.at(0);
    qDebug()<<"workingRowNumber:"<<workingRowNumber;

    QString generatedInsertingSqlFinal;
    QString generatedInsertingSqlPartOne;
    QString generatedInsertingSqlPartTwo;

    for(int i=0;i<lineEditList.count();i++){
        QString tempObjectName = lineEditList.value(i)->objectName();
        QStringList tempSplit = tempObjectName.split("_");
        //QString tempRowNumber = tempSplit.value(0);
        int tempColumnNumber = tempSplit.value(1).toInt();
        QString tempValue = lineEditList.value(i)->text();
        QString tempColumn = headertblInsert.at(tempColumnNumber);
        columnsAndValues.insertMulti(tempValue,tempColumn);
        qDebug()<<"Inserted to columnsAndValues: "<<tempValue<<" & "<<tempColumn;

            bool nextIsDifferent = false;
            bool thisIsLast = false;
            QString tempRow = "";
            if(i+1<lineEditList.count()){
                tempObjectName = lineEditList.value(i+1)->objectName();
                tempSplit = tempObjectName.split("_");
                tempRow = tempSplit.value(0);
                qDebug()<<"tempRow: "<<tempRow;
                if(tempRow!=workingRowNumber) nextIsDifferent = true;
            }
            if(i==lineEditList.count()-1) thisIsLast = true;

            if(nextIsDifferent || thisIsLast){
                //generating statement
                generatedInsertingSqlPartOne = "INSERT INTO " + tableNameInsertNewRecord + " ";
                generatedInsertingSqlPartOne += "(";

                generatedInsertingSqlPartTwo = "VALUES (";
                QMapIterator <QString,QString> iter(columnsAndValues);
                while(iter.hasNext()){
                    iter.next();
                    generatedInsertingSqlPartOne += "'" + iter.value() + "',";
                    generatedInsertingSqlPartTwo += "'" + iter.key() + "',";
                }
                generatedInsertingSqlPartOne = generatedInsertingSqlPartOne.mid(0,generatedInsertingSqlPartOne.length()-1);

                generatedInsertingSqlPartOne += ") ";
                generatedInsertingSqlPartTwo = generatedInsertingSqlPartTwo.mid(0,generatedInsertingSqlPartTwo.length()-1);

                generatedInsertingSqlPartTwo += ");";
                generatedInsertingSqlFinal = generatedInsertingSqlPartOne + generatedInsertingSqlPartTwo;
                sqlgeneratedInsertStatements.push_back(generatedInsertingSqlFinal);
                qDebug()<<"generatedInsertingSqlFinal: "<<generatedInsertingSqlFinal;

                columnsAndValues.clear();
                workingRowNumber = tempRow;
            }
    }




    //Looping through lineEditList,For debugging purpose
    for(int i = 0;i<lineEditList.count();i++){
        qDebug()<<lineEditList.value(i)->text();
    }

    executeGeneratedStatements();

    closingCeremony();
    insertNewRecordRefreshMainWindowSignal();
    close();

}

void InsertNewRecord::executeGeneratedStatements(){
    openDatabase();
    QSqlQuery exeQuery (db2);
    qDebug()<<sqlgeneratedInsertStatements.count();
    for(int i = 0;i<sqlgeneratedInsertStatements.count();i++){
    QString tempQuery = sqlgeneratedInsertStatements.value(i);
    qDebug()<<"executing"<<tempQuery;
    exeQuery.exec(tempQuery);
    }
    db2.close();

}

void InsertNewRecord::closingCeremony(){
    lineEditList.clear();
    headertblInsert.clear();
    sqlgeneratedInsertStatements.clear();
    ui->tblInsert->clear();

}

void InsertNewRecord::reject(){
        qDebug()<<"function exec reject()";
        closingCeremony();

        QDialog::reject();
}

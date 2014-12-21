#include "CreateTableFromOptions.h"
#include "ui_CreateTableFromOptions.h"
#include <QSqlField>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "Globals.h"
#include "DatabaseAdapter.h"

CreateTableFromOptions::CreateTableFromOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTableFromOptions)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Create New Table");

}


CreateTableFromOptions::~CreateTableFromOptions()
{
    delete ui;
}

void CreateTableFromOptions::on_btnGoCreateTable_clicked()
{
    DatabaseAdapter *databaseAdapter = new DatabaseAdapter();
    //checking if lineEdit leTableName is empty
    QString newTableName = ui->leTableName->text().trimmed();
    if(newTableName!=""){

        //checking for duplication in table name
        QString selectStatement = "select name, sql from sqlite_master where type = 'table'";
        databaseAdapter->openDatabase();
        QSqlQuery query (databaseAdapter->databasedb);
        query.exec(selectStatement);
        int count=0;
        bool valid = true;
        while (query.next()){
            count++;
            qDebug()<<query.value(0).toString();
            if(QString::compare(query.value(0).toString(),newTableName,Qt::CaseInsensitive)==0){
                qDebug()<<"table name already exists";
                valid = false;
            }
        }

          if(valid){
              createTableFromOptionsOpenWindowSignal(ui->leTableName->text(),ui->leNumberOfRows->text());
              close();

    }else{
            QMessageBox::warning(this, tr("Error"),
                                           tr("Table Name already exists."),
                                           QMessageBox::Ok);
        }
}
}

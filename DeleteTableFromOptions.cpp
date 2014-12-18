#include "DeleteTableFromOptions.h"
#include "ui_DeleteTableFromOptions.h"
#include "DatabaseAdapter.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

QString deleteTableFromOptionsToDelete;

DeleteTableFromOptions::DeleteTableFromOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteTableFromOptions)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Delete Table");
    populateTable();
    ui->btnDeleteTable->setEnabled(false);

}


DeleteTableFromOptions::~DeleteTableFromOptions()
{
    delete ui;
}


void DeleteTableFromOptions::populateTable(){
    QString selectStatement = "select name from sqlite_master where type = 'table'";
    DatabaseAdapter *databaseAdapter = new DatabaseAdapter();
    databaseAdapter->openDatabase();
    QSqlQuery query (databaseAdapter->databasedb);
    query.exec(selectStatement);
    QString vTableName;
    int count = 0;
    while(query.next()){
        count++;
        vTableName = query.value(0).toString();
        ui->lwTableNames->addItem(vTableName);

    }
    databaseAdapter->closeDatabase();
}

void DeleteTableFromOptions::on_btnDeleteTable_clicked()
{
    qDebug()<<"Will be deleted: " << deleteTableFromOptionsToDelete;
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    QString messageString = "Do you want to delete all the selected rows?";
    messageString = "Do you really want to delete Table " + deleteTableFromOptionsToDelete + "?";
    message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    message.setInformativeText(messageString);
    //code to increase default size of the messagebox
    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)message.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    //executing MessageBox message
    int ret = message.exec();
    switch (ret) {
      case QMessageBox::Cancel:
          qDebug()<<"Cancel was clicked";
          break;
      case QMessageBox::Ok:
    {
        QString generatedSql = "";
        generatedSql = "DROP TABLE " + deleteTableFromOptionsToDelete;
        qDebug()<<"generated sql for dropping table: " + generatedSql;
        DatabaseAdapter *databaseAdapter = new DatabaseAdapter();
        databaseAdapter->openDatabase();
        QSqlQuery query (databaseAdapter->databasedb);
        query.exec(generatedSql);
        deleteTableFromOptionsDoneSignal();
        close();
    }
}
}

void DeleteTableFromOptions::on_lwTableNames_currentTextChanged(const QString &currentText)
{
    ui->btnDeleteTable->setEnabled(true);
    deleteTableFromOptionsToDelete = currentText;
}

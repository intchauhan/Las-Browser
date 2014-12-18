#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Globals.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>
#include <QMessageBox>
#include <QCheckBox>
#include <QList>
#include <QDesktopServices>
#include <QFileInfo>
#include "SetDeviceNumber.h"
#include "SetPackageName.h"
#include "SetDatabaseName.h"
#include "AdbShellProcessor.h"
#include "DatabaseCopier.h"
#include "BackupToLocal.h"
#include "CreateTableFromOptions.h"
#include "DatabaseAdapter.h"
#include "DeleteTableFromOptions.h"
#include "About.h"
#include "CheckForUpdates.h"
#include "OpenDatabaseUi.h"
#include "SetSDKLocation.h"
#include "CreateNewDatabase.h"
#include "SettingsManager.h"
#include <QTreeWidget>
#include <QSettings>

QString gVersionNumber = "1.4.0";

QObject *gMainWindowPointer;

bool isDatabaseOpenedInSoftware;
QString gTypeOfDatabaseOpened;
QString tempStatement;
CreateTable *createTable;
InsertNewRecord *insertNewRecord;
QList<QCheckBox*> cbxRowList;
QList<int *> rowIdList;

DatabaseAdapter *databaseAdapter;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    disableView();

    SettingsManager *settingsManager = new SettingsManager();
    if(settingsManager->loadOpenDatabaseShowAtStartup()){
        OpenDatabaseUi *openDatabaseUi = new OpenDatabaseUi();
        connect(openDatabaseUi, SIGNAL(openDatabaseUiNonAndroidDbSelectedSignal()),this,SLOT(openDatabaseUiNonAndroidDbSelectedSlot()));
        openDatabaseUi->setModal(true);
        openDatabaseUi->show();
        openDatabaseUi->raise();
        openDatabaseUi->activateWindow();
    }

    gTypeOfDatabaseOpened = "none";

    databaseAdapter = new DatabaseAdapter();

    this->setWindowTitle("Las Browser for Android Sqlite");

    ui->btnDeleteRecord->setEnabled(false);

    ui->tabWidget->setTabText(0,"Execute SQL");
    ui->tabWidget->setTabText(1,"Browse");
    ui->tabWidget->setTabText(2,"Structure");

    QPixmap pixmap(":icons/btnExecute");
    QIcon ButtonIcon(pixmap);
    ui->btnExecute->setIcon(ButtonIcon);
    ui->btnExecute->setText("Execute");
    ui->tabWidget->setCurrentIndex(2);

    openDatabase();
    tableInfo();
    trwDatabaseExplorerRefresh();
    tblStructureRefresh();
    cbTableNamesRefresh();
    closeDatabase();

    //disabling database encoding options till they are functional
    ui->actionUTF_8->setEnabled(false);
    ui->actionUTF_16->setEnabled(false);
    ui->menuDatabase_encoding->setEnabled(false);

    gMainWindowPointer = this;

}

void MainWindow::setDeviceNumberIdSlot(bool gotId){
}

void MainWindow::insertNewRecordRefreshMainWindowSlot(){
    qDebug()<<"void MainWindow::insertNewRecordRefreshMainWindowSlot()";
    refreshView();
}

void MainWindow::setPackageNameDoneSlot(bool donePackage){
}

void MainWindow::deleteTableFromOptionsDoneSlot(){
    qDebug()<<"func exec void MainWindow::deleteTableFromOptionsDoneSlot()";
    refreshView();
}

void MainWindow::setDatabaseNameDoneSlot(){
    gOpenDatabaseUiPointer->close();

    qDebug()<<"func exec void MainWindow::setDatabaseNameDoneSlot();";
    DatabaseCopier *databaseCopier = new DatabaseCopier();
    databaseCopier->copyToLocal();
    ui->statusBar->showMessage("Database Name: " + gDatabaseName);
    gTypeOfDatabaseOpened = "android";
    refreshView();
    isDatabaseOpenedInSoftware = true;
    enableView();
}

void MainWindow::tblBrowseRefresh(QString tableName){
    openDatabase();
    QSqlQuery query (databaseAdapter->databasedb);
    QString selectStatement = "SELECT rowid,* FROM " + tableName + ";";
    query.exec(selectStatement);

    this->ui->tblBrowse->clear();
    cbxRowList.clear();
    QSqlRecord rec;
    QSqlField fld;
    rec = query.record();
    qDebug()<<"rec.count: "<<rec.count();
    //setting column count to number of fields
    this->ui->tblBrowse->setColumnCount(rec.count());
    //setting header names
    QStringList header;
    header.push_back(" ");
    QString fieldName;
    //e=1 because we don't want to show rowid
    int e = 1;
    while(e<rec.count()){
        fld = rec.field(e);
        fieldName = rec.fieldName(e);
        header.push_back(fieldName);
        e++;
    }
    this->ui->tblBrowse->setHorizontalHeaderLabels(header);
    this->ui->tblBrowse->horizontalHeader()->resizeSection(0,20);

    QLabel *label;
    int count=0;

    QList<QString> dynamicFields;
    int dynamicFieldsCount = 0;

    while (query.next()){
        count++;
        this->ui->tblBrowse->setRowCount(count);

        //creating & adding checkbox
        QCheckBox *cbxRow = new QCheckBox(this);
        QString rowId = query.value(0).toString();
        qDebug()<<"rowid:"<<rowId;

        cbxRow->setObjectName(rowId);
        cbxRow->setCheckable(true);
        QWidget *cbxRowWidget = new QWidget();
        QHBoxLayout *cbxRowLayout = new QHBoxLayout(cbxRowWidget);
        cbxRowLayout->addWidget(cbxRow);
        cbxRowLayout->setAlignment(Qt::AlignCenter);
        cbxRowLayout->setContentsMargins(0,0,0,0);
        cbxRowWidget->setLayout(cbxRowLayout);
        this->ui->tblBrowse->setCellWidget(count-1,0,cbxRowWidget);
        connect(cbxRow,SIGNAL(stateChanged(int)),this,SLOT(cbxRowStateChanged(int)));


        for(int i=1;i<rec.count();i++){
            dynamicFields.push_back(query.value(i).toString());

            label = new QLabel(this->ui->tblExecute);
            label->setText(dynamicFields.back());
            this->ui->tblBrowse->setCellWidget(count-1,i,label);
            }
        
        cbxRowList.push_back(cbxRow);
    }
    closeDatabase();
}


//ui
void MainWindow::cbTableNamesRefresh(){
    qDebug()<<"function execution: cbTableNamesRefresh()";
    ui->cbTableNames->clear();
    QStringList tableNames;
    QString selectStatement = "select name from sqlite_master where type = 'table'";
    openDatabase();
    QSqlQuery query(databaseAdapter->databasedb);
    query.exec(selectStatement);
    tableNames.clear();
    while (query.next()){
        tableNames.push_back(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    tableNames.sort();


    ui->cbTableNames->insertItems(0,tableNames);
}

//ui
void MainWindow::trwDatabaseExplorerRefresh(){
    ui->trwDatabaseExplorer->clear();

    QTreeWidgetItem *root = new QTreeWidgetItem(ui->trwDatabaseExplorer);
    root->setText(0,gDatabaseName);
    QPixmap pixmap(":icons/Table/s_db.png");
    QIcon ButtonIcon(pixmap);
    root->setIcon(0,pixmap);
    ui->trwDatabaseExplorer->addTopLevelItem(root);

    qDebug()<<"function execution: MainWindow::trwDatabaseExplorerRefresh()";

    QStringList tableNames;
    QString selectStatement = "select name from sqlite_master where type = 'table'";
    openDatabase();
    QSqlQuery query(databaseAdapter->databasedb);
    query.exec(selectStatement);
    tableNames.clear();
    while (query.next()){
        tableNames.push_back(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }

    tableNames.sort();

    for(int i=0;i<tableNames.size();i++){
        //adding table names
        QTreeWidgetItem *tableItem = new QTreeWidgetItem();
        tableItem->setText(0,tableNames.at(i));
        QPixmap pixmap(":icons/Table/s_tbl.png");
        QIcon ButtonIcon(pixmap);
        tableItem->setIcon(0,pixmap);
        root->addChild(tableItem);

        //adding fields inside tableName
        openDatabase();
        QSqlQuery query (databaseAdapter->databasedb);
        QString selectStatement = "SELECT rowid,* FROM " + tableNames.at(i) + ";";
        query.exec(selectStatement);

        QSqlRecord rec;
        QSqlField fld;
        rec = query.record();
        qDebug()<<"rec.count: "<<rec.count();
        QStringList fieldNames;
        QString fieldName;
        //e=1 because we don't want to show rowid
        int e = 1;
        while(e<rec.count()){
            fld = rec.field(e);
            fieldName = rec.fieldName(e);
            fieldNames.push_back(fieldName);
            e++;
        }
        for(int i=0;i<fieldNames.count();i++){
            QTreeWidgetItem *field = new QTreeWidgetItem();
            field->setText(0,fieldNames.at(i));
            QPixmap pixmap(":icons/Table/s_row.png");
            QIcon ButtonIcon(pixmap);
            field->setIcon(0,pixmap);
            tableItem->addChild(field);
        }
    }
    //ui->trwDatabaseExplorer->expandAll();
    ui->trwDatabaseExplorer->expandItem(root);
}

//ui
void MainWindow::tblStructureRefresh(){
    qDebug()<<"function execution: tblStructureRefresh()";
    QString selectStatement = "select name, sql from sqlite_master where type = 'table'";
    openDatabase();
    QSqlQuery query (databaseAdapter->databasedb);
    query.exec(selectStatement);
    QString vTableName;
    QString vDefinition;
    QString vNumberOfRows;
    this->ui->tblStructure->clear();
    QStringList header;
    header.push_back("Table");
    header.push_back("SQL Definition");
    header.push_back("Browse");
    header.push_back("Insert");
    header.push_back("Drop");
    //header.push_back("Number of Rows"); //feature to be added in future
    this->ui->tblStructure->setColumnCount(5);
    this->ui->tblStructure->setHorizontalHeaderLabels(header);
    QLabel *label;
    QPushButton *button;
    int count=0;
    while (query.next()){
        this->ui->tblStructure->setRowCount(count+1);
        vTableName = query.value(0).toString();
        vDefinition = query.value(1).toString().simplified();
        qDebug()<<"definition:"<<vDefinition;

        label = new QLabel(this->ui->tblStructure);
        label->setText(vTableName);
        ui->tblStructure->setCellWidget(count,0,label);

        label = new QLabel(this->ui->tblStructure);
        label->setText(QString::QString(vDefinition));

        button = new QPushButton();
        //setting the object name as the sql definition
        button->setObjectName(QString(query.value(1).toString()));
        button->setText("...");
        button->setMaximumWidth(20);
        connect(button,SIGNAL(clicked()),this,SLOT(structureDefinitionClickedSlot()));
        QWidget *definitionWidget = new QWidget();
        QHBoxLayout *definitionLayout = new QHBoxLayout(definitionWidget);
        definitionLayout->addWidget(label);
        definitionLayout->addWidget(button);
        definitionLayout->setAlignment(Qt::AlignCenter);
        definitionLayout->setContentsMargins(0,0,0,0);
        definitionWidget->setLayout(definitionLayout);

        ui->tblStructure->setCellWidget(count,1,definitionWidget);


        button = new QPushButton(ui->tblStructure);
        button->setText("Browse");
        button->setObjectName(QString(query.value(0).toString()));
        this->ui->tblStructure->setCellWidget(count,2,button);
        connect(button,SIGNAL(clicked()),this,SLOT(structureBrowseClickSlot()));

        button = new QPushButton(ui->tblStructure);
        button->setText("Insert");
        button->setObjectName(QString(query.value(0).toString()));
        this->ui->tblStructure->setCellWidget(count,3,button);
        connect(button,SIGNAL(clicked()),this,SLOT(structureInsertClickedSlot()));

        button = new QPushButton(ui->tblStructure);
        button->setText("Drop");
        button->setObjectName(QString(query.value(0).toString()));
        this->ui->tblStructure->setCellWidget(count,4,button);
        connect(button,SIGNAL(clicked()),this,SLOT(structureDropClickedSlot()));

        label = new QLabel(this->ui->tblStructure);
        label->setText(vNumberOfRows);
        this->ui->tblStructure->setCellWidget(count,5,label);
    count++;
    }
}

void MainWindow::structureBrowseClickSlot(){
    QObject *obj = sender();
    QString tableName = obj->objectName();
    ui->tabWidget->setCurrentIndex(1);
    int toSelect = ui->cbTableNames->findText(tableName);
    ui->cbTableNames->setCurrentIndex(toSelect);
}

void MainWindow::structureDropClickedSlot(){
    QObject *obj = sender();
    QString tableName = obj->objectName();

    qDebug()<<"Table will be deleted: " << tableName;
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    QString messageString = "Do you want to delete all the selected rows?";
    messageString = "Do you really want to drop Table: " + tableName + "?";
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
        generatedSql = "DROP TABLE " + tableName;
        qDebug()<<"generated sql for dropping table: " + generatedSql;
        DatabaseAdapter *databaseAdapter = new DatabaseAdapter();
        databaseAdapter->openDatabase();
        QSqlQuery query (databaseAdapter->databasedb);
        query.exec(generatedSql);
        refreshView();
    }
}

}

void MainWindow::structureInsertClickedSlot(){
    QObject *obj = sender();
    QString tableName = obj->objectName();
    insertNewRecord = new InsertNewRecord(this,tableName,&databaseAdapter->databasedb);
    connect(insertNewRecord,SIGNAL(insertNewRecordRefreshMainWindowSignal()),this,SLOT(insertNewRecordRefreshMainWindowSlot()));
    insertNewRecord->show();
    insertNewRecord->raise();
    insertNewRecord->activateWindow();
    insertNewRecord->setAttribute(Qt::WA_DeleteOnClose, true);


}

void MainWindow::structureDefinitionClickedSlot(){
    QObject* obj = sender();
    QString messageString = obj->objectName();
    QMessageBox message;
    message.setIcon(QMessageBox::Information);
    message.setStandardButtons(QMessageBox::Ok);
    message.setInformativeText(messageString);
    //code to increase default size of the messagebox
    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)message.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    int ret = message.exec();
    switch (ret) {
      default:
          return;
          break;
}
}


void MainWindow::tableInfo(){
    qDebug()<<"function execution: tableInfo";
    QString selectStatement = "select * from students";
    openDatabase();
    QSqlQuery query(databaseAdapter->databasedb);
    query.exec(selectStatement);
    QSqlRecord rec;
    QSqlField fld;

    rec = query.record();
    qDebug()<<rec.count()<<endl;
    QString fieldName;
    int e = 0;
    while(e<rec.count()){
        fld = rec.field(e);
        fieldName = rec.fieldName(e);
        qDebug()<<fieldName;

        if(fld.type() == QVariant::Int){
            qDebug()<<"Integer";
        }
        if(fld.type() == QVariant::String){
            qDebug()<<"String";
        }
        e++;
}
    closeDatabase();
    qDebug()<<"End function MainWindow::tableInfo()";
}


void MainWindow::openDatabase(){
    databaseAdapter->databasedb.setDatabaseName(gLocalDbLocation);
    qDebug()<<"gLocalDbLocation:"<<gLocalDbLocation;
    databaseAdapter->openDatabase();
}

void MainWindow::closeDatabase(){
    if(databaseAdapter->databasedb.isOpen()){
        databaseAdapter->databasedb.close();
        qDebug()<<"Database closed by closeDatabase() function";
}}

void MainWindow::clear(){
}

void MainWindow::refreshView(){
    //saving selected item text of combobox cbxTableNames for setting it again after refresh takes place
    QString currentText = ui->cbTableNames->currentText();
    openDatabase();
    tableInfo();
    tblStructureRefresh();
    cbTableNamesRefresh();
    trwDatabaseExplorerRefresh();
    closeDatabase();
    //restoring combobox cbxTableName selected item to what was selected before the action
    ui->cbTableNames->setCurrentText(currentText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//SLOT (ui element)
void MainWindow::on_actionExit_triggered()
{
    if(isDatabaseOpenedInSoftware){
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        QString messageString = "All the unsaved/unpushed changes will be lost. Continue?";
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setInformativeText(messageString);
        //code to increase default size of the messagebox
        QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)message.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        //executing MessageBox message
        int ret = message.exec();
        switch (ret) {
          case QMessageBox::No:
              qDebug()<<"Cancel was clicked";
              return;
              break;
    }
    }

    if(databaseAdapter->databasedb.isOpen()) databaseAdapter->databasedb.close();
    exit(0);
}

void MainWindow::on_btnExecute_clicked()
{
    tempStatement = ui->teSqlInput->toPlainText().trimmed();
    if(tempStatement.isEmpty()) {
        ui->tbSqlOutput->setText("<i>No SQL statement entered.</i>");
        ui->tbSqlOutput->setStyleSheet("color:gray;");
    }else{
        //determining if its a SELECT statement
        bool isSelect = false;
        int tempIndex;
        QString tempStatementType;
        tempIndex = tempStatement.indexOf(" ");
        tempStatementType = tempStatement.left(tempIndex).toUpper();
        qDebug()<<"First word of query: " + tempStatementType;
        if(tempStatementType=="SELECT") isSelect=true;
        if(isSelect){
            selectStatementExe();
        }else{
            nonSelectStatementExe();
        }
        //refreshing the ui
        refreshView();
    }
}

void MainWindow::selectStatementExe(){
    openDatabase();
    QSqlQuery query (databaseAdapter->databasedb);
    QString selectStatement = tempStatement;
    query.exec(selectStatement);

    ui->tblExecute->clear();
    QSqlRecord rec;
    QSqlField fld;
    rec = query.record();
    ui->tblExecute->setColumnCount(rec.count());
    //setting header names
    QStringList header;
    QString fieldName;
    int e = 0;
    while(e<rec.count()){
        fld = rec.field(e);
        fieldName = rec.fieldName(e);
        header.push_back(fieldName);
        e++;
    }

    QList<QString> dynamicFields;
    int dynamicFieldsCount = 0;

    this->ui->tblExecute->setColumnCount(rec.count());
    this->ui->tblExecute->setHorizontalHeaderLabels(header);
    QLabel *label;
    int count=0;
    while (query.next()){
        count++;
        this->ui->tblExecute->setRowCount(count);

        for(int i=0;i<rec.count();i++){
            dynamicFields.push_back(query.value(i).toString());


        label = new QLabel(this->ui->tblExecute);
        label->setText(dynamicFields.back());
        this->ui->tblExecute->setCellWidget(count-1,i,label);
        }

    }

    tbSqlOutputSuccess(tempStatement);
    closeDatabase();
}

void MainWindow::tbSqlOutputSuccess(QString tempStatement){
    ui->tbSqlOutput->setStyleSheet("color:black");
    ui->tbSqlOutput->setText("<font color='brown'><i> Statement executed: </i></font>"+tempStatement);
}

void MainWindow::nonSelectStatementExe(){
    openDatabase();
    QSqlQuery query(databaseAdapter->databasedb);
    query.exec(tempStatement);


    QString output =(ui->teSqlInput->toPlainText().trimmed());
    output+="\n Executed";
    if(output.trimmed().isEmpty()){
        tbSqlOutputSuccess(tempStatement);
    }else{
        ui->tbSqlOutput->setStyleSheet("color:black");
        ui->tbSqlOutput->setText(output);
    }
    closeDatabase();
}

QString MainWindow::executeQuery(QString statement){
    QString output="";
    openDatabase();
    QSqlQuery query (databaseAdapter->databasedb);
    query.exec(statement);

    QSqlRecord rec;
    QSqlField fld;

    rec = query.record();
    qDebug()<<rec;

    closeDatabase();

    return output;
}

void MainWindow::on_cbTableNames_currentIndexChanged(const QString &tableName)
{
    qDebug()<<"Table selected in cbTableName:" << tableName;
    tblBrowseRefresh(tableName);
    ui->btnDeleteRecord->setEnabled(false);
}

void MainWindow::on_btnCreateTable_clicked()
{
    //checking if lineEdit leTableName is empty
    QString newTableName = ui->leTableName->text().trimmed();
    if(newTableName!=""){

        //checking for duplication in table name
        QString selectStatement = "select name, sql from sqlite_master where type = 'table'";
        openDatabase();
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
          createTable = new CreateTable(this,ui->leTableName->text(),ui->leNumberOfRows->text().trimmed().toInt());
          connect(createTable,SIGNAL(createTableCopyStatementSignal(QString)),this,SLOT(createTableCopyStatementSlot(QString)));
          connect(createTable,SIGNAL(createTableExecuteSignal(QString,QString)),this,SLOT(createTableExecuteSlot(QString,QString)));
          createTable->show();
          createTable->raise();
          createTable->activateWindow();
    }else{
            QMessageBox::warning(this, tr("Error"),
                                           tr("Table Name already exists."),
                                           QMessageBox::Ok);
        }
}
}

//Slot
void MainWindow::createTableFromOptionsOpenWindowSlot(QString tableName,QString numberOfRows){
    qDebug()<<"func exec void createTableFromOptionsOpenWindowSlot(QString,QString)";
    createTable = new CreateTable(this,tableName,numberOfRows.toInt());
    connect(createTable,SIGNAL(createTableCopyStatementSignal(QString)),this,SLOT(createTableCopyStatementSlot(QString)));
    connect(createTable,SIGNAL(createTableExecuteSignal(QString,QString)),this,SLOT(createTableExecuteSlot(QString,QString)));
    createTable->show();
    createTable->raise();
    createTable->activateWindow();
}

//SLOTS
//slot for createTable:Copy statement to teSqlInput
void MainWindow::createTableCopyStatementSlot(QString statement){
    qDebug()<<"Running function void MainWindow::createTableCopyStatementSlot(QString statement)";
    qDebug()<<"Query Received:"<<statement;
    ui->tabWidget->setCurrentIndex(0);
    ui->teSqlInput->setPlainText(statement);

}

//slot for createTable:Execute statement
void MainWindow::createTableExecuteSlot(QString statement, QString tableName){
    qDebug()<<"function exec void MainWindow::createTableExecuteSlot(QString statement)";
    qDebug()<<"Query Received:"<<statement;
    qDebug()<<"tableName received: "<<tableName;
    QString output = executeQuery(statement);
    refreshView();
    ui->tabWidget->setCurrentIndex(0);//displaying apropriate tab
    int toSelect = ui->cbTableNames->findText(tableName);
    qDebug()<<"toSelect: "<<toSelect;
    ui->cbTableNames->setCurrentIndex(toSelect);
}

void MainWindow::createNewDatabaseDoneSlot(){
    qDebug()<<"func exec void MainWindow::createNewDatabaseDoneSlot()";
    gTypeOfDatabaseOpened = "nonandroid";
    isDatabaseOpenedInSoftware = true;
    enableView();
    refreshView();
}

//SLOT (ui element)
void MainWindow::on_actionRefresh_triggered()
{
    refreshView();
}

void MainWindow::on_btnDeleteRecord_clicked()
{

    bool isMultipleCbxRowChecked = false;
    int forMultiple = 0;
    QList<QString> rowIdsToDelete;

    int i = -1;
    for(i=0;i<cbxRowList.count();i++){
        if(cbxRowList.value(i)->isChecked()){
            rowIdsToDelete.push_back(cbxRowList.value(i)->objectName());
        }
    }

    if(rowIdsToDelete.count()>1) isMultipleCbxRowChecked = true;

    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    QString messageString = "Do you want to delete all the selected rows?";
    if(!isMultipleCbxRowChecked){
       messageString = "Do you want to delete the selected row where rowid = " + rowIdsToDelete.value(0);
    }

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
          qDebug()<<"OK was clicked";
          //generating sql delete statement
          QString executeQuery = "";
          QString tempTableName =  ui->cbTableNames->currentText();
          executeQuery+="DELETE FROM '" + tempTableName;
          executeQuery+="' WHERE _rowid_ = '";
          executeQuery+= rowIdsToDelete.value(0);
          executeQuery+="'";
          for(i=1;i<rowIdsToDelete.count();i++){
              executeQuery+=" OR ";
              executeQuery+="_rowid_ = '";
              executeQuery+=rowIdsToDelete.value(i);
              executeQuery+="'";
          }
          executeQuery+=";";
          qDebug()<<"executeQuery: "<<executeQuery;
          openDatabase();
          QSqlQuery query(databaseAdapter->databasedb);
          query.exec(executeQuery);
          refreshView();
          ui->cbTableNames->setCurrentText(tempTableName);
          closeDatabase();
    }
          break;
      default:
          // should never be reached
          break;
    }
}

//SLOT
void MainWindow::cbxRowStateChanged(int){
    qDebug()<<"function exec void MainWindow::cbxRowStateChanged(int)";
    bool isAnycbxRowChecked = false;
    for(int i=0;i<cbxRowList.count();i++){
        if(cbxRowList.value(i)->isChecked()){
            isAnycbxRowChecked = true;
            break;
        }
    }

    if(isAnycbxRowChecked){
       ui->btnDeleteRecord->setEnabled(true);
    }else{
       ui->btnDeleteRecord->setEnabled(false);
    }

}

//SLOT
void MainWindow::on_btninsertNewRecord_clicked()
{
    qDebug()<<"function run void MainWindow::on_btninsertNewRecord_clicked()";
    QString temp = ui->cbTableNames->currentText();
    insertNewRecord = new InsertNewRecord(this,temp,&databaseAdapter->databasedb);
    connect(insertNewRecord,SIGNAL(insertNewRecordRefreshMainWindowSignal()),this,SLOT(insertNewRecordRefreshMainWindowSlot()));
    insertNewRecord->show();
    insertNewRecord->raise();
    insertNewRecord->activateWindow();
    insertNewRecord->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionWrite_Changes_triggered()
{
    DatabaseCopier *databaseCopier = new DatabaseCopier();
    databaseCopier->pushToDevice();
}

void MainWindow::on_actionDiscard_all_Unpushed_Changes_and_Resync_triggered()
{
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    QString messageString = "This will discard all the unsaved changes and database from the phone will be loaded again. Do you wish to continue?";
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
        qDebug()<<"OK was clicked";
        DatabaseCopier *databaseCopier = new DatabaseCopier();
        databaseCopier->copyToLocal();
        refreshView();
    }
}
}

void MainWindow::on_actionBackupDb_triggered()
{
   BackupToLocal *backupToLocal = new BackupToLocal();
   backupToLocal->setModal(true);
   backupToLocal->show();
   backupToLocal->raise();
   backupToLocal->activateWindow();
}

void MainWindow::on_actionCreate_Table_triggered()
{
    CreateTableFromOptions *createTableFromOptions = new CreateTableFromOptions();
    connect(createTableFromOptions,SIGNAL(createTableFromOptionsOpenWindowSignal(QString,QString)),this,SLOT(createTableFromOptionsOpenWindowSlot(QString,QString)));
    createTableFromOptions->setModal(true);
    createTableFromOptions->show();
    createTableFromOptions->raise();
    createTableFromOptions->activateWindow();
}

void MainWindow::on_actionDelete_Table_triggered()
{
    DeleteTableFromOptions *deleteTableFromOptions = new DeleteTableFromOptions();
    connect(deleteTableFromOptions,SIGNAL(deleteTableFromOptionsDoneSignal()),this,SLOT(deleteTableFromOptionsDoneSlot()));
    deleteTableFromOptions->setModal(true);
    deleteTableFromOptions->show();
    deleteTableFromOptions->raise();
    deleteTableFromOptions->activateWindow();
}

void MainWindow::on_actionAbout_triggered()
{
    About *about = new About();
    about->setModal(true);
    about->show();
    about->raise();
    about->activateWindow();
}

void MainWindow::on_actionWebsite_triggered()
{
    QString link = "http://lasbrowser.intchauhan.com";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionDeveloper_triggered()
{
    QString link = "http://intchauhan.com/";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionReport_Bug_triggered()
{
    QString link = "http://lasbrowser.intchauhan.com/#contact";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    CheckForUpdates *checkForUpdates = new CheckForUpdates();
    checkForUpdates->setModal(true);
    checkForUpdates->show();
    checkForUpdates->raise();
    checkForUpdates->activateWindow();
}

void MainWindow::on_actionClose_Database_triggered()
{
    if(gTypeOfDatabaseOpened=="android"){
    databaseAdapter->closeDatabase();
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    QString messageString = "Do you want to Push Changes you made to the device before closing?";

    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
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
      case QMessageBox::Yes:
    {
    //TODO push changes to device
        DatabaseCopier *databaseCopier = new DatabaseCopier();
        databaseCopier->pushToDevice();
    }
       case QMessageBox::No:
            //nothing to do here
       default:
    {
        isDatabaseOpenedInSoftware = false;
        databaseAdapter->closeDatabase();
        disableView();
        gLocalDbLocation = "";
    }
    }
    }else{
        databaseAdapter->closeDatabase();
        isDatabaseOpenedInSoftware = false;
        disableView();
        gLocalDbLocation = "";
    }
}

void MainWindow::disableView(){
    //disabling buttons and actions
    ui->btnCreateTable->setEnabled(false);
    ui->btnDeleteRecord->setEnabled(false);
    ui->btnExecute->setEnabled(false);
    ui->btninsertNewRecord->setEnabled(false);
    ui->actionBackupDb->setEnabled(false);
    ui->actionClose_Database->setEnabled(false);
    ui->actionCreate_Table->setEnabled(false);
    ui->actionDelete_Table->setEnabled(false);
    ui->actionDiscard_all_Unpushed_Changes_and_Resync->setEnabled(false);
    ui->actionEdit_Table->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->actionUTF_8->setEnabled(false);
    ui->actionUTF_16->setEnabled(false);
    ui->actionWrite_Changes->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);

    //emptying ui elements
    ui->cbTableNames->clear();
    ui->leNumberOfRows->clear();
    ui->leTableName->clear();
    ui->tblBrowse->clear();
    ui->tblExecute->clear();
    ui->tblStructure->clear();
    ui->statusBar->showMessage("");
    ui->teSqlInput->setEnabled(false);
    ui->trwDatabaseExplorer->clear();
    ui->trwDatabaseExplorer->setEnabled(false);
}

void MainWindow::enableView(){
    if(gTypeOfDatabaseOpened=="android"){
        //enabling
        ui->actionCreate_Table->setEnabled(true);
        ui->actionNew_Database->setEnabled(true);
        ui->actionOpen_Database->setEnabled(true);
        ui->actionChange_SDK_Location->setEnabled(true);
        ui->actionClose_Database->setEnabled(true);
        ui->actionEdit_Table->setEnabled(true);
        ui->actionDelete_Table->setEnabled(true);
        ui->actionRefresh->setEnabled(true);
        ui->actionBackupDb->setEnabled(true);
        ui->actionWrite_Changes->setEnabled(true);
        ui->actionDiscard_all_Unpushed_Changes_and_Resync->setEnabled(true);
        ui->btnExecute->setEnabled(true);
        ui->btninsertNewRecord->setEnabled(true);
        ui->btnCreateTable->setEnabled(true);
        ui->btnDeleteRecord->setEnabled(true);
        ui->teSqlInput->setEnabled(true);
        ui->trwDatabaseExplorer->setEnabled(true);
        //disabling
        ui->actionSave_As->setEnabled(false);
        ui->actionSave->setEnabled(false);
        return;
    }

    //for local db
    //enabling
    ui->btnCreateTable->setEnabled(true);
    ui->btnDeleteRecord->setEnabled(true);
    ui->btnExecute->setEnabled(true);
    ui->btninsertNewRecord->setEnabled(true);
    ui->actionClose_Database->setEnabled(true);
    ui->actionCreate_Table->setEnabled(true);
    ui->actionDelete_Table->setEnabled(true);
    ui->actionEdit_Table->setEnabled(true);
    ui->actionRefresh->setEnabled(true);
    //ui->actionUTF_8->setEnabled(true); //uncomment after adding functionality
    //ui->actionUTF_16->setEnabled(true); //uncomment after adding functionality
    ui->teSqlInput->setEnabled(true);
    ui->trwDatabaseExplorer->setEnabled(true);

    //disabling
    ui->actionBackupDb->setEnabled(false);
    ui->actionDiscard_all_Unpushed_Changes_and_Resync->setEnabled(false);
    ui->actionWrite_Changes->setEnabled(false);
}

void MainWindow::on_actionOpen_Database_triggered()
{
    if(isDatabaseOpenedInSoftware){
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        QString messageString = "All the unsaved/unpushed changes will be lost. Continue?";
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setInformativeText(messageString);
        //code to increase default size of the messagebox
        QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)message.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        //executing MessageBox message
        int ret = message.exec();
        switch (ret) {
          case QMessageBox::No:
              qDebug()<<"Cancel was clicked";
              return;
              break;
          case QMessageBox::Yes:
        {
            disableView();
            isDatabaseOpenedInSoftware = false;
            gLocalDbLocation = "";
        }
    }
    }

    OpenDatabaseUi *openDatabaseUi = new OpenDatabaseUi();
    connect(openDatabaseUi, SIGNAL(openDatabaseUiNonAndroidDbSelectedSignal()),this,SLOT(openDatabaseUiNonAndroidDbSelectedSlot()));
    openDatabaseUi->setModal(true);
    openDatabaseUi->show();
    openDatabaseUi->raise();
    openDatabaseUi->activateWindow();
}


void MainWindow::openDatabaseUiNonAndroidDbSelectedSlot(){
    qDebug()<<"func exec openDatabaseUiNonAndroidDbSelectedSignal";
    ui->statusBar->showMessage("Database Name: " + gDatabaseName);
    gTypeOfDatabaseOpened = "nonandroid";
    isDatabaseOpenedInSoftware = true;
    enableView();
    refreshView();
}

void MainWindow::on_actionChange_SDK_Location_triggered()
{
    SetSDKLocation *setSdkLocation = new SetSDKLocation();
    setSdkLocation->setModal(true);
    setSdkLocation->show();
    setSdkLocation->raise();
    setSdkLocation->activateWindow();
}

void MainWindow::on_actionNew_Database_triggered()
{
    if(isDatabaseOpenedInSoftware){
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        QString messageString = "All the unsaved/unpushed changes will be lost and the current database will be closed. Continue?";
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setInformativeText(messageString);
        //code to increase default size of the messagebox
        QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)message.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        //executing MessageBox message
        int ret = message.exec();
        switch (ret) {
          case QMessageBox::No:
              qDebug()<<"Cancel was clicked";
              return;
              break;
          case QMessageBox::Yes:
            break;
        }
    }
    disableView();
    isDatabaseOpenedInSoftware = false;
    gLocalDbLocation = "";
    gTypeOfDatabaseOpened = "none";

    CreateNewDatabase *createNewDatabase = new CreateNewDatabase();
    connect(createNewDatabase,SIGNAL(createNewDatabaseDoneSignal()),this,SLOT(createNewDatabaseDoneSlot()));
    createNewDatabase->setModal(true);
    createNewDatabase->show();
    createNewDatabase->raise();
    createNewDatabase->activateWindow();
}

void MainWindow::on_actionSave_triggered()
{
    //to implement
}

void MainWindow::on_actionSave_As_triggered()
{
    //to implement
}

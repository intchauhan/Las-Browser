#include "CreateTable.h"
#include "ui_CreateTable.h"
#include <QHBoxLayout>
#include <QList>
#include <QSqlQuery>
#include <QMessageBox>
#include "Globals.h"


CreateTable::CreateTable(QWidget *parent, QString enteredTableName, int enteredNumberOfRows) :
    QDialog(parent),
    ui(new Ui::CreateTable)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle("Create New Table");

    tableName = enteredTableName;
    numberOfRows=enteredNumberOfRows;
    if(numberOfRows==0) numberOfRows = 1;
    ui->btnExecute->setEnabled(false);
    ui->btnCopyToteSqlInput->setEnabled(false);
    ui->lbNewTable->setText("Table Name: <font color='brown'><strong>" + tableName.trimmed());
    generateTableLayout();
    generateInitialRows();
}

void CreateTable::generateTableLayout(){
    ui->tblCreateTable->setRowCount(numberOfRows);
    ui->tblCreateTable->setColumnCount(8);
    QStringList titles;
    titles.push_back("Name");
    titles.push_back("Data Type");
    titles.push_back("Check");
    titles.push_back("Default Value");
    titles.push_back("Not Null");
    titles.push_back("Primary");
    titles.push_back("Unique");
    titles.push_back("Auto Increment");
    titles.push_back("Scale");//unimplemented yet

    ui->tblCreateTable->setHorizontalHeaderLabels(titles);
}

void CreateTable::generateInitialRows(){
    for(int x=0;x<numberOfRows;x++){
        addNewRow(x);}
}

CreateTable::~CreateTable()
{
    delete ui;
}

//will be called on clicking the cross button
void CreateTable::reject(){
        qDebug()<<"function exec reject()";
        //code to clear lists and delete references when the CreateTable window is closed
        leNameList.clear();
        leCheckList.clear();
        leDefaultList.clear();
        cbTypeList.clear();
        cbxPrimaryList.clear();
        cbxUniqueList.clear();
        cbxNullList.clear();
        cbxAutoIList.clear();

        QDialog::reject();
}


void CreateTable::on_btnAddRow_clicked()
{
    numberOfRows++;
    ui->tblCreateTable->setRowCount(numberOfRows);
    addNewRow(numberOfRows-1);
}

void CreateTable::addNewRow(int rowNumber){
    //making and adding new row
    //setting lineedit for NAME
    QLineEdit *leName = new QLineEdit(this);
    //setting combobox for TYPE
    QComboBox *cbType = new QComboBox(this);
    QStringList types;
    types.push_back("INTEGER");
    types.push_back("TEXT");
    types.push_back("BLOB");
    types.push_back("REAL");
    //types.push_back("NUMERIC");
    cbType->addItems(types);
    //setting lineedit for Check
    QLineEdit *leCheck = new QLineEdit(this);
    //setting lineedit for DEFAULT VALUE
    QLineEdit *leDefault = new QLineEdit(this);
    //setting check box for NOT NULL
    QCheckBox *cbxNull = new QCheckBox(this);
    QWidget *cbxNullWidget = new QWidget();
    QHBoxLayout *cbxNullLayout = new QHBoxLayout(cbxNullWidget);
    cbxNullLayout->addWidget(cbxNull);
    cbxNullLayout->setAlignment(Qt::AlignCenter);
    cbxNullLayout->setContentsMargins(0,0,0,0);
    cbxNullWidget->setLayout(cbxNullLayout);

    //setting check box for PRIMARY
    QCheckBox *cbxPrimary = new QCheckBox(this);
    cbxPrimary->setCheckable(true);
    cbxPrimary->setObjectName(""+rowNumber);
    QWidget *cbxPrimaryWidget = new QWidget();
    QHBoxLayout *cbxPrimaryLayout = new QHBoxLayout(cbxPrimaryWidget);
    cbxPrimaryLayout->addWidget(cbxPrimary);
    cbxPrimaryLayout->setAlignment(Qt::AlignCenter);
    cbxPrimaryLayout->setContentsMargins(0,0,0,0);
    cbxPrimaryWidget->setLayout(cbxPrimaryLayout);
    connect(cbxPrimary, SIGNAL(stateChanged(int)), this, SLOT(primaryStateChanged(int)));


    //setting check box for AUTOINCREMENT
    QCheckBox *cbxAutoI = new QCheckBox(this);
    cbxAutoI->setCheckable(true);
    QWidget *cbxAutoIWidget = new QWidget();
    QHBoxLayout *cbxAutoILayout = new QHBoxLayout(cbxAutoIWidget);
    cbxAutoILayout->addWidget(cbxAutoI);
    cbxAutoILayout->setAlignment(Qt::AlignCenter);
    cbxAutoILayout->setContentsMargins(0,0,0,0);
    cbxAutoIWidget->setLayout(cbxAutoILayout);
    connect(cbxAutoI, SIGNAL(stateChanged(int)), this, SLOT(aIStateChanged(int)));

    //setting check box for UNIQUE
    QCheckBox *cbxUnique = new QCheckBox(this);
    QWidget *cbxUniqueWidget = new QWidget();
    QHBoxLayout *cbxUniqueLayout = new QHBoxLayout(cbxUniqueWidget);
    cbxUniqueLayout->addWidget(cbxUnique);
    cbxUniqueLayout->setAlignment(Qt::AlignCenter);
    cbxUniqueLayout->setContentsMargins(0,0,0,0);
    cbxUniqueWidget->setLayout(cbxUniqueLayout);
    ui->tblCreateTable->setCellWidget(rowNumber,0,leName);
    ui->tblCreateTable->setCellWidget(rowNumber,1,cbType);
    ui->tblCreateTable->setCellWidget(rowNumber,2,leCheck);
    ui->tblCreateTable->setCellWidget(rowNumber,3,leDefault);
    ui->tblCreateTable->setCellWidget(rowNumber,4,cbxNullWidget);
    ui->tblCreateTable->setCellWidget(rowNumber,5,cbxPrimaryWidget);
    ui->tblCreateTable->setCellWidget(rowNumber,6,cbxUniqueWidget);
    ui->tblCreateTable->setCellWidget(rowNumber,7,cbxAutoIWidget);

    //pushing references to list
    leNameList.push_back(leName);
    leCheckList.push_back(leCheck);
    leDefaultList.push_back(leDefault);
    cbTypeList.push_back(cbType);
    cbxPrimaryList.push_back(cbxPrimary);
    cbxUniqueList.push_back(cbxUnique);
    cbxNullList.push_back(cbxNull);
    cbxAutoIList.push_back(cbxAutoI);

    qDebug()<<"Value of variable numberOfRows after adding: "<<numberOfRows;
}

void CreateTable::on_btnDeleteRow_clicked()
{
    if(numberOfRows>0){
    ui->tblCreateTable->setRowCount(--numberOfRows);
    delete leNameList.takeLast();
    delete leCheckList.takeLast();
    delete leDefaultList.takeLast();
    delete cbTypeList.takeLast();
    delete cbxNullList.takeLast();
    delete cbxAutoIList.takeLast();
    delete cbxPrimaryList.takeLast();
    delete cbxUniqueList.takeLast();
    }
}


void CreateTable::openDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(gLocalDbLocation);
    if(db.open()){
    qDebug()<<"Database opened by openDatabase function";
    }else{
        qDebug()<<"Database FAILED to opened by openDatabase() function";
        }
}
void CreateTable::on_btnCreateTableGenerateSql_clicked()
{
    qDebug()<<"func exec on_btnCreateTableGenerateSql_clicked()";

    int rowsCount=0;
    bool someAutoIChecked = false;
    bool somePrimaryChecked = false;

    //UNCOMMENT FOLLOWING FOR DEBUGGING PURPOSES

    //qDebug()<<leNameList.value(2)->text();
    //qDebug()<<leCheckList.value(2)->text();
    //qDebug()<<leDefaultList.value(2)->text();
    //qDebug()<<cbTypeList.value(2)->currentText();
    //qDebug()<<cbxNullList.value(2)->isChecked();
    //qDebug()<<cbxPrimaryList.value(2)->isChecked();
    //qDebug()<<cbxUniqueList.value(2)->isChecked();
    //qDebug()<<cbxAutoIList.value(2)->isChecked();

    for(int i=0;i<leNameList.count();i++){
        if(leNameList.value(i)->text().trimmed()!=""){
            rowsCount++;
        }else break;
    }
    qDebug()<<"rowsCount: "<<rowsCount;

    QString generatedSql;

    if(rowsCount>0) {
        generatedSql = "CREATE TABLE " + tableName + "\n(";
    }

    //VALIDATION

    //checking for duplication in field name
    for(int i=0;i<rowsCount;i++){
         for(int k=0;k<rowsCount;k++){
             if(i!=k){
                 if(leNameList.value(k)->text().toUpper() == leNameList.value(i)->text().toUpper()){
                     QString message = "Duplicate column name: " + leNameList.value(i)->text();
                     QMessageBox::warning(this, tr("Error"),message,QMessageBox::Ok);
                     return;
                 }
             }
         }
    }

    //generating sqlStatement out of the form
    qDebug()<<"rowsCount= "<<rowsCount;
    for(int i=0;i<rowsCount;i++){
        generatedSql+="\n";
        generatedSql+=leNameList.value(i)->text();
        generatedSql+=" ";
        generatedSql+=cbTypeList.value(i)->currentText();
        if(cbxNullList.value(i)->isChecked()) generatedSql+=" NOT NULL";
        QString tempDefault = leDefaultList.value(i)->text().trimmed();
        if(tempDefault!="") generatedSql = generatedSql + " DEFAULT '" + tempDefault + "'";
        QString tempCheck = leCheckList.value(i)->text().trimmed();
        if(tempCheck!="") generatedSql = generatedSql + " CHECK(" + tempCheck + ")";
        if(cbxAutoIList.value(i)->isChecked()) generatedSql+=" PRIMARY KEY AUTOINCREMENT";
        if(cbxUniqueList.value(i)->isChecked()) generatedSql+=" UNIQUE";
        //adding comma if the row is not last
        if(i!=rowsCount-1)generatedSql+=",";


    }

    for(int i=0;i<rowsCount;i++){
        if(cbxAutoIList.value(i)->isChecked()) someAutoIChecked = true;

        if(cbxPrimaryList.value(i)->isChecked()) somePrimaryChecked = true;
    }

    if(!someAutoIChecked){
        if(somePrimaryChecked){
        generatedSql += ",\nPRIMARY KEY(";
        //bool run for comma implementation
        bool run = false;
        for(int i=0;i<rowsCount;i++){
            if(cbxPrimaryList.value(i)->isChecked()){
                if(run)generatedSql+=",";
                generatedSql+=leNameList.value(i)->text();
                run = true;
            }
        }
        generatedSql += ")";
    }}

    if(rowsCount>0) {
    generatedSql+="\n);";}

    ui->txbrCreateTableSql->setText(generatedSql);

    if(ui->txbrCreateTableSql->toPlainText().trimmed()!=""){
        ui->btnExecute->setEnabled(true);
        ui->btnCopyToteSqlInput->setEnabled(true);
    }else{
        ui->btnExecute->setEnabled(false);
        ui->btnCopyToteSqlInput->setEnabled(false);
    }
}

void CreateTable::aIStateChanged(int cbxIndex){
qDebug()<<"function exec void primaryOrAIStateChanged(int)";

fromaIStateChanged = true;


if(!fromprimaryStateChanged){

qDebug()<<"incoming index: "<<cbxIndex;

bool isSame=false;

if(indexOfAI==-1){
for(int i=0;i<numberOfRows;i++){
    if(cbxAutoIList.value(i)->isChecked()){
        for(int k=0;k<numberOfRows;k++){
        cbxPrimaryList.value(k)->setAutoExclusive(false);
        cbxPrimaryList.value(k)->setChecked(false);
        cbxPrimaryList.value(k)->setAutoExclusive(true);
        }
        cbxPrimaryList.value(i)->setAutoExclusive(false);
        cbxPrimaryList.value(i)->setChecked(true);
        cbxPrimaryList.value(i)->setAutoExclusive(true);
        cbTypeList.value(i)->setCurrentIndex(0);
        indexOfAI = i;
        break;
}
}
}else{
    isSame=true;
    for(int i=0;i<numberOfRows;i++){
        if(cbxAutoIList.value(i)->isChecked()){
            isSame = false;
            qDebug()<<"bool isSame false";
            //indexOfAI=i;
            break;
        }
}

    if(isSame) qDebug()<<"bool isSame true";
    if(isSame) indexOfAI=-1;

    if(!isSame){
        cbxAutoIList.value(indexOfAI)->setAutoExclusive(false);
        cbxAutoIList.value(indexOfAI)->setChecked(false);
        cbxAutoIList.value(indexOfAI)->setAutoExclusive(true);
        for(int i=0;i<numberOfRows;i++){
            if(cbxAutoIList.value(i)->isChecked()){
                for(int k=0;k<numberOfRows;k++){
                cbxPrimaryList.value(k)->setAutoExclusive(false);
                cbxPrimaryList.value(k)->setChecked(false);
                cbxPrimaryList.value(k)->setAutoExclusive(true);
                }
                cbxPrimaryList.value(i)->setAutoExclusive(false);
                cbxPrimaryList.value(i)->setChecked(true);
                cbxPrimaryList.value(i)->setAutoExclusive(true);
                cbTypeList.value(i)->setCurrentIndex(0);
                indexOfAI = i;
                break;
        }
        }
    }
}
}
fromaIStateChanged = false;
}

//SLOT
void CreateTable::primaryStateChanged(int cbxIndex){
    qDebug()<<"function exec primaryStateChanged(int cbxIndex)";
    fromprimaryStateChanged = true;


    if(!fromaIStateChanged){
    for(int i=0;i<numberOfRows;i++){
        //if(i!=indexOfAI){
            qDebug()<<"i ="<<i<<"indexOfAI: "<<indexOfAI;
        if(cbxAutoIList.value(i)->isChecked()){
            cbxAutoIList.value(i)->setAutoExclusive(false);
            cbxAutoIList.value(i)->setChecked(false);
            cbxAutoIList.value(i)->setAutoExclusive(true);
    }
}
}
    fromprimaryStateChanged = false;
}

//SLOT
void CreateTable::on_btnExecute_clicked()
{
    qDebug()<<"function exec on_btnExecute_clicked()";
    QString execStatement = ui->txbrCreateTableSql->toPlainText().trimmed();
    createTableExecuteSignal(execStatement,tableName);
    this->close();
}

//SLOT
void CreateTable::on_btnCopyToteSqlInput_clicked()
{
    qDebug()<<"function exec on_btnCopyToteSqlInput_clicked()";
    QString execStatement = ui->txbrCreateTableSql->toPlainText().trimmed();
    createTableCopyStatementSignal(execStatement);
    this->close();
}

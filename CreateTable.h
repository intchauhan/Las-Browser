#ifndef CREATETABLE_H
#define CREATETABLE_H

#include <QDialog>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

namespace Ui {
class CreateTable;
}

class CreateTable : public QDialog
{
    Q_OBJECT

public:
    CreateTable(QWidget *parent = 0, QString tableName="", int numberOfRows=1);
    ~CreateTable();
    void CreateTable::reject();
    void CreateTable::generateTableLayout();
    void CreateTable::generateInitialRows();
    void CreateTable::addNewRow(int rowNumber);
    void CreateTable::openDatabase();


    QList<QLineEdit*> leNameList;
    QList<QLineEdit*> leCheckList;
    QList<QLineEdit*> leDefaultList;
    QList<QComboBox*> cbTypeList;
    QList<QCheckBox*> cbxPrimaryList;
    QList<QCheckBox*> cbxUniqueList;
    QList<QCheckBox*> cbxNullList;
    QList<QCheckBox*> cbxAutoIList;



    int numberOfRows = 1;
    QString tableName;
    int indexOfAI = -1;


    bool fromprimaryStateChanged = false;
    bool fromaIStateChanged = false;

signals:

    void createTableCopyStatementSignal(QString statement);
    void createTableExecuteSignal(QString statement, QString tableName);


private slots:
    void on_btnAddRow_clicked();

    void on_btnDeleteRow_clicked();

    void on_btnCreateTableGenerateSql_clicked();

    void aIStateChanged(int cbxIndex);

    void primaryStateChanged(int cbxIndex);

    void on_btnExecute_clicked();

    void on_btnCopyToteSqlInput_clicked();

private:
    Ui::CreateTable *ui;
};

#endif // CREATETABLE_H

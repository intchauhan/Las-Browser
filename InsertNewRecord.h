#ifndef INSERTNEWRECORD_H
#define INSERTNEWRECORD_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class InsertNewRecord;
}

class InsertNewRecord : public QDialog
{
    Q_OBJECT

signals:
    void insertNewRecordRefreshMainWindowSignal();

public:
    explicit InsertNewRecord(QWidget *parent = 0, QString tableName=0,QSqlDatabase *db=0);
    ~InsertNewRecord();
    void InsertNewRecord::reject();
    void InsertNewRecord::generateRow();
    void InsertNewRecord::deleteLastRow();
    void InsertNewRecord::generateInitialTableLayout();
    void InsertNewRecord::openDatabase();
    void InsertNewRecord::closeDatabase();
    void InsertNewRecord::closingCeremony();
    void InsertNewRecord::executeGeneratedStatements();

private slots:
    void on_cbNumberOfRecords_currentTextChanged(const QString &arg1);

    void on_btnAddRecords_clicked();

private:
    Ui::InsertNewRecord *ui;
};

#endif // INSERTNEWRECORD_H

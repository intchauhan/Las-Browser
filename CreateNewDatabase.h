#ifndef CREATENEWDATABASE_H
#define CREATENEWDATABASE_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

namespace Ui {
class CreateNewDatabase;
}

class CreateNewDatabase : public QDialog
{
    Q_OBJECT

signals:
    void createNewDatabaseDoneSignal();

public:
    explicit CreateNewDatabase(QWidget *parent = 0);
    ~CreateNewDatabase();
    void CreateNewDatabase::generateTableLayout();
    void CreateNewDatabase::generateInitialRows();
    void CreateNewDatabase::reject();//do we need this?
    void CreateNewDatabase::openDatabase();
    void CreateNewDatabase::addNewRow(int rowNumber);

    QString tempDbLocation;
    QString tempDbName;
    QString tableName;

    QList<QLineEdit*> leNameList;
    QList<QLineEdit*> leCheckList;
    QList<QLineEdit*> leDefaultList;
    QList<QComboBox*> cbTypeList;
    QList<QCheckBox*> cbxPrimaryList;
    QList<QCheckBox*> cbxUniqueList;
    QList<QCheckBox*> cbxNullList;
    QList<QCheckBox*> cbxAutoIList;

    int numberOfRows;
    int indexOfAI;

    bool fromprimaryStateChanged;
    bool fromaIStateChanged;

private slots:
    void aIStateChanged(int cbxIndex);

    void primaryStateChanged(int cbxIndex);

    void on_btnBrowseNewDb_clicked();

    void on_btnExecute_clicked();

    void on_btnAddRow_clicked();

    void on_btnDeleteRow_clicked();

private:
    Ui::CreateNewDatabase *ui;
};

#endif // CREATENEWDATABASE_H

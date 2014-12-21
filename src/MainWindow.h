#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CreateTable.h"
#include "InsertNewRecord.h"
#include <QDebug>
#include <QtSql>
#include <QComboBox>
#include <QStatusBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openDatabase();
    void closeDatabase();
    void refreshView();
    void clear();
    QString executeQuery(QString statement);
    void MainWindow::selectStatementExe();
    void MainWindow::nonSelectStatementExe();
    void MainWindow::tbSqlOutputSuccess(QString tempStatement);
    void MainWindow::tableInfo();
    void MainWindow::tbStructureSetup();
    void MainWindow::tblStructureRefresh();
    void MainWindow::cbTableNamesRefresh();
    void MainWindow::tblBrowseRefresh(QString tableName);
    void MainWindow::disableView();
    void MainWindow::enableView();
    void MainWindow::trwDatabaseExplorerRefresh();

private slots:
    void createNewDatabaseDoneSlot();

    void openDatabaseUiNonAndroidDbSelectedSlot();

    void on_actionExit_triggered();

    void on_btnExecute_clicked();

    void on_cbTableNames_currentIndexChanged(const QString &tableName);

    void on_btnCreateTable_clicked();

    void createTableCopyStatementSlot(QString statement);

    void createTableExecuteSlot(QString statement, QString tableName);

    void insertNewRecordRefreshMainWindowSlot();

    void setDeviceNumberIdSlot(bool goId);

    void setPackageNameDoneSlot(bool);

    void setDatabaseNameDoneSlot();

    void deleteTableFromOptionsDoneSlot();

    void cbxRowStateChanged(int);

    void on_actionRefresh_triggered();

    void on_btnDeleteRecord_clicked();

    void on_btninsertNewRecord_clicked();

    void on_actionWrite_Changes_triggered();

    void on_actionDiscard_all_Unpushed_Changes_and_Resync_triggered();

    void on_actionBackupDb_triggered();

    void on_actionCreate_Table_triggered();

    void createTableFromOptionsOpenWindowSlot(QString,QString);

    void on_actionDelete_Table_triggered();

    void on_actionAbout_triggered();

    void on_actionWebsite_triggered();

    void on_actionDeveloper_triggered();

    void on_actionReport_Bug_triggered();

    void on_actionCheck_for_updates_triggered();

    void on_actionClose_Database_triggered();

    void on_actionOpen_Database_triggered();

    void on_actionChange_SDK_Location_triggered();

    void on_actionNew_Database_triggered();

    void structureDefinitionClickedSlot();

    void structureInsertClickedSlot();

    void structureDropClickedSlot();

    void structureBrowseClickSlot();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

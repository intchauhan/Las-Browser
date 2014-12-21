#ifndef SETDATABASENAME_H
#define SETDATABASENAME_H

#include <QDialog>

namespace Ui {
class SetDatabaseName;
}

class SetDatabaseName : public QDialog
{
    Q_OBJECT

signals:
    void setDatabaseNameDoneSignal();



public:
    explicit SetDatabaseName(QWidget *parent = 0);
    ~SetDatabaseName();
    void showDbs();
    void SetDatabaseName::doFinalJob(QString selectedDb);

private slots:
    void on_btnListDbOne_clicked();

    void on_btnListDbTwo_clicked();

    void on_lwDatabases_doubleClicked(const QModelIndex &index);

    void on_btnNext_clicked();

private:
    Ui::SetDatabaseName *ui;
};

#endif // SETDATABASENAME_H

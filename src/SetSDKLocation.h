#ifndef SETSDKLOCATION_H
#define SETSDKLOCATION_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QLineEdit>
#include "MainWindow.h"

namespace Ui {
class SetSDKLocation;
}

class SetSDKLocation : public QDialog
{
    Q_OBJECT

public:
    explicit SetSDKLocation(QWidget *parent = 0);
    ~SetSDKLocation();
    MainWindow *mainWindow;
    void SetSDKLocation::Write(QString filename,QString data);

    void SetSDKLocation::Read(QString filename,QString data);

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::SetSDKLocation *ui;
};

#endif // SETSDKLOCATION_H

#ifndef SETDEVICENUMBER_H
#define SETDEVICENUMBER_H

#include <QDialog>
#include <QListWidgetItem>
#include "Globals.h"

namespace Ui {
class SetDeviceNumber;
}

class SetDeviceNumber : public QDialog
{
    Q_OBJECT


signals:
    void setDeviceNumberIdSignal(bool gotId);

public:
    explicit SetDeviceNumber(QWidget *parent = 0);
    ~SetDeviceNumber();
    void doJob();
    QStringList getAllConnectedDevicesEmulatorsList();
    void displayListOnlwDeviceEmulatorList(QStringList toSetInTable);

private slots:
    void on_btnRefresh_clicked();

    void on_btnDeepRefresh_clicked();

    void on_lwDeviceEmulatorList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SetDeviceNumber *ui;
};

#endif // SETDEVICENUMBER_H

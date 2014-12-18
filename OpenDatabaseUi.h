#ifndef OPENDATABASEUI_H
#define OPENDATABASEUI_H

#include <QDialog>

namespace Ui {
class OpenDatabaseUi;
}

class OpenDatabaseUi : public QDialog
{
    Q_OBJECT

signals:
    void openDatabaseUiNonAndroidDbSelectedSignal();

public:
    explicit OpenDatabaseUi(QWidget *parent = 0);
    ~OpenDatabaseUi();
    void OpenDatabaseUi::closeEvent(QCloseEvent *event);

private slots:
    void on_btnAndroidDb_clicked();

    void on_btnLocalDb_clicked();

    void on_btnCancel_clicked();

private:
    Ui::OpenDatabaseUi *ui;
};

#endif // OPENDATABASEUI_H

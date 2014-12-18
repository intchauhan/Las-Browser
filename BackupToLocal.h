#ifndef BACKUPTOLOCAL_H
#define BACKUPTOLOCAL_H

#include <QDialog>

namespace Ui {
class BackupToLocal;
}

class BackupToLocal : public QDialog
{
    Q_OBJECT

public:
    explicit BackupToLocal(QWidget *parent = 0);
    ~BackupToLocal();
    QString location;

private slots:
    void on_btnBrowse_clicked();

    void on_btnBackup_clicked();

    void on_leDestination_textChanged(const QString &arg1);

private:
    Ui::BackupToLocal *ui;
};

#endif // BACKUPTOLOCAL_H

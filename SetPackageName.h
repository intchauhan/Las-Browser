#ifndef SETPACKAGENAME_H
#define SETPACKAGENAME_H

#include <QDialog>

namespace Ui {
class SetPackageName;
}

class SetPackageName : public QDialog
{
    Q_OBJECT

signals:
    void setPackageNameDoneSignal(bool donePackage);


public:
    explicit SetPackageName(QWidget *parent = 0);
    ~SetPackageName();
    void SetPackageName::showPackages();
    void SetPackageName::doFinalJob(QString packageName);

private slots:
    void on_btnNext_clicked();

    void on_btnListPkgOne_clicked();

    void on_btnListPkgTwo_clicked();

    void on_lwPackages_doubleClicked(const QModelIndex &index);

private:
    Ui::SetPackageName *ui;
};

#endif // SETPACKAGENAME_H

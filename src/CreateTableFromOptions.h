#ifndef CREATETABLEFROMOPTIONS_H
#define CREATETABLEFROMOPTIONS_H

#include <QDialog>

namespace Ui {
class CreateTableFromOptions;
}

class CreateTableFromOptions : public QDialog
{
    Q_OBJECT

signals:
    void createTableFromOptionsOpenWindowSignal(QString,QString);

public:
    explicit CreateTableFromOptions(QWidget *parent = 0);
    ~CreateTableFromOptions();
    void CreateTableFromOptions::openDatabase();

private slots:
    void on_btnGoCreateTable_clicked();

private:
    Ui::CreateTableFromOptions *ui;
};

#endif // CREATETABLEFROMOPTIONS_H

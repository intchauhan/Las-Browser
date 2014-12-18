#ifndef DELETETABLEFROMOPTIONS_H
#define DELETETABLEFROMOPTIONS_H

#include <QDialog>

namespace Ui {
class DeleteTableFromOptions;
}

class DeleteTableFromOptions : public QDialog
{
    Q_OBJECT

signals:
    void deleteTableFromOptionsDoneSignal();

public:
    explicit DeleteTableFromOptions(QWidget *parent = 0);
    ~DeleteTableFromOptions();
    void populateTable();

private slots:
    void on_btnDeleteTable_clicked();

    void on_lwTableNames_currentTextChanged(const QString &currentText);

private:
    Ui::DeleteTableFromOptions *ui;
};

#endif // DELETETABLEFROMOPTIONS_H

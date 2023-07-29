#ifndef DIALOGCHECKCORRUPTION_H
#define DIALOGCHECKCORRUPTION_H

#include <QDialog>

#include "datastructdefines.h"
#include "databasesqlite.h"
#include "tablemodelcorruptedfils.h"

namespace Ui {
class DialogCheckCorruption;
}

class DialogCheckCorruption : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheckCorruption(QWidget *parent = nullptr);
    ~DialogCheckCorruption();
    void add_table_files_model(table_model *p);

private slots:
    void on_pushButtonCheck_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::DialogCheckCorruption *ui;
    QVector<corrupted_file_t> corrupted_files;
    TableModelCorruptedFils model;
    table_model *p_table_files_model;
};

#endif // DIALOGCHECKCORRUPTION_H

#ifndef DIALOGIMPORTFILES_H
#define DIALOGIMPORTFILES_H

#include <QDialog>
#include <QString>
#include <QVector>

#include "datastructdefines.h"
#include "databasesqlite.h"
#include "tablemodelnewfiles.h"

namespace Ui {
class DialogImportFiles;
}

class DialogImportFiles : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImportFiles(QWidget *parent = nullptr);
    ~DialogImportFiles();
    void add_table_files_model(table_model *p);

private slots:
    void on_pushButtonSelectFiles_clicked();

    void on_pushButtonHash_clicked();

    void on_pushButtonCommit_clicked();

    void on_pushButtonMetadata_clicked();

private:
    Ui::DialogImportFiles *ui;
    TableModelNewFiles model;
    QVector<new_file_info_t> file_metadatas;
    bool hash_finished;
    table_model *p_table_files_model;
};

#endif // DIALOGIMPORTFILES_H

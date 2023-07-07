#ifndef DIALOGCOPYFILESIN_H
#define DIALOGCOPYFILESIN_H

#include <QDialog>
#include <QString>
#include <QVector>

#include "datastructdefines.h"
#include "newfilelisttablemodel.h"

namespace Ui {
class DialogCopyFilesIn;
}

class DialogCopyFilesIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCopyFilesIn(QWidget *parent = nullptr);
    ~DialogCopyFilesIn();

private slots:
    void on_pushButtonSelectFiles_clicked();

    void on_pushButtonHash_clicked();

    void on_pushButtonCommit_clicked();

private:
    Ui::DialogCopyFilesIn *ui;
    NewFileListTableModel model;
    QVector<new_file_info_t> file_metadatas;
    bool hash_finished;
};

#endif // DIALOGCOPYFILESIN_H

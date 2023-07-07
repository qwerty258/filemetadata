#ifndef DIALOGCHECKCORRUPTION_H
#define DIALOGCHECKCORRUPTION_H

#include <QDialog>

#include "datastructdefines.h"
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

private slots:
    void on_pushButtonCheck_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::DialogCheckCorruption *ui;
    QVector<corrupted_file_t> corrupted_files;
    TableModelCorruptedFils model;
};

#endif // DIALOGCHECKCORRUPTION_H

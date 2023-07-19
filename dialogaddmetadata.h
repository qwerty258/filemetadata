#ifndef DIALOGADDMETADATA_H
#define DIALOGADDMETADATA_H

#include <QDialog>

#include "datastructdefines.h"

namespace Ui {
class DialogAddMetadata;
}

class DialogAddMetadata : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddMetadata(QWidget *parent = nullptr);
    ~DialogAddMetadata();

    void add_abs_file_path(QString &path);

    void add_file_name(QString &name);

    void add_metadata_reference(metadata_t *p);

    void auto_get_metadata();

private slots:
    void on_pushButtonOK_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogAddMetadata *ui;
    metadata_t *metadata;
    QString abs_file_path;
    QString file_name;
};

#endif // DIALOGADDMETADATA_H

#ifndef DIALOGEDITMETADATASERIALS_H
#define DIALOGEDITMETADATASERIALS_H

#include <QDialog>

#include "databasesqlite.h"

namespace Ui {
class DialogEditMetadataSerials;
}

class DialogEditMetadataSerials : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditMetadataSerials(QWidget *parent = nullptr);
    ~DialogEditMetadataSerials();

private slots:
    void on_tableViewEditSerials_customContextMenuRequested(const QPoint &pos);
    void on_tableViewEditSerials_customContextMenuRequested_action_delete();

private:
    Ui::DialogEditMetadataSerials *ui;
    table_model *p_table_serials_model;
};

#endif // DIALOGEDITMETADATASERIALS_H

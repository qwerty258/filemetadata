#ifndef DIALOGVIEWTAGS_H
#define DIALOGVIEWTAGS_H

#include <QDialog>

#include "databasesqlite.h"

namespace Ui {
class Dialogviewtags;
}

class Dialogviewtags : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogviewtags(QWidget *parent = nullptr);
    ~Dialogviewtags();
    void add_table_model_files(table_model *p);

private slots:
    void on_push_button_filter_clicked();

    void on_push_button_clear_clicked();

private:
    Ui::Dialogviewtags *ui;
    table_model *p_table_files_model;
    table_model *p_table_tags_model;
};

#endif // DIALOGVIEWTAGS_H

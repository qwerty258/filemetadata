#ifndef DIALOGADDTAG_H
#define DIALOGADDTAG_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class DialogAddTag;
}

class DialogAddTag : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddTag(QWidget *parent = nullptr);
    void add_table_files_table_view(QTableView *p);
    ~DialogAddTag();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogAddTag *ui;
    QTableView *p_table_files_table_view = nullptr;
};

#endif // DIALOGADDTAG_H

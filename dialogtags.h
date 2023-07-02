#ifndef DIALOGTAGS_H
#define DIALOGTAGS_H

#include <QDialog>

namespace Ui {
class DialogTags;
}

class DialogTags : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTags(QWidget *parent = nullptr);
    ~DialogTags();

private slots:
    void on_pushButtonAddTag_clicked();

    void on_tableViewTags_customContextMenuRequested(const QPoint &pos);

    void on_tableViewTags_customContextMenuRequested_action_delete();

private:
    Ui::DialogTags *ui;
};

#endif // DIALOGTAGS_H

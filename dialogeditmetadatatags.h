#ifndef DIALOGEDITMETADATATAGS_H
#define DIALOGEDITMETADATATAGS_H

#include <QDialog>

namespace Ui {
class DialogEditMetadataTags;
}

class DialogEditMetadataTags : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditMetadataTags(QWidget *parent = nullptr);
    ~DialogEditMetadataTags();

private slots:
    void on_pushButtonAddTag_clicked();

    void on_tableViewEditTags_customContextMenuRequested(const QPoint &pos);

    void on_tableViewEditTags_customContextMenuRequested_action_delete();

private:
    Ui::DialogEditMetadataTags *ui;
};

#endif // DIALOGEDITMETADATATAGS_H

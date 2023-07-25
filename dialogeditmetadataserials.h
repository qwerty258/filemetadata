#ifndef DIALOGEDITMETADATASERIALS_H
#define DIALOGEDITMETADATASERIALS_H

#include <QDialog>

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
};

#endif // DIALOGEDITMETADATASERIALS_H

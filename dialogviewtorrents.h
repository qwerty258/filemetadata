#ifndef DIALOGVIEWTORRENTS_H
#define DIALOGVIEWTORRENTS_H

#include <QDialog>

#include "databasesqlite.h"

namespace Ui {
class DialogViewTorrents;
}

class DialogViewTorrents : public QDialog
{
    Q_OBJECT

public:
    explicit DialogViewTorrents(QWidget *parent = nullptr);
    ~DialogViewTorrents();

private slots:
    void on_push_button_torrents_search_clicked();

    void on_push_button_torrents_clear_clicked();

    void on_push_button_files_in_torrents_search_clicked();

    void on_push_button_files_in_torrents_clear_clicked();

private:
    Ui::DialogViewTorrents *ui;
    table_model *p_table_torrents_model;
    table_model *p_table_files_in_torrent_model;
};

#endif // DIALOGVIEWTORRENTS_H

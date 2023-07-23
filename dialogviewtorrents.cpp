#include "dialogviewtorrents.h"
#include "ui_dialogviewtorrents.h"
#include "databasesqlite.h"

DialogViewTorrents::DialogViewTorrents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogViewTorrents)
{
    ui->setupUi(this);
    database_table_torrents_create_model();
    database_table_files_in_torrent_create_model();

    database_table_torrents_add_model_to_view(ui->tableViewTorrents);
    database_table_files_in_torrent_add_model_to_view(ui->tableViewFilesInTorrent);
}

DialogViewTorrents::~DialogViewTorrents()
{
    delete ui;
    database_table_torrents_delete_model();
    database_table_files_in_torrent_create_model();
}

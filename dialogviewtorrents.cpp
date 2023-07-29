#include "dialogviewtorrents.h"
#include "ui_dialogviewtorrents.h"

DialogViewTorrents::DialogViewTorrents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogViewTorrents)
{
    ui->setupUi(this);

    p_table_torrents_model = new table_model("torrents");
    p_table_files_in_torrent_model = new table_model("files_in_torrent");

    ui->tableViewTorrents->setModel(p_table_torrents_model->get_table_model());
    ui->tableViewFilesInTorrent->setModel(p_table_files_in_torrent_model->get_table_model());

    ui->tableViewTorrents->resizeColumnsToContents();
    ui->tableViewFilesInTorrent->resizeColumnsToContents();
}

DialogViewTorrents::~DialogViewTorrents()
{
    delete ui;
    delete p_table_torrents_model;
    delete p_table_files_in_torrent_model;
}

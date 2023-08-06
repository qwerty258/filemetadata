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

void DialogViewTorrents::on_push_button_torrents_search_clicked()
{
    QString tmp = ui->line_edit_torrents_search_term->text().trimmed().remove('\r').remove('\n');
    p_table_torrents_model->table_torrents_match(tmp);
    p_table_torrents_model->table_select();
}

void DialogViewTorrents::on_push_button_torrents_clear_clicked()
{
    p_table_torrents_model->clear_match();
    ui->line_edit_torrents_search_term->clear();
}

void DialogViewTorrents::on_push_button_files_in_torrents_search_clicked()
{
    QString tmp = ui->line_edit_files_in_torrents_search_term->text().trimmed().remove('\r').remove('\n');
    p_table_files_in_torrent_model->table_files_in_torrent_match(tmp);
    p_table_files_in_torrent_model->table_select();
}

void DialogViewTorrents::on_push_button_files_in_torrents_clear_clicked()
{
    p_table_files_in_torrent_model->clear_match();
    ui->line_edit_files_in_torrents_search_term->clear();
}

#include "frametorrent.h"
#include "ui_frametorrent.h"

FrameTorrent::FrameTorrent(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTorrent)
{
    ui->setupUi(this);
    model.add_data_source(&files_in_torrent);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
}

FrameTorrent::~FrameTorrent()
{
    delete ui;
}

void FrameTorrent::set_data(torrent_metadata_t &torrent_metadata)
{
    ui->lineEditComment->setText(torrent_metadata.comment);
    ui->lineEditCreatedBy->setText(torrent_metadata.created_by);
    ui->lineEditCreationDate->setText(torrent_metadata.creation_date);
    model.begin_update_data();
    files_in_torrent = torrent_metadata.files;
    model.end_update_data();
    ui->tableView->resizeColumnsToContents();
    ui->lineEditInfoHashV1->setText(torrent_metadata.info_hash_v1);
    ui->lineEditInfoHashV2->setText(torrent_metadata.info_hash_v2);
    ui->lineEditName->setText(torrent_metadata.name);
    ui->lineEditPieceLength->setText(QString::number(torrent_metadata.piece_length));
    ui->lineEditPieces->setText(QString::number(torrent_metadata.pieces));
}

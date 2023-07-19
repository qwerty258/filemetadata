#include <QFileInfo>
#include <QDebug>
#include <QTimeZone>

#include <libtorrent/libtorrent.hpp>

#include "dialogaddmetadata.h"
#include "ui_dialogaddmetadata.h"
#include "frametorrent.h"

DialogAddMetadata::DialogAddMetadata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddMetadata)
{
    ui->setupUi(this);
    ui->comboBox->insertItem(METADATA_TYPE_BOOK, tr("Book"), QVariant(METADATA_TYPE_BOOK));
    ui->comboBox->insertItem(METADATA_TYPE_SERIAL, tr("Serial"), QVariant(METADATA_TYPE_SERIAL));
    ui->comboBox->insertItem(METADATA_TYPE_TORRENT, tr("Torrent"), QVariant(METADATA_TYPE_TORRENT));
    ui->comboBox->insertItem(METADATA_TYPE_MAX, tr("None"), QVariant(METADATA_TYPE_MAX));
    ui->comboBox->setCurrentIndex(METADATA_TYPE_MAX);
}

DialogAddMetadata::~DialogAddMetadata()
{
    delete ui;
}

void DialogAddMetadata::add_abs_file_path(QString &path)
{
    abs_file_path = path;
}

void DialogAddMetadata::add_file_name(QString &name)
{
    file_name = name;
    ui->lineEditFileName->setText(file_name);
}

void DialogAddMetadata::auto_get_metadata()
{
    QFileInfo file_info(abs_file_path);
    QString ext = file_info.suffix().toLower();
    if ("torrent" == ext)
    {
        libtorrent::error_code error;
        libtorrent::torrent_info torrent_info(
            file_info.absoluteFilePath().toStdString(),
            error);

        if (error.failed())
            return;

        metadata->torrent.comment = QString::fromUtf8(
            torrent_info.comment().c_str(),
            torrent_info.comment().length());

        metadata->torrent.created_by = QString::fromUtf8(
            torrent_info.creator().c_str(),
            torrent_info.creator().length());

        qint64 time = torrent_info.creation_date();
        metadata->torrent.creation_date = QDateTime::fromSecsSinceEpoch(time, QTimeZone::utc());

        int num_files = torrent_info.num_files();
        metadata->torrent.files.resize(num_files);
        for (int i = 0; i < num_files; i++)
        {
            metadata->torrent.files[i].path = QString::fromUtf8(
                torrent_info.files().file_path(i).c_str(),
                torrent_info.files().file_path(i).length());
            metadata->torrent.files[i].length = torrent_info.files().file_size(i);
        }

        if (torrent_info.info_hashes().has_v1())
        {
            QByteArray bytes;
            bytes.setRawData(
                torrent_info.info_hashes().v1.data(),
                torrent_info.info_hashes().v1.size());
            metadata->torrent.info_hash_v1 = bytes.toHex().toUpper();
        }
        else
        {
            metadata->torrent.info_hash_v1.clear();
        }

        if (torrent_info.info_hashes().has_v2())
        {
            QByteArray bytes;
            bytes.setRawData(
                torrent_info.info_hashes().v2.data(),
                torrent_info.info_hashes().v2.size());
            metadata->torrent.info_hash_v2 = bytes.toHex().toUpper();
        }
        else
        {
            metadata->torrent.info_hash_v2.clear();
        }

        metadata->torrent.name = QString::fromUtf8(
            torrent_info.name().c_str(),
            torrent_info.name().length());

        metadata->torrent.piece_length = torrent_info.piece_length();

        metadata->torrent.pieces = torrent_info.num_pieces();

        ui->comboBox->setCurrentIndex(METADATA_TYPE_TORRENT);
    }
}

void DialogAddMetadata::add_metadata_reference(metadata_t *p)
{
    metadata = p;
}

void DialogAddMetadata::on_comboBox_currentIndexChanged(int index)
{
    delete ui->frame;

    switch (ui->comboBox->itemData(index).toUInt())
    {
        case METADATA_TYPE_TORRENT:
            {
                FrameTorrent* p = new FrameTorrent(this);
                p->set_data(metadata->torrent);
                ui->frame = p;
            }
            break;
        default:
            ui->frame = new QFrame(this);
            break;
    }

    ui->gridLayout->addWidget(ui->frame, 3, 0, 1, 3);
    ui->frame->show();
}

void DialogAddMetadata::on_pushButtonOK_clicked()
{
    // get data from user input
    switch (ui->comboBox->itemData(ui->comboBox->currentIndex()).toUInt())
    {
        case METADATA_TYPE_TORRENT:
            metadata->type = METADATA_TYPE_TORRENT;
            break;
        default:
            metadata->type = METADATA_TYPE_MAX;
            break;
    }
    close();
}

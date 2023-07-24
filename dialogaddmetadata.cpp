#include <QFileInfo>
#include <QDebug>
#include <QTimeZone>

#include <libtorrent/libtorrent.hpp>

#include "dialogaddmetadata.h"
#include "ui_dialogaddmetadata.h"
#include "frametorrent.h"
#include "frameserial.h"

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

void DialogAddMetadata::get_torrent_metadata()
{
    libtorrent::error_code error;
    libtorrent::torrent_info torrent_info(
        abs_file_path.toStdString(),
        error);

    if (error.failed())
        return;

    if (0 == torrent_info.comment().length())
    {
        metadata->torrent.comment.clear();
    }
    else
    {
        metadata->torrent.comment = QString::fromUtf8(
            torrent_info.comment().c_str(),
            torrent_info.comment().length());
    }

    if (0 == torrent_info.creator().length())
    {
        metadata->torrent.created_by.clear();
    }
    else
    {
        metadata->torrent.created_by = QString::fromUtf8(
            torrent_info.creator().c_str(),
            torrent_info.creator().length());
    }

    qint64 time = torrent_info.creation_date();
    if (0 == time)
    {
        metadata->torrent.creation_date.clear();
    }
    else
    {
        metadata->torrent.creation_date =
            QDateTime::fromSecsSinceEpoch(time, QTimeZone::utc()).toString(Qt::DateFormat::ISODate);
    }

    int num_files = torrent_info.num_files();
    file_in_torrent file_in_torrent_tmp;
    for (int i = 0; i < num_files; i++)
    {
        // filter out padding files
        if (std::string::npos != torrent_info.files().file_path(i).find("_____padding_file"))
            continue;

        file_in_torrent_tmp.path = QString::fromUtf8(
            torrent_info.files().file_path(i).c_str(),
            torrent_info.files().file_path(i).length());
        file_in_torrent_tmp.length = torrent_info.files().file_size(i);
        metadata->torrent.files.push_back(file_in_torrent_tmp);
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

    if (0 == torrent_info.name().length())
    {
        metadata->torrent.name.clear();
    }
    else
    {
        metadata->torrent.name = QString::fromUtf8(
            torrent_info.name().c_str(),
            torrent_info.name().length());
    }

    metadata->torrent.piece_length = torrent_info.piece_length();

    metadata->torrent.pieces = torrent_info.num_pieces();
}

void DialogAddMetadata::auto_get_metadata()
{
    QFileInfo file_info(abs_file_path);
    QString ext = file_info.suffix().toLower();
    if ("torrent" == ext)
    {
        get_torrent_metadata();
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
        case METADATA_TYPE_SERIAL:
            ui->frame = new FrameSerial(this);
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
        case METADATA_TYPE_SERIAL:
            {
                metadata->type = METADATA_TYPE_SERIAL;
                ((FrameSerial*)(ui->frame))->get_data(metadata->serial);
            }
            break;
        default:
            metadata->type = METADATA_TYPE_MAX;
            break;
    }
    close();
}

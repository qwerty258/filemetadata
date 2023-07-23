#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>

#include "dialogaddmetadata.h"
#include "dialogcopyfilesin.h"
#include "ui_dialogcopyfilesin.h"

#include "databasesqlite.h"
#include "fileoperation.h"

extern QSettings global_settings;

DialogCopyFilesIn::DialogCopyFilesIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCopyFilesIn)
{
    hash_finished = false;
    ui->setupUi(this);
    model.add_dialog_copy_file(&file_metadatas);
    ui->tableView->setModel(&model);
    ui->tableView->resizeColumnsToContents();
    ui->progressBar->setValue(0);
}

DialogCopyFilesIn::~DialogCopyFilesIn()
{
    delete ui;
}

void DialogCopyFilesIn::on_pushButtonSelectFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files");
    int total_file_count = files.size();
    if (total_file_count <= 0)
        return;

    model.begin_update_data();

    file_metadatas.resize(total_file_count);

    for (int i = 0; i < total_file_count; i++)
    {
        file_metadatas[i].full_path = files[i];
        file_metadatas[i].size = 0;
        file_metadatas[i].metadata.type = METADATA_TYPE_MAX;
    }

    model.end_update_data();
    ui->tableView->resizeColumnsToContents();
}

void DialogCopyFilesIn::on_pushButtonHash_clicked()
{
    qsizetype size = file_metadatas.size();

    if (0 == size)
        return;

    model.begin_update_data();

    // TODO: multithread sha1 calc in back ground with progress bar
    for (qsizetype i = 0; i < size; i++)
    {
        QFile f(file_metadatas[i].full_path);
        if (f.open(QFile::ReadOnly))
        {
            QCryptographicHash hash(QCryptographicHash::Sha1);
            if (hash.addData(&f))
            {
                file_metadatas[i].file_name = QFileInfo(f.fileName()).fileName();
                file_metadatas[i].sha1 = hash.result().toHex().toUpper();
                file_metadatas[i].size = f.size();
            }
        }
        ui->progressBar->setValue(i * 100.0f / size);
    }
    ui->progressBar->setValue(100);
    hash_finished = true;

    model.end_update_data();
    ui->tableView->resizeColumnsToContents();
}

void DialogCopyFilesIn::on_pushButtonMetadata_clicked()
{
    qsizetype size = file_metadatas.size();

    for (qsizetype i = 0; i < size; i++)
    {
        DialogAddMetadata add_metadata;
        add_metadata.add_abs_file_path(file_metadatas[i].full_path);
        add_metadata.add_file_name(file_metadatas[i].file_name);
        add_metadata.add_metadata_reference(&file_metadatas[i].metadata);
        add_metadata.auto_get_metadata();
        add_metadata.exec();
    }
}

void DialogCopyFilesIn::on_pushButtonCommit_clicked()
{
    if (!hash_finished)
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText(tr("Please finish hash before database commit"));
        msg.exec();
        return;
    }

    // prepare copy file path
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    qsizetype size = file_metadatas.size();

    quint64 new_file_id;

    for (qsizetype i = 0; i < size; i++)
    {
        bool found_dup = false;
        if (0 != database_table_files_search_for_sha1_dup(file_metadatas[i].sha1, &found_dup, file_metadatas[i].size))
        {
            continue;
        }
        if (found_dup)
        {
            QMessageBox msg;
            msg.setText(file_metadatas[i].full_path + " already in database");
            msg.exec();
            continue;
        }

        if (database_table_files_add_new_file_record(
                file_metadatas[i].file_name,
                file_metadatas[i].size,
                file_metadatas[i].sha1,
                new_file_id))
        {
            // qDebug() << "database_table_files_add_new_file_record";
            file_operation_new_file(
                file_metadatas[i].full_path,
                database_root_path,
                file_metadatas[i].sha1);
            // qDebug() << file_metadatas[i].metadata.type;
            switch (file_metadatas[i].metadata.type)
            {
                case METADATA_TYPE_TORRENT:
                    {
                        // qDebug() << "METADATA_TYPE_TORRENT";
                        database_table_torrents_create_model();
                        database_table_torrents_add_torrent(file_metadatas[i].metadata.torrent, new_file_id);
                        database_table_torrents_delete_model();

                        database_table_files_in_torrent_create_model();
                        database_table_files_in_torrent_add_torrent(file_metadatas[i].metadata.torrent, new_file_id);
                        database_table_files_in_torrent_delete_model();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    close();
}

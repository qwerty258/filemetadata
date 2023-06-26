#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>

#include "dialogcopyfilesin.h"
#include "ui_dialogcopyfilesin.h"

#include "databasesqlite.h"

extern QSettings global_settings;

DialogCopyFilesIn::DialogCopyFilesIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCopyFilesIn)
{
    ui->setupUi(this);
}

DialogCopyFilesIn::~DialogCopyFilesIn()
{
    delete ui;
}

void DialogCopyFilesIn::on_pushButton_clicked()
{
    files = QFileDialog::getOpenFileNames(this, "Select Files");
    int total_file_count = files.size();
    if(total_file_count <= 0)
        return;
    sha1.resize(total_file_count);
    size.resize(total_file_count);

    // prepare copy file path
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    for (int i = 0; i < total_file_count; i++)
    {
        qDebug() << files[i];
        QFile f(files[i]);
        if (f.open(QFile::ReadOnly))
        {
            // TODO: multithread sha1 calc in back ground with progress bar
            QCryptographicHash hash(QCryptographicHash::Sha1);
            if (hash.addData(&f))
            {
                sha1[i] = hash.result().toHex().toUpper();
                qDebug() << sha1[i];
            }
            size[i] = f.size();
            qDebug() <<  size[i];
            QFileInfo file_info(files[i]);
            QString filename = file_info.fileName();

            bool found_dup = false;
            if (0 != database_search_for_sha1_dup(sha1[i], &found_dup, size[i]))
            {
                continue;
            }
            if (found_dup)
            {
                QMessageBox msg;
                msg.setText(filename + " already in database");
                msg.exec();
                continue;
            }

            database_add_new_file_record(files[i], database_root_path, filename, size[i], sha1[i]);
        }
    }
}

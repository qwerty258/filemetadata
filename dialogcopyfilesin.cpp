#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QCryptographicHash>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QtConfig>

#include "dialogcopyfilesin.h"
#include "ui_dialogcopyfilesin.h"

extern QSettings global_settings;
extern QSqlTableModel *model;
extern QSqlDatabase db;

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
    QString root_path = global_settings.value("database_location", "").toString();
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

            QSqlQuery query(db);
            QString sql = "SELECT * FROM files WHERE file_sha1sum = \"" + sha1[i] + "\";";
            if(query.exec(sql))
            {
                qDebug() << query.size();
                bool found_dup = false;
                while (query.next())
                {
                    qDebug() << "3" << query.value(3).toString();
                    if(sha1[i]==query.value(3).toString())
                    {
                        if(query.value(2).toInt()==size[i])
                        {
                            found_dup = true;
                        }
                        else
                        {
                            QMessageBox msg;
                            msg.setText("Real World SHA1 colision !!!");
                            msg.exec();
                        }
                    }
                }
                if(found_dup)
                {
                    QMessageBox msg;
                    msg.setText(filename + " already in database");
                    msg.exec();
                    continue;
                }
            }
            else
            {
                QMessageBox msg;
                msg.setText("Dup look error:" + query.lastError().text());
                msg.exec();
                continue;
            }

            QSqlRecord record = model->record();

            record.remove(record.indexOf("file_id"));
            record.setValue("file_name", filename);
            record.setValue("file_size", size[i]);
            record.setValue("file_sha1sum", sha1[i]);

            /*-1 is set to indicate that it will be added to the last row*/
            if(model->insertRecord(-1, record))
            {
                qDebug()<<"successful insertion";
                model->submitAll();
                db.commit();

                QString path =  root_path + "/" + sha1[i].mid(0, 2) + "/" + sha1[i].mid(2,2);
                QString file_path = path + "/" + sha1[i] + ".bin";
                qDebug() << path;
                qDebug() << file_path;
                QDir dir(path);
                dir.mkpath(path);
                QFile::copy(files[i], file_path);
            }
            else
            {
                db.rollback();
            }
        }
    }
}

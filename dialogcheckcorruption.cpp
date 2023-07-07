#include <QSqlTableModel>
#include <QSettings>
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>

#include "dialogcheckcorruption.h"
#include "ui_dialogcheckcorruption.h"

extern QSettings global_settings;
extern QSqlTableModel *p_sql_table_model_table_files;

DialogCheckCorruption::DialogCheckCorruption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCheckCorruption)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    model.add_data_source(&corrupted_files);
    ui->tableView->setModel(&model);
}

DialogCheckCorruption::~DialogCheckCorruption()
{
    delete ui;
}

void DialogCheckCorruption::on_pushButtonCheck_clicked()
{
    // prepare copy file path
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    model.begin_update_data();

    corrupted_files.resize(0);
    int size = p_sql_table_model_table_files->rowCount();
    for (int i = 0; i < size; i++)
    {
        QString sha1 = p_sql_table_model_table_files->index(i, 3).data().toString();
        QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2) + "/" + sha1 + ".bin";

        QFile f(path);
        if (f.open(QFile::ReadOnly))
        {
            QCryptographicHash hash(QCryptographicHash::Sha1);
            if (hash.addData(&f))
            {
                if (sha1 != hash.result().toHex().toUpper())
                {
                    corrupted_file_t corrupted_file;
                    corrupted_file.file_id = p_sql_table_model_table_files->index(i, 0).data().toULongLong();
                    corrupted_file.file_name = p_sql_table_model_table_files->index(i, 1).data().toString();
                    corrupted_file.size = p_sql_table_model_table_files->index(i, 2).data().toULongLong();
                    corrupted_file.sha1 = p_sql_table_model_table_files->index(i, 3).data().toString();
                    corrupted_files.push_back(corrupted_file);
                }
            }
        }
        ui->progressBar->setValue(i * 100.0f / size);
    }
    ui->progressBar->setValue(100);

    model.end_update_data();

    ui->tableView->resizeColumnsToContents();

    if (0 == corrupted_files.size())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText(tr("No corrupted file found"));
        msg.exec();
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText(tr("Corrupted file found"));
        msg.exec();
    }
}

void DialogCheckCorruption::on_pushButtonClose_clicked()
{
    close();
}

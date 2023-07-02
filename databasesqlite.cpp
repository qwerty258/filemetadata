#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "databasesqlite.h"

#define DATABASE_VERSION 0

extern QSettings global_settings;

QSqlDatabase db;
QSqlTableModel *model = NULL;
QSqlTableModel *p_sql_table_model_table_tags = NULL;

int database_exec_sql_file(QString path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file error";
    }
    QTextStream in(&f);
    QSqlQuery query(db);
    QString sql = in.readAll();
    QStringList sqls = sql.trimmed().remove('\n').split(';');

    for (QStringList::Iterator it = sqls.begin(); it != sqls.end(); it++)
    {
        QString tmp = (*it);
        qDebug() << "SQL: " << tmp;
        if (tmp.isEmpty())
            continue;
        if (!query.exec(*it + ';'))
        {
            QMessageBox msg;
            msg.setText("database_000000.sql: " + *it + "ERROR: " + query.lastError().text());
            msg.exec();
            db.rollback();
            return -1;
        }
    }
    db.commit();
    return 0;
}

int database_init(void)
{
    global_settings.beginGroup("database");
    QString path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    if (QFileInfo(path).isDir())
    {
        path += "/FileMetadata.db";
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path);
        qDebug() << path;
        if (!db.open())
        {
            qDebug() << "db open error";
            qDebug() << db.lastError().text();
        }
        if (db.tables().contains("version"))
        {
            QSqlQuery query(db);
            if (query.exec("SELECT * FROM version;"))
            {
                query.next();
                unsigned int version = query.value(0).toUInt();
                qDebug() << "database version: " << version;
                switch (version)
                {
                case 0:
                    goto DATABASE_VERSION_1_UPDATE;
                    break;
                default:
                    break;
                }
            DATABASE_VERSION_1_UPDATE:
                qDebug() << "database version: " << version;
            }
            else
            {
                qDebug() << "SELECT version FROM version; ERROR";
                qDebug() << db.lastError().text();
                return -1;
            }
        }
        else
        {
            // create tables
            if (0 != database_exec_sql_file(":/database_000000.sql"))
                return -1;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText("database location invalid.");
        msg.exec();
        return -1;
    }
    return 0;
}

void database_uninit(void)
{
    if (db.isOpen())
    {
        db.close();
    }
}

int database_add_model_to_view(QTableView *p_table_view)
{
    model = new QSqlTableModel(NULL, db);
    if (NULL == model)
    {
        return -1;
    }
    model->setTable("files");
    if (model->select())
    {
        p_table_view->setModel(model);
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(model->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        return -1;
    }

    return 0;
}

void database_delete_model(void)
{
    if (NULL != model)
    {
        delete model;
    }
    model = NULL;
}

int database_search_for_sha1_dup(QString sha1, bool *result, qint64 size)
{
    QSqlQuery query(db);
    QString sql = "SELECT * FROM files WHERE file_sha1sum = \"" + sha1 + "\";";
    if (query.exec(sql))
    {
        qDebug() << query.size();
        *result = false;
        while (query.next())
        {
            qDebug() << "3" << query.value(3).toString();
            if (query.value(3).toString() == sha1)
            {
                if (query.value(2).toInt() == size)
                {
                    *result = true;
                }
                else
                {
                    QMessageBox msg;
                    msg.setIcon(QMessageBox::Information);
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.setText(
                        "Congratulations real world SHA1 collision !!!\nSame SHA1: " +
                        sha1 +
                        "\nDifferent size: in database: " +
                        query.value(2).toInt() +
                        " new file size: " +
                        size);
                    msg.exec();
                }
                break;
            }
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("SHA1 dup look error: " + query.lastError().text());
        msg.exec();
        *result = false;
        return -1;
    }
    return 0;
}

int database_add_new_file_record(QString &file_path_outside_filemetadata, QString &database_root_path, QString &filename, qint64 &size, QString &sha1)
{
    int ret = 0;
    QSqlRecord record = model->record();

    record.remove(record.indexOf("file_id"));
    record.setValue("file_name", filename);
    record.setValue("file_size", size);
    record.setValue("file_sha1sum", sha1);

    /*-1 is set to indicate that it will be added to the last row*/
    if (model->insertRecord(-1, record))
    {
        qDebug() << "successful insertion";
        model->submitAll();
        db.commit();

        QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2);
        QString file_path = path + "/" + sha1 + ".bin";
        qDebug() << path;
        qDebug() << file_path;
        QDir dir(path);
        dir.mkpath(path);
        QFile::copy(file_path_outside_filemetadata, file_path);
    }
    else
    {
        db.rollback();
        ret = -1;
    }
    return ret;
}

int database_delete_file_record(QString &database_root_path, qint64 index)
{
    int ret;
    QString sha1 = model->index(index, 3).data().toString();
    if (model->removeRows(index, 1))
    {
        if (model->submitAll())
        {
            model->database().commit();
            QString file_path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2) + "/" + sha1 + ".bin";
            if (QFile::moveToTrash(file_path))
                ret = 0;
            else
                ret = -1;
        }
        else
        {
            model->database().rollback();
            ret = -1;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("delete file recored error: " + model->lastError().text());
        ret = -1;
    }
    return ret;
}

int database_delete_file_record_refresh(void)
{
    return model->select() ? 0 : -1;
}

int database_table_tages_add_model_to_view(QTableView *p_table_view)
{
    p_sql_table_model_table_tags = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_tags)
    {
        return -1;
    }
    p_sql_table_model_table_tags->setTable("tags");
    if (p_sql_table_model_table_tags->select())
    {
        p_table_view->setModel(p_sql_table_model_table_tags);
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(model->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_tags;
        p_sql_table_model_table_tags = nullptr;
        return -1;
    }

    return 0;
}

void database_delete_table_tags_model(void)
{
    if (nullptr != p_sql_table_model_table_tags)
    {
        delete p_sql_table_model_table_tags;
    }
    p_sql_table_model_table_tags = nullptr;
}

int database_table_tages_add_tag(QString tag)
{
    int ret = 0;
    QSqlRecord record = p_sql_table_model_table_tags->record();
    record.remove(record.indexOf("tag_id"));
    record.setValue("tag_name", tag);
    if (p_sql_table_model_table_tags->insertRecord(-1, record))
    {
        qDebug() << "tag insert";
        p_sql_table_model_table_tags->submitAll();
        db.commit();
    }
    else
    {
        db.rollback();
        ret = -1;
    }
    return ret;
}

int database_table_tags_delete(qint64 index)
{
    int ret = 0;
    // TODO: add trigger to delete join tables
    if (p_sql_table_model_table_tags->removeRows(index, 1))
    {
        if (model->submitAll())
        {
            model->database().commit();
        }
        else
        {
            model->database().rollback();
            ret = -1;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("delete file recored error: " + p_sql_table_model_table_tags->lastError().text());
        ret = -1;
    }
    return ret;
}

int database_table_tags_delete_refresh(void)
{
    return p_sql_table_model_table_tags->select() ? 0 : -1;
}

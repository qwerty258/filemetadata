#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "databasesqlite.h"

#define DATABASE_MAX_VERSION 0

extern QSettings global_settings;

QSqlDatabase db;

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
    QStringList sqls = sql.trimmed().replace('\r', ' ').replace('\n', ' ').split("/*STATEMENT SEPARATOR*/");

    for (QStringList::Iterator it = sqls.begin(); it != sqls.end(); it++)
    {
        QString tmp = (*it);
        qDebug() << "SQL: " << tmp;
        if (tmp.isEmpty())
            continue;
        if (!query.exec(*it))
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
    QString file_path = path + "/filemetadata.db";

    if (QFileInfo(file_path).exists())
    {
        if (!QFileInfo(file_path).isFile())
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Database file found as folder, the folder will be trashed");
            msg.exec();
            if (!QFile::moveToTrash(file_path))
            {
                QMessageBox msg;
                msg.setIcon(QMessageBox::Critical);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setText("Trash folder error");
                msg.exec();
                return -1;
            }
        }
    }
    else
    {
        QDir dir(path);
        if (!dir.mkpath(path))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Make path error");
            msg.exec();
            return -1;
        }
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file_path);
    if (!db.open())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("database open error: " + db.lastError().text());
        msg.exec();
        return -1;
    }

    db.exec("PRAGMA synchronous = OFF;");
    db.exec("PRAGMA journal_mode = MEMORY;");

    size_t i = 0;

    if (db.tables().contains("version"))
    {
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM version;"))
        {
            query.next();
            i = query.value(0).toUInt() + 1;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Get database version error: " + query.lastError().text());
            msg.exec();
            return -1;
        }
    }

    for (; i <= DATABASE_MAX_VERSION; i++)
    {
        QString sql_file = QString(":/database_%1.sql").arg(i, 6, 10, QChar('0'));
        if (0 != database_exec_sql_file(sql_file))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Execute SQL file " + sql_file + " error");
            msg.exec();
            return -1;
        }
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

table_model::table_model(QString table_name)
{
    p_sql_table_model = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model)
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText("Database table model pointer ERROR");
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
    }
    this->table_name = table_name;
    p_sql_table_model->setTable(table_name);
    p_sql_table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if (!p_sql_table_model->select())
    {
        show_error_message_box();
    }
}

table_model::~table_model()
{
    delete p_sql_table_model;
}

void table_model::show_error_message_box()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle("Error");
    msgbox.setText("Database ERROR: " + p_sql_table_model->lastError().text());
    msgbox.setStandardButtons(QMessageBox::Ok);
    msgbox.exec();
}

QSqlTableModel *table_model::get_table_model()
{
    return p_sql_table_model;
}

bool table_model::table_select()
{
    return p_sql_table_model->select();
}

bool table_model::table_sync()
{
    bool ret = p_sql_table_model->submitAll();
    if (ret)
    {
        p_sql_table_model->database().commit();
    }
    else
    {
        show_error_message_box();
        p_sql_table_model->revertAll();
        p_sql_table_model->database().rollback();
    }
    return ret;
}

void table_model::table_files_tag_match(quint64 tag_id)
{
    if ("files" != table_name)
        return;

    QString filter = QString("file_id IN(SELECT file_id FROM tag_file_join WHERE tag_id=%1)").arg(QString::number(tag_id));

    p_sql_table_model->setFilter(filter);
    p_sql_table_model->select();
}

void table_model::table_files_match(QString &match_term)
{
    if ("files" != table_name)
        return;

    p_sql_table_model->setFilter("file_name LIKE '%" + match_term + "%'");
    p_sql_table_model->select();
}

void table_model::table_torrents_match(QString &match_term)
{
    if ("torrents" != table_name)
        return;

    p_sql_table_model->setFilter("name LIKE '%" + match_term + "%'");
    p_sql_table_model->select();
}

void table_model::table_files_in_torrent_match(QString &match_term)
{
    if ("files_in_torrent" != table_name)
        return;

    p_sql_table_model->setFilter("path LIKE '%" + match_term + "%'");
    p_sql_table_model->select();
}

void table_model::clear_match(void)
{
    p_sql_table_model->setFilter("");
    p_sql_table_model->select();
}

bool table_model::table_files_add_record(QString &filename, qint64 &size, QString &sha1, quint64 &new_file_id)
{
    if ("files" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();

    record.remove(record.indexOf("file_id"));
    record.setValue("file_name", filename);
    record.setValue("file_size", size);
    record.setValue("file_sha1sum", sha1);

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
        }

        QVariant val = p_sql_table_model->query().lastInsertId();
        if (val.isValid())
        {
            new_file_id = val.toULongLong();
            // qDebug() << "new file_id: " << new_file_id;
        }
        else
        {
            // qDebug() << "query.lastInsertId() invalid";
            return false;
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_files_get_file_info(qint64 index, QString &filename, QString &sha1)
{
    if ("files" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    filename = p_sql_table_model->index(index, 1).data().toString();
    sha1 = p_sql_table_model->index(index, 3).data().toString();

    return true;
}

bool table_model::table_files_search_for_sha1_dup(QString sha1, bool *b_dup, qint64 size)
{
    *b_dup = false;

    if ("files" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    p_sql_table_model->setFilter("file_sha1sum = \"" + sha1 + "\"");
    if (p_sql_table_model->select())
    {
        if (0 != p_sql_table_model->rowCount())
        {
            if (p_sql_table_model->index(0, 2).data().toUInt() == size)
            {
                *b_dup = true;
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
                    QString::number(p_sql_table_model->index(0, 2).data().toUInt()) +
                    " new file size: " +
                    QString::number(size));
                msg.exec();
            }
        }
    }
    else
    {
        show_error_message_box();
        *b_dup = false;
        p_sql_table_model->setFilter("");
        return false;
    }
    p_sql_table_model->setFilter("");
    return true;
}

bool table_model::table_files_in_torrent_add_record(torrent_metadata_t &data, quint64 file_id_as_torrent_id)
{
    if ("files_in_torrent" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();

    for (qsizetype i = 0; i < data.files.size(); i++)
    {
        record.setValue("torrent_id", file_id_as_torrent_id);
        record.setValue("path", data.files[i].path);
        record.setValue("length", data.files[i].length);
        if (!p_sql_table_model->insertRecord(-1, record))
        {
            show_error_message_box();
            p_sql_table_model->database().rollback();
            return false;
        }
    }

    if (p_sql_table_model->submitAll())
    {
        p_sql_table_model->database().commit();
    }
    else
    {
        show_error_message_box();
        return false;
    }

    return true;
}

bool table_model::table_file_serial_join_add_record(quint64 &new_file_id, quint64 &new_serial_id)
{
    if ("file_serial_join" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();

    record.setValue("file_id", new_file_id);
    record.setValue("serial_id", new_serial_id);

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_serials_add_record(serial_metadata_t &data, quint64 &new_serial_id)
{
    if ("serials" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();

    record.remove(record.indexOf("serial_id"));

    record.setValue("author", data.author);
    record.setValue("year", data.year);
    record.setValue("title", data.title);
    record.setValue("journal", data.journal);
    record.setValue("volume", data.volume);
    record.setValue("issue", data.issue);
    record.setValue("pages", data.pages);
    record.setValue("start_page", data.start_page);
    record.setValue("issn", data.issn);
    record.setValue("doi", data.doi);
    record.setValue("keywords", data.keywords);
    record.setValue("abstract", data.abstract);
    record.setValue("author_address", data.author_address);
    record.setValue("language", data.language);

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();

            QVariant val = p_sql_table_model->query().lastInsertId();
            if (val.isValid())
            {
                new_serial_id = val.toULongLong();
                // qDebug() << "new serial_id: " << new_serial_id;
            }
            else
            {
                // qDebug() << "query.lastInsertId() invalid";
                return false;
            }
        }
        else
        {
            show_error_message_box();
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_tags_add_record(QString tag)
{
    if ("tags" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();
    record.remove(record.indexOf("tag_id"));
    record.setValue("tag_name", tag);
    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
            return false;
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_tag_file_join_add(int tag_index, qint64 file_index)
{
    if ("tag_file_join" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    // TODO: set correct id when called
    QSqlRecord record = p_sql_table_model->record();
    record.setValue("tag_id", tag_index);
    record.setValue("file_id", file_index);

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
            return false;
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_torrents_add_record(torrent_metadata_t &data, quint64 file_id_as_torrent_id)
{
    if ("torrents" != table_name)
        return false;

    if (nullptr == p_sql_table_model)
        return false;

    QSqlRecord record = p_sql_table_model->record();

    record.remove(record.indexOf("torrent_id"));
    record.setValue("torrent_id", file_id_as_torrent_id);
    record.setValue("comment", data.comment);
    record.setValue("created_by", data.created_by);
    record.setValue("creation_date", data.creation_date);
    record.setValue("info_hash_v1", data.info_hash_v1);
    record.setValue("info_hash_v2", data.info_hash_v2);
    record.setValue("name", data.name);
    record.setValue("piece_length", data.piece_length);
    record.setValue("pieces", data.pieces);

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
            return false;
        }
    }
    else
    {
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_delete_record(qint64 index)
{
    if (nullptr == p_sql_table_model)
        return false;

    if (p_sql_table_model->removeRows(index, 1))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            p_sql_table_model->database().rollback();
            return false;
        }
    }
    else
    {
        show_error_message_box();
        return false;
    }
    return true;
}

bool table_model::table_files_delete_record(qint64 index, QString &sha1)
{
    if ("files" != table_name)
        return false;

    sha1 = p_sql_table_model->index(index, 3).data().toString();
    if (p_sql_table_model->removeRows(index, 1))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            p_sql_table_model->database().rollback();
            return false;
        }
    }
    else
    {
        show_error_message_box();
        return false;
    }
    return true;
}

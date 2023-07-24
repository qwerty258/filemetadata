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

#define DATABASE_MAX_VERSION 0

extern QSettings global_settings;

QSqlDatabase db;
QSqlTableModel *p_sql_table_model_table_files = nullptr;
QSqlTableModel *p_sql_table_model_table_tags = nullptr;
QSqlTableModel *p_sql_table_model_table_torrents = nullptr;
QSqlTableModel *p_sql_table_model_table_files_in_torrent = nullptr;
QSqlTableModel *p_sql_table_model_table_serials = nullptr;
QSqlTableModel *p_sql_table_model_table_file_serial_join = nullptr;

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
    QString file_path = path + "/FileMetadata.db";

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

    for(; i <= DATABASE_MAX_VERSION; i++)
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

int database_table_files_add_model_to_view(QTableView *p_table_view)
{
    p_sql_table_model_table_files = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_files)
    {
        return -1;
    }
    p_sql_table_model_table_files->setTable("files");
    p_sql_table_model_table_files->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // TODO: delete code below. This is a test for filter here, for feature dev.
    p_sql_table_model_table_files->setFilter("file_id IN(SELECT file_id FROM tag_file_join)");
    p_sql_table_model_table_files->setFilter("");
    if (p_sql_table_model_table_files->select())
    {
        p_table_view->setModel(p_sql_table_model_table_files);
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_files->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        return -1;
    }

    return 0;
}

void database_table_files_delete_model(void)
{
    if (nullptr != p_sql_table_model_table_files)
    {
        delete p_sql_table_model_table_files;
    }
    p_sql_table_model_table_files = nullptr;
}

int database_table_files_search_for_sha1_dup(QString sha1, bool *result, qint64 size)
{
    QSqlQuery query(db);
    QString sql = "SELECT * FROM files WHERE file_sha1sum = \"" + sha1 + "\";";
    if (query.exec(sql))
    {
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
                        QString::number(query.value(2).toInt()) +
                        " new file size: " +
                        QString::number(size));
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

bool database_table_files_add_new_file_record(QString &filename, qint64 &size, QString &sha1, quint64 &new_file_id)
{
    QSqlRecord record = p_sql_table_model_table_files->record();

    record.remove(record.indexOf("file_id"));
    record.setValue("file_name", filename);
    record.setValue("file_size", size);
    record.setValue("file_sha1sum", sha1);

    /*-1 is set to indicate that it will be added to the last row*/
    if (p_sql_table_model_table_files->insertRecord(-1, record))
    {
        // qDebug() << "add new file sql: " << p_sql_table_model_table_files->query().lastQuery();
        p_sql_table_model_table_files->submitAll();
        db.commit();
        QVariant val = p_sql_table_model_table_files->query().lastInsertId();
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
        db.rollback();
        return false;
    }
    return true;
}

void database_table_files_get_file_info(qint64 index, QString &filename, QString &sha1)
{
    filename = p_sql_table_model_table_files->index(index, 1).data().toString();
    sha1 = p_sql_table_model_table_files->index(index, 3).data().toString();
}

bool database_table_files_delete_file_record(qint64 index, QString &sha1)
{
    bool ret = true;
    sha1 = p_sql_table_model_table_files->index(index, 3).data().toString();
    if (p_sql_table_model_table_files->removeRows(index, 1))
    {
        if (p_sql_table_model_table_files->submitAll())
        {
            p_sql_table_model_table_files->database().commit();
        }
        else
        {
            p_sql_table_model_table_files->database().rollback();
            ret = false;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("delete file recored error: " + p_sql_table_model_table_files->lastError().text());
        msg.exec();
        ret = false;
    }
    return ret;
}

int database_table_files_model_select(void)
{
    return p_sql_table_model_table_files->select() ? 0 : -1;
}

void database_table_files_match_name(QString &match_term)
{
    p_sql_table_model_table_files->setFilter(
        "file_name LIKE '%" + match_term + "%'");
}

void database_table_files_clear_match(void)
{
    p_sql_table_model_table_files->setFilter("");
}

int database_table_tags_create_model(void)
{
    p_sql_table_model_table_tags = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_tags)
    {
        return -1;
    }
    p_sql_table_model_table_tags->setTable("tags");
    p_sql_table_model_table_tags->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!p_sql_table_model_table_tags->select())
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_files->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_tags;
        p_sql_table_model_table_tags = nullptr;
        return -1;
    }

    return 0;
}

int database_table_tags_add_model_to_view(QTableView *p_table_view)
{
    if (0 == database_table_tags_create_model())
    {
        p_table_view->setModel(p_sql_table_model_table_tags);
    }
    else
        return -1;

    return 0;
}

int database_table_tags_add_model_to_combobox(QComboBox *p_combobox)
{
    if (0 == database_table_tags_create_model())
    {
        p_combobox->setModel(p_sql_table_model_table_tags);
    }
    else
        return -1;

    return 0;
}

void database_table_tags_delete_model(void)
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

    if (p_sql_table_model_table_tags->removeRows(index, 1))
    {
        if (p_sql_table_model_table_files->submitAll())
        {
            p_sql_table_model_table_files->database().commit();
        }
        else
        {
            p_sql_table_model_table_files->database().rollback();
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

int database_table_tags_model_select(void)
{
    return p_sql_table_model_table_tags->select() ? 0 : -1;
}

int database_table_tag_file_join_add(int tag_index, qint64 file_index)
{
    QSqlTableModel *p_sql_table_model_table_tag_file_join = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_tag_file_join)
    {
        return -1;
    }
    p_sql_table_model_table_tag_file_join->setTable("tag_file_join");
    if (!p_sql_table_model_table_tag_file_join->select())
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_files->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_tag_file_join;
        p_sql_table_model_table_tag_file_join = nullptr;
        return -1;
    }

    QSqlRecord record = p_sql_table_model_table_tag_file_join->record();
    record.setValue("tag_id", p_sql_table_model_table_tags->index(tag_index, 0).data().toULongLong());
    record.setValue("file_id", p_sql_table_model_table_files->index(file_index, 0).data().toULongLong());

    if (p_sql_table_model_table_tag_file_join->insertRecord(-1, record))
    {
        p_sql_table_model_table_tag_file_join->submitAll();
        db.commit();
    }

    delete p_sql_table_model_table_tag_file_join;

    return 0;
}

bool database_table_torrents_create_model(void)
{
    if (nullptr != p_sql_table_model_table_torrents)
        return true;

    p_sql_table_model_table_torrents = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_torrents)
    {
        return false;
    }
    p_sql_table_model_table_torrents->setTable("torrents");
    p_sql_table_model_table_torrents->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!p_sql_table_model_table_torrents->select())
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_torrents->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_torrents;
        p_sql_table_model_table_torrents = nullptr;
        return false;
    }

    return true;
}

void database_table_torrents_add_model_to_view(QTableView *p_table_view)
{
    p_table_view->setModel(p_sql_table_model_table_torrents);
}

void database_table_torrents_delete_model(void)
{
    if (nullptr != p_sql_table_model_table_torrents)
    {
        delete p_sql_table_model_table_torrents;
    }
    p_sql_table_model_table_torrents = nullptr;
}

bool database_table_torrents_add_torrent(torrent_metadata_t &data, quint64 file_id_as_torrent_id)
{
    if (nullptr == p_sql_table_model_table_torrents)
        return false;

    QSqlRecord record = p_sql_table_model_table_torrents->record();

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

    if (p_sql_table_model_table_torrents->insertRecord(-1, record))
    {
        if (p_sql_table_model_table_torrents->submitAll())
        {
            db.commit();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText(p_sql_table_model_table_torrents->lastError().text());
            msg.exec();
        }
    }
    else
    {
        db.rollback();
        return false;
    }
    return true;
}

bool database_table_files_in_torrent_create_model(void)
{
    if (nullptr != p_sql_table_model_table_files_in_torrent)
        return true;

    p_sql_table_model_table_files_in_torrent = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_files_in_torrent)
    {
        return false;
    }
    p_sql_table_model_table_files_in_torrent->setTable("files_in_torrent");
    p_sql_table_model_table_files_in_torrent->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!p_sql_table_model_table_files_in_torrent->select())
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_files_in_torrent->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_files_in_torrent;
        p_sql_table_model_table_files_in_torrent = nullptr;
        return false;
    }

    return true;
}

void database_table_files_in_torrent_add_model_to_view(QTableView *p_table_view)
{
    p_table_view->setModel(p_sql_table_model_table_files_in_torrent);
}

void database_table_files_in_torrent_delete_model(void)
{
    if (nullptr != p_sql_table_model_table_files_in_torrent)
    {
        delete p_sql_table_model_table_files_in_torrent;
    }
    p_sql_table_model_table_files_in_torrent = nullptr;
}

bool database_table_files_in_torrent_add_torrent(torrent_metadata_t &data, quint64 file_id_as_torrent_id)
{
    if (nullptr == p_sql_table_model_table_files_in_torrent)
        return false;

    QSqlRecord record = p_sql_table_model_table_files_in_torrent->record();

    for (qsizetype i = 0; i < data.files.size(); i++)
    {
        record.setValue("torrent_id", file_id_as_torrent_id);
        record.setValue("path", data.files[i].path);
        record.setValue("length", data.files[i].length);
        if (!p_sql_table_model_table_files_in_torrent->insertRecord(-1, record))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText(p_sql_table_model_table_files_in_torrent->lastError().text());
            msg.exec();
            db.rollback();
            return false;
        }
    }

    p_sql_table_model_table_files_in_torrent->submitAll();
    db.commit();

    return true;
}

bool database_table_serials_create_model(void)
{
    if (nullptr != p_sql_table_model_table_serials)
        return true;

    p_sql_table_model_table_serials = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_serials) {
        return false;
    }
    p_sql_table_model_table_serials->setTable("serials");
    p_sql_table_model_table_serials->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!p_sql_table_model_table_serials->select()) {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_serials->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_serials;
        p_sql_table_model_table_serials = nullptr;
        return false;
    }

    return true;
}

bool database_table_serials_add_record(serial_metadata_t &data, quint64 &new_serial_id)
{
    if (nullptr == p_sql_table_model_table_serials)
        return false;

    QSqlRecord record = p_sql_table_model_table_serials->record();

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

    if (p_sql_table_model_table_serials->insertRecord(-1, record)) {
        if (p_sql_table_model_table_serials->submitAll()) {
            db.commit();

            QVariant val = p_sql_table_model_table_serials->query().lastInsertId();
            if (val.isValid()) {
                new_serial_id = val.toULongLong();
                // qDebug() << "new serial_id: " << new_serial_id;
            } else {
                // qDebug() << "query.lastInsertId() invalid";
                return false;
            }
        } else {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText(p_sql_table_model_table_serials->lastError().text());
            msg.exec();
        }
    } else {
        db.rollback();
        return false;
    }
    return true;
}

void database_table_serials_delete_model(void)
{
    if (nullptr != p_sql_table_model_table_serials) {
        delete p_sql_table_model_table_serials;
    }
    p_sql_table_model_table_serials = nullptr;
}

bool database_table_serial_file_join_create_model(void)
{
    if (nullptr != p_sql_table_model_table_file_serial_join)
        return true;

    p_sql_table_model_table_file_serial_join = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model_table_file_serial_join) {
        return false;
    }
    p_sql_table_model_table_file_serial_join->setTable("serial_file_join");
    p_sql_table_model_table_file_serial_join->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!p_sql_table_model_table_file_serial_join->select()) {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(p_sql_table_model_table_file_serial_join->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        delete p_sql_table_model_table_file_serial_join;
        p_sql_table_model_table_file_serial_join = nullptr;
        return false;
    }

    return true;
}

bool database_table_serials_add_record(quint64 &new_file_id, quint64 &new_serial_id)
{
    if (nullptr == p_sql_table_model_table_file_serial_join)
        return false;

    QSqlRecord record = p_sql_table_model_table_file_serial_join->record();

    record.setValue("file_id", new_file_id);
    record.setValue("serial_id", new_serial_id);

    if (p_sql_table_model_table_file_serial_join->insertRecord(-1, record)) {
        if (p_sql_table_model_table_file_serial_join->submitAll()) {
            db.commit();
        } else {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText(p_sql_table_model_table_file_serial_join->lastError().text());
            msg.exec();
        }
    } else {
        db.rollback();
        return false;
    }
    return true;
}

void database_table_serial_file_join_delete_model(void)
{
    if (nullptr != p_sql_table_model_table_file_serial_join)
    {
        delete p_sql_table_model_table_file_serial_join;
    }
    p_sql_table_model_table_file_serial_join = nullptr;
}

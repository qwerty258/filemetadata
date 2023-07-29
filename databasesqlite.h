#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QString>
#include <QSqlTableModel>

#include "datastructdefines.h"

int database_init(void);
void database_uninit(void);
int database_exec_sql_file(QString path);

namespace FileMetadata {
class table_model;
}

class table_model
{
public:
    explicit table_model(QString table_name);
    ~table_model();
    void show_error_message_box();
    QSqlTableModel *get_table_model();
    bool table_select();
    bool table_sync();
    void table_files_match(QString &match_term);
    void clear_match();
    bool table_files_add_record(QString &filename, qint64 &size, QString &sha1, quint64 &new_file_id);
    bool table_files_delete_record(qint64 index, QString &sha1);
    bool table_files_get_file_info(qint64 index, QString &filename, QString &sha1);
    bool table_files_search_for_sha1_dup(QString sha1, bool *b_dup, qint64 size);
    bool table_files_in_torrent_add_record(torrent_metadata_t &data, quint64 file_id_as_torrent_id);
    bool table_serials_add_record(serial_metadata_t &data, quint64 &new_serial_id);
    bool table_file_serial_join_add_record(quint64 &new_file_id, quint64 &new_serial_id);
    bool table_serials_delete_record(qint64 index);
    bool table_tags_add_record(QString tag);
    bool table_tag_file_join_add(int tag_index, qint64 file_index);
    bool table_torrents_add_record(torrent_metadata_t &data, quint64 file_id_as_torrent_id);
    bool table_delete_record(qint64 index);

private:
    QSqlTableModel *p_sql_table_model;
    QString table_name;
};

#endif // DATABASESQLITE_H

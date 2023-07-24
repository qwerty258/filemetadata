#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QString>
#include <QTableView>
#include <QComboBox>

#include "datastructdefines.h"

int database_init(void);
void database_uninit(void);
int database_exec_sql_file(QString path);

int database_table_files_add_model_to_view(QTableView *p_table_view);
void database_table_files_delete_model(void);
int database_table_files_search_for_sha1_dup(QString sha1, bool *result, qint64 size);
bool database_table_files_add_new_file_record(QString &filename, qint64 &size, QString &sha1, quint64 &new_file_id);
void database_table_files_get_file_info(qint64 index, QString &filename, QString &sha1);
bool database_table_files_delete_file_record(qint64 index, QString &sha1);
int database_table_files_model_select(void);
void database_table_files_match_name(QString &match_term);
void database_table_files_clear_match(void);

int database_table_tags_create_model(void);
int database_table_tags_add_model_to_view(QTableView *p_table_view);
void database_table_tags_delete_model(void);
int database_table_tages_add_tag(QString tag);
int database_table_tags_delete(qint64 index);
int database_table_tags_model_select(void);
int database_table_tags_add_model_to_combobox(QComboBox *p_combobox);
int database_table_tag_file_join_add(int tag_index, qint64 file_index);

bool database_table_torrents_create_model(void);
void database_table_torrents_add_model_to_view(QTableView *p_table_view);
void database_table_torrents_delete_model(void);
bool database_table_torrents_add_torrent(torrent_metadata_t &data, quint64 file_id_as_torrent_id);

bool database_table_files_in_torrent_create_model(void);
void database_table_files_in_torrent_add_model_to_view(QTableView *p_table_view);
void database_table_files_in_torrent_delete_model(void);
bool database_table_files_in_torrent_add_torrent(torrent_metadata_t &data, quint64 file_id_as_torrent_id);

bool database_table_serials_create_model(void);
bool database_table_serials_add_record(serial_metadata_t &data, quint64 &new_serial_id);
void database_table_serials_delete_model(void);

bool database_table_serial_file_join_create_model(void);
bool database_table_serials_add_record(quint64 &new_file_id, quint64 &new_serial_id);
void database_table_serial_file_join_delete_model(void);

#endif // DATABASESQLITE_H

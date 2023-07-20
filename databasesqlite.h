#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QString>
#include <QTableView>
#include <QComboBox>

int database_init(void);
void database_uninit(void);
int database_exec_sql_file(QString path);

int database_table_files_add_model_to_view(QTableView *p_table_view);
void database_table_files_delete_model(void);
int database_table_files_search_for_sha1_dup(QString sha1, bool *result, qint64 size);
bool database_table_files_add_new_file_record(QString &filename, qint64 &size, QString &sha1);
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

#endif // DATABASESQLITE_H

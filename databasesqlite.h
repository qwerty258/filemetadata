#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QString>
#include <QTableView>

int database_init(void);
void database_uninit(void);
int database_exec_sql_file(QString path);
int database_add_model_to_view(QTableView *p_table_view);
void database_delete_model(void);
int database_search_for_sha1_dup(QString sha1, bool *result, qint64 size);
int database_add_new_file_record(QString &file_path_outside_filemetadata, QString &database_root_path, QString &filename, qint64 &size, QString &sha1);
int database_delete_file_record(QString &database_root_path, qint64 index);
int database_delete_file_record_refresh(void);

#endif // DATABASESQLITE_H

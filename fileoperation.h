#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QString>

bool file_operation_new_file(QString &full_file_path, QString &database_root_path, QString &sha1, bool b_copy);
bool file_operation_delete_file(QString &database_root_path, QString &sha1);
bool file_operation_export_file(QString &database_root_path, QString &filename, QString &sha1, QString &save_path);

#endif // FILEOPERATION_H

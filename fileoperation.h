#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QString>

bool file_operation_new_file(QString &full_file_path, QString &database_root_path, QString &sha1);
bool file_operation_delete_file(QString &database_root_path, QString &sha1);

#endif // FILEOPERATION_H

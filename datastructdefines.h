#ifndef DATASTRUCTDEFINES_H
#define DATASTRUCTDEFINES_H

#include <QString>

typedef struct
{
    QString full_path;
    QString file_name;
    QString sha1;
    qint64 size;
} new_file_info_t;

typedef struct
{
    qint64 file_id;
    QString file_name;
    qint64 size;
    QString sha1;
} corrupted_file_t;

#endif // DATASTRUCTDEFINES_H

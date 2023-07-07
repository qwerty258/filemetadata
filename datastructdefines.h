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

#endif // DATASTRUCTDEFINES_H

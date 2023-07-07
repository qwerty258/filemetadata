#ifndef NEWFILESLISTDATASTRUCT_H
#define NEWFILESLISTDATASTRUCT_H

#include <QString>

typedef struct {
    QString full_path;
    QString file_name;
    QString sha1;
    qint64 size;
} basic_file_metadata_t;

#endif // NEWFILESLISTDATASTRUCT_H

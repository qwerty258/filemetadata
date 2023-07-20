#include <QDir>
#include <QFile>
// #include <QDebug>

#include "fileoperation.h"

bool file_operation_new_file(QString &full_file_path, QString &database_root_path, QString &sha1)
{
    bool ret = false;
    QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2);
    QString file_path = path + "/" + sha1 + ".bin";
    // qDebug() << full_file_path;
    // qDebug() << path;
    // qDebug() << file_path;
    QDir dir(path);
    ret = dir.mkpath(path);
    if (ret)
        ret = QFile::copy(full_file_path, file_path);
    return ret;
}

bool file_operation_delete_file(QString &database_root_path, QString &sha1)
{
    QString file_path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2) + "/" + sha1 + ".bin";
    // qDebug() << file_path;
    return QFile::moveToTrash(file_path);
}

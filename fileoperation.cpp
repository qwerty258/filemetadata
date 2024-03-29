#include <QDir>
#include <QFile>
// #include <QDebug>

#include "fileoperation.h"

bool file_operation_new_file(QString &full_file_path, QString &database_root_path, QString &sha1, bool b_copy)
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
    {
        // qDebug() << b_copy;
        if (b_copy)
        {
            ret = QFile::copy(full_file_path, file_path);
        }
        else
        {
            ret = QFile::rename(full_file_path, file_path);
        }
    }
    return ret;
}

bool file_operation_delete_file(QString &database_root_path, QString &sha1)
{
    QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2);
    QString file_path = path + "/" + sha1 + ".bin";
    // qDebug() << path;
    // qDebug() << file_path;
    int ret = QFile::moveToTrash(file_path);
    QDir dir(path);
    dir.rmpath(path);
    return ret;
}

bool file_operation_export_file(QString &database_root_path, QString &filename, QString &sha1, QString &save_path)
{
    QString file_in_management = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2) + "/" + sha1 + ".bin";
    QString file_save_path = save_path + "/" + filename;
    return QFile::copy(file_in_management, file_save_path);
}

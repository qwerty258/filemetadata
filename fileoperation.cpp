#include <QDir>
#include <QFile>
// #include <QDebug>

#include "fileoperation.h"

bool file_operation_new_file(QString &full_file_path, QString &database_root_path, QString &sha1)
{
    bool ret = false;
    QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2);
    QString file_path = path + "/" + sha1 + ".bin";
    // qDebug() << __FILE__ << __LINE__ << full_file_path;
    // qDebug() << __FILE__ << __LINE__ << path;
    // qDebug() << __FILE__ << __LINE__ << file_path;
    QDir dir(path);
    ret = dir.mkpath(path);
    if (ret)
        ret = QFile::copy(full_file_path, file_path);
    return ret;
}

bool file_operation_delete_file(QString &database_root_path, QString &sha1)
{
    QString path = database_root_path + "/" + sha1.mid(0, 2) + "/" + sha1.mid(2, 2);
    QString file_path = path + "/" + sha1 + ".bin";
    // qDebug() << __FILE__ << __LINE__ << path;
    // qDebug() << __FILE__ << __LINE__ << file_path;
    int ret = QFile::moveToTrash(file_path);
    QDir dir(path);
    dir.rmpath(path);
    return ret;
}

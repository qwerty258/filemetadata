#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QDebug>

QSettings global_settings(QSettings::NativeFormat, QSettings::UserScope, "FileMetadata", "FileMetadata");

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file} %{line}: %{message}");
    global_settings.sync();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

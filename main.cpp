#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

QSettings global_settings(QSettings::NativeFormat, QSettings::UserScope, "FileMetadata", "FileMetadata");

int main(int argc, char *argv[])
{
    global_settings.sync();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

QSettings *p_global_settings;

int main(int argc, char *argv[])
{
    p_global_settings=new QSettings(QSettings::NativeFormat, QSettings::UserScope, "FileMetadata", "FileMetadata");
    p_global_settings->sync();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

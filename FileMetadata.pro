QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += GIT_DESCRIBE="\\\"$$system(git describe)\\\""
DEFINES += GIT_CURRENT_SHA1="\\\"$$system(git rev-parse --verify HEAD)\\\""

SOURCES += \
    databasesqlite.cpp \
    dialogabout.cpp \
    dialogaddtag.cpp \
    dialogcopyfilesin.cpp \
    dialogglobalsettings.cpp \
    dialogtags.cpp \
    main.cpp \
    mainwindow.cpp \
    newfilelisttablemodel.cpp

HEADERS += \
    databasesqlite.h \
    dialogabout.h \
    dialogaddtag.h \
    dialogcopyfilesin.h \
    dialogglobalsettings.h \
    dialogtags.h \
    mainwindow.h \
    newfilelisttablemodel.h \
    newfileslistdatastruct.h

FORMS += \
    dialogabout.ui \
    dialogaddtag.ui \
    dialogcopyfilesin.ui \
    dialogglobalsettings.ui \
    dialogtags.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FileMetadata.qrc

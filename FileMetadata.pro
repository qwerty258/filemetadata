QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += GIT_DESCRIBE="\\\"$(shell git -C \""$$_PRO_FILE_PWD_"\" describe)\\\""
DEFINES += GIT_CURRENT_SHA1="\\\"$(shell git -C \""$$_PRO_FILE_PWD_"\" rev-parse --verify HEAD)\\\""

SOURCES += \
    databasesqlite.cpp \
    dialogabout.cpp \
    dialogcopyfilesin.cpp \
    dialogglobalsettings.cpp \
    dialogtags.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    databasesqlite.h \
    dialogabout.h \
    dialogcopyfilesin.h \
    dialogglobalsettings.h \
    dialogtags.h \
    mainwindow.h

FORMS += \
    dialogabout.ui \
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

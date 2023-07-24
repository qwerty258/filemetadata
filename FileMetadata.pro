QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += GIT_DESCRIBE="\\\"$$system(git describe --tags)\\\""
DEFINES += GIT_CURRENT_SHA1="\\\"$$system(git rev-parse --verify HEAD)\\\""

SOURCES += \
    databasesqlite.cpp \
    dialogabout.cpp \
    dialogaddmetadata.cpp \
    dialogaddtag.cpp \
    dialogcheckcorruption.cpp \
    dialogglobalsettings.cpp \
    dialogimportfiles.cpp \
    dialogtags.cpp \
    dialogviewtorrents.cpp \
    fileoperation.cpp \
    frameserial.cpp \
    frametorrent.cpp \
    main.cpp \
    mainwindow.cpp \
    tablemodelcorruptedfils.cpp \
    tablemodelfilesintorrent.cpp \
    tablemodelnewfiles.cpp

HEADERS += \
    databasesqlite.h \
    datastructdefines.h \
    dialogabout.h \
    dialogaddmetadata.h \
    dialogaddtag.h \
    dialogcheckcorruption.h \
    dialogglobalsettings.h \
    dialogimportfiles.h \
    dialogtags.h \
    dialogviewtorrents.h \
    fileoperation.h \
    frameserial.h \
    frametorrent.h \
    mainwindow.h \
    tablemodelcorruptedfils.h \
    tablemodelfilesintorrent.h \
    tablemodelnewfiles.h

FORMS += \
    dialogabout.ui \
    dialogaddmetadata.ui \
    dialogaddtag.ui \
    dialogcheckcorruption.ui \
    dialogglobalsettings.ui \
    dialogimportfiles.ui \
    dialogtags.ui \
    dialogviewtorrents.ui \
    frameserial.ui \
    frametorrent.ui \
    mainwindow.ui

LIBS += -ltorrent-rasterbar

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FileMetadata.qrc

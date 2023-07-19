#ifndef FRAMETORRENT_H
#define FRAMETORRENT_H

#include <QFrame>

#include "tablemodelfilesintorrent.h"

namespace Ui {
class FrameTorrent;
}

class FrameTorrent : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTorrent(QWidget *parent = nullptr);
    ~FrameTorrent();

    void set_data(torrent_metadata_t &torrent_metadata);

private:
    Ui::FrameTorrent *ui;
    tablemodelfilesintorrent model;
    QVector<file_in_torrent> files_in_torrent;
};

#endif // FRAMETORRENT_H

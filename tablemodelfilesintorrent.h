#ifndef TABLEMODELFILESINTORRENT_H
#define TABLEMODELFILESINTORRENT_H

#include <QAbstractTableModel>

#include "datastructdefines.h"

class tablemodelfilesintorrent : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit tablemodelfilesintorrent(QObject *parent = nullptr);
    void add_data_source(QVector<file_in_torrent> *p);
    void begin_update_data();
    void end_update_data();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<file_in_torrent> *p_data;
};

#endif // TABLEMODELFILESINTORRENT_H

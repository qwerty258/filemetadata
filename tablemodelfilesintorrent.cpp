#include "tablemodelfilesintorrent.h"

tablemodelfilesintorrent::tablemodelfilesintorrent(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void tablemodelfilesintorrent::add_data_source(QVector<file_in_torrent> *p)
{
    p_data = p;
}

void tablemodelfilesintorrent::begin_update_data()
{
    layoutAboutToBeChanged();
}

void tablemodelfilesintorrent::end_update_data()
{
    layoutChanged();
}

QVariant tablemodelfilesintorrent::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header_name;
    if ((Qt::Orientation::Horizontal == orientation) && (Qt::DisplayRole == role))
    {
        switch (section)
        {
        case 0:
            header_name.setValue(tr("File Path"));
            break;
        case 1:
            header_name.setValue(tr("File Size"));
            break;
        default:
            break;
        }
    }

    return header_name;
}

int tablemodelfilesintorrent::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return p_data->count();
}

int tablemodelfilesintorrent::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant tablemodelfilesintorrent::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QVariant value;

    if (Qt::DisplayRole == role)
    {
        switch (index.column())
        {
        case 0:
            value.setValue((*p_data)[index.row()].path);
            break;
        case 1:
            value.setValue((*p_data)[index.row()].length);
            break;
        default:
            break;
        }
    }
    return value;
}

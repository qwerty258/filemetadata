#include "tablemodelcorruptedfils.h"

TableModelCorruptedFils::TableModelCorruptedFils(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void TableModelCorruptedFils::add_data_source(QVector<corrupted_file_t> *p)
{
    p_data = p;
}

void TableModelCorruptedFils::begin_update_data()
{
    layoutAboutToBeChanged();
}

void TableModelCorruptedFils::end_update_data()
{
    layoutChanged();
}

QVariant TableModelCorruptedFils::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header_name;
    if ((Qt::Orientation::Horizontal == orientation) && (Qt::DisplayRole == role))
    {
        switch (section)
        {
        case 0:
            header_name.setValue(QString(tr("file_id")));
            break;
        case 1:
            header_name.setValue(QString(tr("file_name")));
            break;
        case 2:
            header_name.setValue(QString(tr("size")));
            break;
        case 3:
            header_name.setValue(QString(tr("sha1")));
            break;
        default:
            break;
        }
    }

    return header_name;
}

int TableModelCorruptedFils::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return p_data->size();
}

int TableModelCorruptedFils::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant TableModelCorruptedFils::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (!index.isValid())
        return value;

    if (Qt::DisplayRole == role)
    {
        switch (index.column())
        {
        case 0:
            value.setValue((*p_data)[index.row()].file_id);
            break;
        case 1:
            value.setValue((*p_data)[index.row()].file_name);
            break;
        case 2:
            value.setValue((*p_data)[index.row()].size);
            break;
        case 3:
            value.setValue((*p_data)[index.row()].sha1);
            break;
        default:
            break;
        }
    }
    return value;
}

#include "tablemodelnewfiles.h"

TableModelNewFiles::TableModelNewFiles(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void TableModelNewFiles::add_dialog_copy_file(QVector<new_file_info_t>* p)
{
    p_data = p;
}

void TableModelNewFiles::begin_update_data()
{
    layoutAboutToBeChanged();
}

void TableModelNewFiles::end_update_data()
{
    layoutChanged();
}

QVariant TableModelNewFiles::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header_name;
    if ((Qt::Orientation::Horizontal == orientation) && (Qt::DisplayRole == role))
    {
        switch (section)
        {
        case 0:
            header_name.setValue(tr("Full Path"));
            break;
        case 1:
            header_name.setValue(tr("File Name"));
            break;
        case 2:
            header_name.setValue(tr("SHA1"));
            break;
        case 3:
            header_name.setValue(tr("Size"));
            break;
        default:
            break;
        }
    }

    return header_name;
}

int TableModelNewFiles::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return p_data->size();
}

int TableModelNewFiles::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant TableModelNewFiles::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (!index.isValid())
        return value;

    if (Qt::DisplayRole == role)
    {
        switch (index.column())
        {
        case 0:
            value.setValue((*p_data)[index.row()].full_path);
            break;
        case 1:
            value.setValue((*p_data)[index.row()].file_name);
            break;
        case 2:
            value.setValue((*p_data)[index.row()].sha1);
            break;
        case 3:
            value.setValue((*p_data)[index.row()].size);
            break;
        default:
            break;
        }
    }
    return value;
}

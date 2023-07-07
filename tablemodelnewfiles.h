#ifndef TABLEMODELNEWFILES_H
#define TABLEMODELNEWFILES_H

#include <QAbstractTableModel>

#include "datastructdefines.h"

class TableModelNewFiles : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModelNewFiles(QObject *parent = nullptr);
    void add_dialog_copy_file(QVector<new_file_info_t>* p);
    void begin_update_data();
    void end_update_data();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:

    QVector<new_file_info_t>* p_data;
};

#endif // TABLEMODELNEWFILES_H

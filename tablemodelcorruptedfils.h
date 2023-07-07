#ifndef TABLEMODELCORRUPTEDFILS_H
#define TABLEMODELCORRUPTEDFILS_H

#include <QAbstractTableModel>

#include "datastructdefines.h"

class TableModelCorruptedFils : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModelCorruptedFils(QObject *parent = nullptr);
    void add_data_source(QVector<corrupted_file_t> *p);
    void begin_update_data();
    void end_update_data();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<corrupted_file_t> *p_data;
};

#endif // TABLEMODELCORRUPTEDFILS_H

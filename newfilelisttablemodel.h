#ifndef NEWFILELISTTABLEMODEL_H
#define NEWFILELISTTABLEMODEL_H

#include <QAbstractTableModel>

#include "newfileslistdatastruct.h"

class NewFileListTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit NewFileListTableModel(QObject *parent = nullptr);
    void add_dialog_copy_file(QVector<basic_file_metadata_t>* p);
    void begin_update_data();
    void end_update_data();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:

    QVector<basic_file_metadata_t>* p_data;
};

#endif // NEWFILELISTTABLEMODEL_H

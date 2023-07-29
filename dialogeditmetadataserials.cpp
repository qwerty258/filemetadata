#include <QMenu>

#include "dialogeditmetadataserials.h"
#include "ui_dialogeditmetadataserials.h"

DialogEditMetadataSerials::DialogEditMetadataSerials(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditMetadataSerials)
{
    ui->setupUi(this);
    ui->tableViewEditSerials->setContextMenuPolicy(Qt::CustomContextMenu);
    p_table_serials_model = new table_model("serials");
    ui->tableViewEditSerials->setModel(p_table_serials_model->get_table_model());
    ui->tableViewEditSerials->resizeColumnsToContents();
}

DialogEditMetadataSerials::~DialogEditMetadataSerials()
{
    delete ui;
    p_table_serials_model->table_sync();
    delete p_table_serials_model;
}

void DialogEditMetadataSerials::on_tableViewEditSerials_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *action_delete = new QAction("Delete", this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_tableViewEditSerials_customContextMenuRequested_action_delete()));

    menu->popup(ui->tableViewEditSerials->viewport()->mapToGlobal(pos));
}

void DialogEditMetadataSerials::on_tableViewEditSerials_customContextMenuRequested_action_delete()
{
    QModelIndexList index_list = ui->tableViewEditSerials->selectionModel()->selectedIndexes();

    for (int i = 0; i < index_list.count(); i++)
    {
        p_table_serials_model->table_delete_record(index_list[i].row());
    }
    p_table_serials_model->table_select();
    ui->tableViewEditSerials->resizeColumnsToContents();
}

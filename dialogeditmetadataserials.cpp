#include <QMenu>

#include "dialogeditmetadataserials.h"
#include "ui_dialogeditmetadataserials.h"

#include "databasesqlite.h"

DialogEditMetadataSerials::DialogEditMetadataSerials(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditMetadataSerials)
{
    ui->setupUi(this);
    ui->tableViewEditSerials->setContextMenuPolicy(Qt::CustomContextMenu);
    database_table_serials_create_model();
    database_table_serials_add_model_to_view(ui->tableViewEditSerials);
    ui->tableViewEditSerials->resizeColumnsToContents();
}

DialogEditMetadataSerials::~DialogEditMetadataSerials()
{
    delete ui;
    database_table_serials_submit();
    database_table_serials_delete_model();
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
        database_table_serials_delete_record(index_list[i].row());
    }
    database_table_serials_select();
    ui->tableViewEditSerials->resizeColumnsToContents();
}

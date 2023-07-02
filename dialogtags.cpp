#include <QMenu>
#include <QMessageBox>

#include "dialogtags.h"
#include "ui_dialogtags.h"

#include "databasesqlite.h"

#include <QDebug>

DialogTags::DialogTags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTags)
{
    ui->setupUi(this);
    ui->tableViewTags->setContextMenuPolicy(Qt::CustomContextMenu);
    database_table_tages_add_model_to_view(ui->tableViewTags);
    ui->tableViewTags->setColumnHidden(0, true);
}

DialogTags::~DialogTags()
{
    delete ui;
    database_delete_table_tags_model();
}

void DialogTags::on_pushButtonAddTag_clicked()
{
    if (!ui->lineEditTag->text().trimmed().remove('\n').remove('\r').isEmpty())
        database_table_tages_add_tag(ui->lineEditTag->text().trimmed().remove('\n').remove('\r'));
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText(tr("Please enter valid tag name"));
        msg.exec();
    }
    ui->lineEditTag->clear();
}

void DialogTags::on_tableViewTags_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *action_delete = new QAction("Delete", this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_tableViewTags_customContextMenuRequested_action_delete()));

    menu->popup(ui->tableViewTags->viewport()->mapToGlobal(pos));
}

void DialogTags::on_tableViewTags_customContextMenuRequested_action_delete()
{
    QModelIndexList index_list = ui->tableViewTags->selectionModel()->selectedIndexes();

    for (int i = 0; i < index_list.count(); i++)
    {
        database_table_tags_delete(index_list[i].row());
    }
    database_table_tags_delete_refresh();
}
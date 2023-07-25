#include <QMenu>
#include <QMessageBox>
#include <QSettings>

#include "dialogeditmetadatatags.h"
#include "ui_dialogeditmetadatatags.h"

#include "databasesqlite.h"

#include <QDebug>

extern QSettings global_settings;

DialogEditMetadataTags::DialogEditMetadataTags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditMetadataTags)
{
    global_settings.beginGroup("UI");
    bool pro_mode = global_settings.value("pro_mode", "").toBool();
    global_settings.endGroup();

    ui->setupUi(this);
    ui->tableViewEditTags->setContextMenuPolicy(Qt::CustomContextMenu);
    database_table_tags_add_model_to_view(ui->tableViewEditTags);
    if (!pro_mode)
    {
        ui->tableViewEditTags->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewEditTags->setColumnHidden(0, true);
    }
    ui->tableViewEditTags->sortByColumn(1, Qt::SortOrder::AscendingOrder);
    ui->tableViewEditTags->resizeColumnsToContents();
}

DialogEditMetadataTags::~DialogEditMetadataTags()
{
    delete ui;
    database_table_tags_delete_model();
}

void DialogEditMetadataTags::on_pushButtonAddTag_clicked()
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
    ui->tableViewEditTags->resizeColumnsToContents();
}

void DialogEditMetadataTags::on_tableViewEditTags_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *action_delete = new QAction("Delete", this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_tableViewEditTags_customContextMenuRequested_action_delete()));

    menu->popup(ui->tableViewEditTags->viewport()->mapToGlobal(pos));
}

void DialogEditMetadataTags::on_tableViewEditTags_customContextMenuRequested_action_delete()
{
    QModelIndexList index_list = ui->tableViewEditTags->selectionModel()->selectedIndexes();

    for (int i = 0; i < index_list.count(); i++)
    {
        qDebug() << index_list[i].row();
        database_table_tags_delete(index_list[i].row());
    }
    database_table_tags_model_select();
    ui->tableViewEditTags->resizeColumnsToContents();
}

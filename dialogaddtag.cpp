#include "dialogaddtag.h"
#include "ui_dialogaddtag.h"

#include <QMessageBox>
#include <QDebug>

DialogAddTag::DialogAddTag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddTag)
{
    ui->setupUi(this);
    p_table_tags_model = new table_model("tags");
    p_table_tag_file_join_model = new table_model("tag_file_join");
    p_table_tags_model->get_table_model()->sort(1, Qt::SortOrder::AscendingOrder);
    ui->comboBox->setModel(p_table_tags_model->get_table_model());
    ui->comboBox->setModelColumn(1);
}

DialogAddTag::~DialogAddTag()
{
    delete ui;
    p_table_tag_file_join_model->table_sync();
    delete p_table_tag_file_join_model;
    delete p_table_tags_model;
}

void DialogAddTag::on_pushButtonOK_clicked()
{
    if (-1 == ui->comboBox->findText(ui->comboBox->currentText()))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText(tr("The text entered is not in the list. If you want to add new tag please go to \"Edit\" -> \"Metadata Tags\""));
        msg.exec();
        return;
    }

    QModelIndexList index_list = p_table_files_table_view->selectionModel()->selectedIndexes();

    for(int i = 0; i < index_list.count(); i++)
    {
        quint64 tag_id = p_table_tags_model->get_table_model()->index(ui->comboBox->currentIndex(), 0).data().toULongLong();
        quint64 file_id = p_table_files_table_view->model()->index(index_list[i].row(), 0).data().toULongLong();
        p_table_tag_file_join_model->table_tag_file_join_add(tag_id, file_id);
    }

    close();
}

void DialogAddTag::on_pushButtonCancel_clicked()
{
    close();
}

void DialogAddTag::add_table_files_table_view(QTableView *p)
{
    p_table_files_table_view = p;
}

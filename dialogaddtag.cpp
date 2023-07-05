#include "dialogaddtag.h"
#include "ui_dialogaddtag.h"

#include "databasesqlite.h"

#include <QMessageBox>
#include <QDebug>

DialogAddTag::DialogAddTag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddTag)
{
    ui->setupUi(this);
    database_table_tags_add_model_to_combobox(ui->comboBox);
    ui->comboBox->setModelColumn(1);
}

DialogAddTag::~DialogAddTag()
{
    delete ui;
    database_delete_table_tags_model();
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
        database_table_tag_file_join_add(ui->comboBox->currentIndex(), index_list[i].row());
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

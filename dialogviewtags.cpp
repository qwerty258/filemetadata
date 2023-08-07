#include "dialogviewtags.h"
#include "ui_dialogviewtags.h"

Dialogviewtags::Dialogviewtags(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogviewtags)
{
    ui->setupUi(this);
    p_table_tags_model = new table_model("tags");
    p_table_tags_model->get_table_model()->sort(1, Qt::SortOrder::AscendingOrder);
    ui->combo_box_tags->setModel(p_table_tags_model->get_table_model());
    ui->combo_box_tags->setModelColumn(1);
}

Dialogviewtags::~Dialogviewtags()
{
    delete ui;
    delete p_table_tags_model;
    p_table_files_model->clear_match();
}

void Dialogviewtags::add_table_model_files(table_model *p)
{
    p_table_files_model = p;
    ui->table_view_files->setModel(p_table_files_model->get_table_model());
    ui->table_view_files->resizeColumnsToContents();
}

void Dialogviewtags::on_push_button_filter_clicked()
{
    QSqlTableModel *p_qsql_table_files_model = p_table_tags_model->get_table_model();
    QModelIndex tmp = p_qsql_table_files_model->index(ui->combo_box_tags->currentIndex(), 0);
    quint64 tag_id = tmp.data().toULongLong();
    p_table_files_model->table_files_tag_match(tag_id);
    ui->table_view_files->resizeColumnsToContents();
}

void Dialogviewtags::on_push_button_clear_clicked()
{
    p_table_files_model->clear_match();
    ui->table_view_files->resizeColumnsToContents();
}

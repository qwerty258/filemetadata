#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"
#include "dialogcheckcorruption.h"
#include "dialogimportfiles.h"
#include "dialogeditmetadatatags.h"
#include "dialogeditmetadataserials.h"
#include "dialogaddtag.h"
#include "dialogviewtorrents.h"
#include "dialogabout.h"

#include "fileoperation.h"

extern QSettings global_settings;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (0 != database_init())
    {
        this->close();
    }

    global_settings.beginGroup("UI");
    bool pro_mode = global_settings.value("pro_mode", "").toBool();
    global_settings.endGroup();

    ui->table_view->setContextMenuPolicy(Qt::CustomContextMenu);
    p_table_files_model = new table_model("files");
    ui->table_view->setModel(p_table_files_model->get_table_model());
    if (!pro_mode)
    {
        ui->table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_view->setColumnHidden(0, true);
        ui->table_view->setColumnHidden(2, true);
        ui->table_view->setColumnHidden(3, true);
    }
    ui->table_view->setSortingEnabled(true);
    ui->table_view->sortByColumn(1, Qt::SortOrder::AscendingOrder);
    ui->table_view->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
    p_table_files_model->table_sync();
    delete p_table_files_model;
    database_uninit();
}

void MainWindow::on_actionSettings_triggered()
{
    DialogGlobalSettings settings;
    settings.exec();
}

void MainWindow::on_actionCheck_Cruption_triggered()
{
    DialogCheckCorruption check_corruption;
    check_corruption.add_table_files_model(p_table_files_model);
    check_corruption.exec();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionCopy_Files_In_triggered()
{
    DialogImportFiles import_file;
    import_file.add_table_files_model(p_table_files_model);
    import_file.exec();
}

void MainWindow::on_actionTags_triggered()
{
    DialogEditMetadataTags tags;
    tags.exec();
}

void MainWindow::on_actionMetadata_Serials_triggered()
{
    DialogEditMetadataSerials serials;
    serials.exec();
}

void MainWindow::on_actionView_Torrents_triggered()
{
    DialogViewTorrents view_torrents;
    view_torrents.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout about;
    about.exec();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    QString tmp = ui->lineEditSearchTerm->text().trimmed().remove('\r').remove('\n');
    p_table_files_model->table_files_match(tmp);
    p_table_files_model->table_select();
}

void MainWindow::on_pushButtonClearSearch_clicked()
{
    ui->lineEditSearchTerm->clear();
    p_table_files_model->clear_match();
    p_table_files_model->table_select();
}

void MainWindow::on_table_view_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);

    QAction* action_add_tag = new QAction("Add Tag", this);
    menu->addAction(action_add_tag);
    connect(action_add_tag, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_add_tag()));

    QAction* action_replace = new QAction("Replace File", this);
    menu->addAction(action_replace);
    connect(action_replace, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_replace()));

    QAction* action_export = new QAction("Export", this);
    menu->addAction(action_export);
    connect(action_export, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_export()));

    QAction* action_delete = new QAction("Delete", this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_delete()));

    menu->popup(ui->table_view->viewport()->mapToGlobal(pos));
}

void MainWindow::on_table_view_customContextMenuRequested_action_add_tag()
{
    DialogAddTag addtag;
    addtag.add_table_files_table_view(ui->table_view);
    addtag.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_replace()
{
    QMessageBox msg;
    msg.setText("replace not implement");
    msg.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_export()
{
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    QString path =
        QFileDialog::getExistingDirectory(
            this,
            tr("Open Directory"),
            QStandardPaths::displayName(QStandardPaths::HomeLocation),
            QFileDialog::ShowDirsOnly);

    QModelIndexList index_list = ui->table_view->selectionModel()->selectedIndexes();

    QString filename;
    QString sha1;
    for (int i = 0; i < index_list.count(); i++)
    {
        p_table_files_model->table_files_get_file_info(index_list[i].row(), filename, sha1);
        file_operation_export_file(database_root_path, filename, sha1, path);
    }
}

void MainWindow::on_table_view_customContextMenuRequested_action_delete()
{
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    QModelIndexList index_list = ui->table_view->selectionModel()->selectedIndexes();

    qDebug() << "index size:" << index_list.size();

    QString sha1;
    for(int i = 0; i < index_list.count(); i++)
    {
        if (p_table_files_model->table_files_delete_record(index_list[i].row(), sha1))
        {
            file_operation_delete_file(database_root_path, sha1);
        }
    }
    p_table_files_model->table_select();
}

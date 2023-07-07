#include <QMessageBox>
#include <qsettings.h>
#include <QFileInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"
#include "dialogcheckcorruption.h"
#include "dialogcopyfilesin.h"
#include "dialogtags.h"
#include "dialogaddtag.h"
#include "dialogabout.h"

#include "databasesqlite.h"

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
    database_add_model_to_view(ui->table_view);
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
    database_delete_model();
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
    check_corruption.exec();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionCopy_Files_In_triggered()
{
    DialogCopyFilesIn copyfilein;
    copyfilein.exec();
}

void MainWindow::on_actionTags_triggered()
{
    DialogTags tags;
    tags.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout about;
    about.exec();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    database_table_files_match_name(
        ui->lineEditSearchTerm->text().trimmed().remove('\r').remove('\n'));
    database_delete_file_record_refresh();
}

void MainWindow::on_pushButtonClearSearch_clicked()
{
    database_table_files_clear_match();
    database_delete_file_record_refresh();
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
    QMessageBox msg;
    msg.setText("export not implement");
    msg.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_delete()
{
    global_settings.beginGroup("database");
    QString database_root_path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();

    QModelIndexList index_list = ui->table_view->selectionModel()->selectedIndexes();

    qDebug() << "index size:" << index_list.size();

    for(int i = 0; i < index_list.count(); i++)
    {
        database_delete_file_record(database_root_path, index_list[i].row());
    }
    database_delete_file_record_refresh();
}

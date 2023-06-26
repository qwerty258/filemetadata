#include <QMessageBox>
#include <qsettings.h>
#include <QFileInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"
#include "dialogcopyfilesin.h"

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
    ui->table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_view->setContextMenuPolicy(Qt::CustomContextMenu);
    database_add_model_to_view(ui->table_view);
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

void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionCopy_Files_In_triggered()
{
    DialogCopyFilesIn copyfilein;
    copyfilein.exec();
}

void MainWindow::on_table_view_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->table_view->indexAt(pos);
    qDebug() << "context menuc";
    qDebug() << index.row();
    QMenu* menu = new QMenu(this);

    QAction* action_replace = new QAction("Replace", this);
    menu->addAction(action_replace);
    connect(action_replace, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_replace()));

    QAction* action_edit = new QAction("Edit Metadata", this);
    menu->addAction(action_edit);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_edit()));

    QAction* action_export = new QAction("Export", this);
    menu->addAction(action_export);
    connect(action_export, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_export()));

    QAction* action_delete = new QAction("Delete", this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_table_view_customContextMenuRequested_action_delete()));

    menu->popup(ui->table_view->viewport()->mapToGlobal(pos));
}

void MainWindow::on_table_view_customContextMenuRequested_action_replace()
{
    QMessageBox msg;
    msg.setText("replace not implement");
    msg.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_edit()
{
    QMessageBox msg;
    msg.setText("edit not implement");
    msg.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_export()
{
    QMessageBox msg;
    msg.setText("export not implement");
    msg.exec();
}

void MainWindow::on_table_view_customContextMenuRequested_action_delete(void)
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

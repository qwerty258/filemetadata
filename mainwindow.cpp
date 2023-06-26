#include <QMessageBox>

#include "databasesqlite.h"

#include <QFileInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"
#include "dialogcopyfilesin.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (0 != database_init())
    {
        this->close();
    }
    database_add_model_to_view(ui->table_view);
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

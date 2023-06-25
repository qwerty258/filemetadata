#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"

#define DATABASE_VERSION 0

extern QSettings global_settings;

QSqlDatabase db;
QSqlTableModel *model=NULL;

int exec_sql_file(QString path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file error";
    }
    QTextStream in(&f);
    QSqlQuery query(db);
    QString sql = in.readAll();
    QStringList sqls = sql.trimmed().remove('\n').split(';');

    for (QStringList::Iterator it = sqls.begin(); it != sqls.end(); it++)
    {
        QString tmp = (*it);
        qDebug() << "SQL: " << tmp;
        if(tmp.isEmpty())
            continue;
        if(!query.exec(*it + ';'))
        {
            QMessageBox msg;
            msg.setText("database_000000.sql: " + *it + "ERROR: " + query.lastError().text());
            msg.exec();
            return -1;
        }
    }

    return 0;
}

int database_init(void)
{
    global_settings.beginGroup("database");
    QString path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    if( QFileInfo(path).isDir  () )
    {
        path+="/FileMetadata.db";
        db = QSqlDatabase::addDatabase("QSQLITE");;
        db.setDatabaseName(path);
        qDebug()<<path;
        if(!db.open())
        {
            qDebug()<< "db open error";
            qDebug() << db.lastError().text();
        }
        if(db.tables().contains("version"))
        {
            QSqlQuery query(db);
            if(query.exec("SELECT * FROM version;"))
            {
                query.next();
                unsigned int version = query.value(0).toUInt();
                qDebug() << "database version: " << version;
                switch (version) {
                case 0:
                    goto DATABASE_VERSION_1_UPDATE;
                    break;
                default:
                    break;
                }
DATABASE_VERSION_1_UPDATE:
                qDebug() << "database version: " << version;
            }
            else
            {
                qDebug() << "SELECT version FROM version; ERROR";
                qDebug() <<db.lastError().text();
                return -1;
            }
        }
        else
        {
            // create tables
            if(0!= exec_sql_file(":/database_000000.sql"))
                return -1;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText("database location invalid.");
        msg.exec();
        return -1;
    }
    return 0;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (0 != database_init())
    {
        this->close();
    }
    model = new QSqlTableModel(NULL,db);
    model->setTable("files");
    if(model->select())
    {
        ui->table_view->setModel(model);
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText(model->lastError().text());
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
        this->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(NULL != model)
    {
        delete model;
    }
    if(db.isOpen())
    {
        db.close();
    }
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


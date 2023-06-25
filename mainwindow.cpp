#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogglobalsettings.h"

#define DATABASE_VERSION 1

extern QSettings global_settings;

QSqlDatabase db;

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
                case 1:
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
            }
        }
        else
        {
            // create tables
            QSqlQuery query(db);
            QStringList sqllist;
            sqllist.push_back(QString("CREATE TABLE IF NOT EXISTS version (version_id INTEGER);"));

            sqllist.push_back(QString("INSERT INTO version (version_id) VALUES (") +
                              QString::number(DATABASE_VERSION) + ");" );

            sqllist.push_back(QString("CREATE TABLE IF NOT EXISTS files (") +
                              "file_id INTEGER PRIMARY KEY ," +
                              "file_name TEXT DEFAULT \"\","
                              "file_size INTEGER DEFAULT 0,"
                              "file_sha1sum TEXT DEFAULT \"\","
                              "file_metadata_type INTEGER DEFAULT 0,"
                              "file_trash_state INTEGER DEFAULT 0);");

            sqllist.push_back(QString("CREATE TABLE IF NOT EXISTS books (") +
                              "book_id INTEGER PRIMARY KEY ," +
                              "file_id INTEGER," +
                              "author TEXT NOT NULL DEFAULT \"\"," +
                              "secondary_author TEXT NOT NULL DEFAULT \"\"," +
                              "subsidiary_author TEXT NOT NULL DEFAULT \"\"," +
                              "year INTEGER DEFAULT \"\"," +
                              "title TEXT NOT NULL DEFAULT \"\"," +
                              "secondary_title TEXT NOT NULL DEFAULT \"\"," +
                              "pages TEXT NOT NULL DEFAULT \"\"," +
                              "volume TEXT NOT NULL DEFAULT \"\"," +
                              "number TEXT NOT NULL DEFAULT \"\"," +
                              "number_of_volumes TEXT NOT NULL DEFAULT \"\"," +
                              "place_published TEXT NOT NULL DEFAULT \"\"," +
                              "publisher TEXT NOT NULL DEFAULT \"\"," +
                              "edition TEXT NOT NULL DEFAULT \"\"," +
                              "keywords TEXT NOT NULL DEFAULT \"\"," +
                              "abstract TEXT NOT NULL DEFAULT \"\"," +
                              "url TEXT NOT NULL DEFAULT \"\"," +
                              "tertiary_title TEXT NOT NULL DEFAULT \"\"," +
                              "tertiary_author TEXT NOT NULL DEFAULT \"\"," +
                              "isbn TEXT NOT NULL DEFAULT \"\"," +
                              "alternate_title TEXT NOT NULL DEFAULT \"\"," +
                              "accession_number TEXT NOT NULL DEFAULT \"\"," +
                              "call_number TEXT NOT NULL DEFAULT \"\"," +
                              "short_title TEXT NOT NULL DEFAULT \"\"," +
                              "section TEXT NOT NULL DEFAULT \"\"," +
                              "original_publication TEXT NOT NULL DEFAULT \"\"," +
                              "reprint_edition TEXT NOT NULL DEFAULT \"\"," +
                              "electronic_resource_number TEXT NOT NULL DEFAULT \"\"," +
                              "translated_author TEXT NOT NULL DEFAULT \"\"," +
                              "translated_title TEXT NOT NULL DEFAULT \"\"," +
                              "language TEXT NOT NULL DEFAULT \"\"," +
                              "record_properties TEXT NOT NULL DEFAULT \"\"," +
                              "record_last_updated INTEGER NOT NULL DEFAULT 0);");

            sqllist.push_back(QString("CREATE TABLE IF NOT EXISTS journal_article (") +
                              "journal_article_id INTEGER PRIMARY KEY ," +
                              "file_id INTEGER," +
                              "author TEXT NOT NULL DEFAULT \"\"," +
                              "year INTEGER DEFAULT \"\"," +
                              "title TEXT NOT NULL DEFAULT \"\"," +
                              "journal TEXT NOT NULL DEFAULT \"\"," +
                              "volume TEXT NOT NULL DEFAULT \"\"," +
                              "issue TEXT NOT NULL DEFAULT \"\"," +
                              "pages TEXT NOT NULL DEFAULT \"\"," +
                              "start_page TEXT NOT NULL DEFAULT \"\"," +
                              "issn TEXT NOT NULL DEFAULT \"\"," +
                              "doi TEXT NOT NULL DEFAULT \"\"," +
                              "keywords TEXT NOT NULL DEFAULT \"\"," +
                              "abstract TEXT NOT NULL DEFAULT \"\"," +
                              "author_address TEXT NOT NULL DEFAULT \"\"," +
                              "language TEXT NOT NULL DEFAULT \"\");");

            for (QStringList::Iterator it = sqllist.begin(); it != sqllist.end(); it++)
            {
                if(!query.exec(*it))
                {
                    QMessageBox msg;
                    msg.setText("SQL " + *it + "ERROR");
                    msg.exec();
                }
            }
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText("database location invalid.");
        msg.exec();
    }
    return 0;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    database_init();
}

MainWindow::~MainWindow()
{
    delete ui;
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


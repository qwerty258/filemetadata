#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include "dialogglobalsettings.h"
#include "ui_dialogglobalsettings.h"

extern QSettings global_settings;

DialogGlobalSettings::DialogGlobalSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGlobalSettings)
{
    ui->setupUi(this);
    global_settings.beginGroup("database");
    QString temp = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    qDebug() << temp;
    ui->lineEditDataPath->setText(temp);
}

DialogGlobalSettings::~DialogGlobalSettings()
{
    delete ui;
}

void DialogGlobalSettings::on_pushButtonChange_clicked()
{
    QString path = ui->lineEditDataPath->text();
    if( QFileInfo(path).isDir())
    {
        path= QFileDialog::getExistingDirectory(this,"get path", path);
    }
    else
    {
        path= QFileDialog::getExistingDirectory(this,"get path");
    }
    ui->lineEditDataPath->setText(path);
}

void DialogGlobalSettings::on_pushButtonSave_clicked()
{
    global_settings.beginGroup("database");
    global_settings.setValue("database_location", ui->lineEditDataPath->text());
    global_settings.endGroup();
    global_settings.sync();
    QMessageBox msg;
    msg.setText("Please restart the app for the setting to become effective.");
    msg.exec();
}

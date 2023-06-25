#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include "dialogglobalsettings.h"
#include "ui_dialogglobalsettings.h"

extern QSettings* p_global_settings;

DialogGlobalSettings::DialogGlobalSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGlobalSettings)
{
    ui->setupUi(this);
    p_global_settings->beginGroup("database");
    QString temp  = p_global_settings->value("database_location","").toString();
    p_global_settings->endGroup();
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
    p_global_settings->beginGroup("database");
    p_global_settings->setValue("database_location", ui->lineEditDataPath->text());
    p_global_settings->endGroup();
    p_global_settings->sync();
    QMessageBox msg;
    msg.setText("Please restart the app for the setting to become effective.");
    msg.exec();
}

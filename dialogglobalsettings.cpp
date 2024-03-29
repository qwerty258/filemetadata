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
    QString data_location = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    global_settings.beginGroup("UI");
    bool pro_mode = global_settings.value("pro_mode", "").toBool();
    bool copy_as_default = global_settings.value("copy_as_default", "").toBool();
    bool autometadata_as_default = global_settings.value("auto_add_metadata_as_default", "").toBool();
    global_settings.endGroup();
    ui->lineEditDataPath->setText(data_location);
    ui->checkBoxProMode->setCheckState(pro_mode ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->checkBoxDefaultCopy->setCheckState(copy_as_default ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->check_box_autometadata_as_default->setCheckState(
        autometadata_as_default ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
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
    global_settings.beginGroup("UI");
    bool pro_mode = ui->checkBoxProMode->checkState() == Qt::CheckState::Checked;
    global_settings.setValue("pro_mode", pro_mode);
    bool copy_as_default = ui->checkBoxDefaultCopy->checkState() == Qt::CheckState::Checked;
    global_settings.setValue("copy_as_default", copy_as_default);
    global_settings.setValue(
        "auto_add_metadata_as_default",
        ui->check_box_autometadata_as_default->checkState() == Qt::CheckState::Checked);
    global_settings.endGroup();
    global_settings.sync();
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setText("Please restart the app for the setting to become effective.");
    msg.exec();
    close();
}

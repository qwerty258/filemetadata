#include "dialogcopyfilesin.h"
#include "ui_dialogcopyfilesin.h"

DialogCopyFilesIn::DialogCopyFilesIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCopyFilesIn)
{
    ui->setupUi(this);
}

DialogCopyFilesIn::~DialogCopyFilesIn()
{
    delete ui;
}

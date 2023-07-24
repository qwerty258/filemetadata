#include "frameserial.h"
#include "ui_frameserial.h"

FrameSerial::FrameSerial(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSerial)
{
    ui->setupUi(this);
}

FrameSerial::~FrameSerial()
{
    delete ui;
}

void FrameSerial::set_data(serial_metadata_t &serial_metadata)
{
    ui->lineEditAbstract->setText(serial_metadata.abstract);
    ui->lineEditAuthor->setText(serial_metadata.author);
    ui->lineEditAbstract->setText(serial_metadata.author_address);
    ui->lineEditDOI->setText(serial_metadata.doi);
    ui->lineEditISSN->setText(serial_metadata.issn);
    ui->lineEditIssue->setText(serial_metadata.issue);
    ui->lineEditJournal->setText(serial_metadata.journal);
    ui->lineEditKeywords->setText(serial_metadata.keywords);
    ui->lineEditLanguage->setText(serial_metadata.language);
    ui->lineEditPages->setText(serial_metadata.pages);
    ui->lineEditStartPage->setText(serial_metadata.start_page);
    ui->lineEditTitle->setText(serial_metadata.title);
    ui->lineEditVolume->setText(serial_metadata.volume);
    QDate date;
    date.setDate(serial_metadata.year.toInt(), 0, 0);
    ui->dateEdit->setDate(date);
}

void FrameSerial::get_data(serial_metadata_t &serial_metadata)
{
    serial_metadata.abstract = ui->lineEditAbstract->text();
    serial_metadata.author = ui->lineEditAuthor->text();
    serial_metadata.author_address = ui->lineEditAuthorAddress->text();
    serial_metadata.doi = ui->lineEditDOI->text();
    serial_metadata.issn = ui->lineEditISSN->text();
    serial_metadata.issue = ui->lineEditIssue->text();
    serial_metadata.journal = ui->lineEditJournal->text();
    serial_metadata.keywords = ui->lineEditKeywords->text();
    serial_metadata.language = ui->lineEditLanguage->text();
    serial_metadata.pages = ui->lineEditPages->text();
    serial_metadata.start_page = ui->lineEditStartPage->text();
    serial_metadata.title = ui->lineEditTitle->text();
    serial_metadata.volume = ui->lineEditVolume->text();
    serial_metadata.year = QString::number(ui->dateEdit->date().year());
}

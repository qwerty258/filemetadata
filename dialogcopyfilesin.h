#ifndef DIALOGCOPYFILESIN_H
#define DIALOGCOPYFILESIN_H

#include <QDialog>
#include <QString>
#include <QVector>

namespace Ui {
class DialogCopyFilesIn;
}

class DialogCopyFilesIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCopyFilesIn(QWidget *parent = nullptr);
    ~DialogCopyFilesIn();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogCopyFilesIn *ui;
    QStringList files;
    QVector<QString> sha1;
    QVector<qint64> size;

};

#endif // DIALOGCOPYFILESIN_H

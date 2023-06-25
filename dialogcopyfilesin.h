#ifndef DIALOGCOPYFILESIN_H
#define DIALOGCOPYFILESIN_H

#include <QDialog>

namespace Ui {
class DialogCopyFilesIn;
}

class DialogCopyFilesIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCopyFilesIn(QWidget *parent = nullptr);
    ~DialogCopyFilesIn();

private:
    Ui::DialogCopyFilesIn *ui;
};

#endif // DIALOGCOPYFILESIN_H

#ifndef DIALOGVIEWTORRENTS_H
#define DIALOGVIEWTORRENTS_H

#include <QDialog>

namespace Ui {
class DialogViewTorrents;
}

class DialogViewTorrents : public QDialog
{
    Q_OBJECT

public:
    explicit DialogViewTorrents(QWidget *parent = nullptr);
    ~DialogViewTorrents();

private:
    Ui::DialogViewTorrents *ui;
};

#endif // DIALOGVIEWTORRENTS_H

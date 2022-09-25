#ifndef DIALOGGLOBALSETTINGS_H
#define DIALOGGLOBALSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogGlobalSettings;
}

class DialogGlobalSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGlobalSettings(QWidget *parent = nullptr);
    ~DialogGlobalSettings();

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::DialogGlobalSettings *ui;
};

#endif // DIALOGGLOBALSETTINGS_H

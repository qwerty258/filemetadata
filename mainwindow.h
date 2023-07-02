#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

typedef enum {
    FILE_METADAT_TYPE_NONE = 0,
    FILE_METADAT_TYPE_BOOK = 1,
    FILE_METADAT_TYPE_MAX
} FILE_METADAT_TYPE;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSettings_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_Files_In_triggered();

    void on_actionTags_triggered();

    void on_table_view_customContextMenuRequested(const QPoint &pos);

    void on_table_view_customContextMenuRequested_action_replace();

    void on_table_view_customContextMenuRequested_action_edit();

    void on_table_view_customContextMenuRequested_action_export();

    void on_table_view_customContextMenuRequested_action_delete();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

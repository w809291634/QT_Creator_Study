#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_btn_openFile_clicked();
    void on_btn_openFiles_clicked();
    void on_btn_openDir_clicked();
    void on_btn_save_clicked();

};

#endif // MAINWINDOW_H

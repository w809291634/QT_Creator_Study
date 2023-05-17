#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QSemaphore>
#include <QDebug>
#include "mythread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_QSemaphore_btn_clicked(bool checked);
    void update_QSemaphore_bar(const QString& msg);

private:
    Ui::MainWindow *ui;
};

void app_main(MainWindow* instance);
extern MainWindow* _main_win;

#endif // MAINWINDOW_H

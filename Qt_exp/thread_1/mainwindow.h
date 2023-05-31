#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class MyThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_send_custom_event_clicked();
    void on_send_quit_clicked();
    void on_send_exit_clicked();
    void thread_1_fin();
private:
    MyThread* thread_1;
    Ui::MainWindow *ui;
};

extern MainWindow* _mian_win;
#endif // MAINWINDOW_H

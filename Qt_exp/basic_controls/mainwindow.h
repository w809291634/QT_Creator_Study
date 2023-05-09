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
    void initInterface();
    void inttControl();
private slots:
    /* mainwindow设置当前主页 */
    void on_setCurrentIndex();
    /* 按钮页面槽函数 */
//    void pushbutton_clicked();
//    void toolbutton_clicked();
//    void radiobutton_clicked();

};

#endif // MAINWINDOW_H

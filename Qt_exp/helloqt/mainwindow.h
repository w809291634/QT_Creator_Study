#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;                       //定义 Ui 命令空间中 包含 MainWindow
}

class MainWindow : public QMainWindow       // 公有继承 QMainWindow QMainWindow私有成员不可见
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

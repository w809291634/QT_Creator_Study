#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "ui_form.h"

class MainWindow : public QMainWindow       // 公有继承 QMainWindow QMainWindow私有成员不可见
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);     // 构造函数
    ~MainWindow();

private:
    Ui::MainWindow *ui;                     // 指向 Ui::MainWindow 类的指针，Ui命名空间可能是 mainwindow.ui 中表达的项目
    Ui::Form *ui1;
};

#endif // MAINWINDOW_H

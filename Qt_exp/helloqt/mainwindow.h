#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow       // 公有继承 QMainWindow QMainWindow私有成员不可见
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);     // 构造函数
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;                     // 指向 Ui::MainWindow 类的指针，Ui命名空间可能是 mainwindow.ui 中表达的项目
};

#endif // MAINWINDOW_H

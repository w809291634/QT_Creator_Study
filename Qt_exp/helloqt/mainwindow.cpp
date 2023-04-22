#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),                            // 基类的构造函数，这里 parent 好像每个子类都有
    ui(new Ui::MainWindow),                          // 新建一个指针，指针赋值给 ui
    ui1(new Ui::Form)
{
                           // 调用 Ui::MainWindow 类的 setupUi函数
    ui1->setupUi(this);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

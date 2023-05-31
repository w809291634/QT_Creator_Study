#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include <functional>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread_1 = new MyThread(this);
    auto Fun = bind(&MainWindow::callBackFun,
                    this, placeholders::_1);/*中转一下,利用C++11特性*/
    thread_1->regeditCallBack(Fun);
    thread_1->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::callBackFun(void* data)
{
    while(1){
        qDebug()<<"here";
        thread_1->sleep(1);
    }
}

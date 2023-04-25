#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    from_1.reset(new Form("窗口1","窗口1通知主窗口","窗口1通知窗口2"));
    from_2.reset(new Form("窗口2","窗口2通知主窗口","窗口2通知窗口1"));
    from_1->hide();
    from_2->hide();

    // 设置 主窗口-->窗口1 的信号连接
    connect(this,SIGNAL(showchild()),from_1.data(),SLOT(showMyself()));
    connect(this,SIGNAL(closechild()),from_1.data(),SLOT(closeMyself()));
    connect(this,SIGNAL(sendText(const QString str)),from_1.data(),SLOT(Form::setlabelText(const QString str)));

}

MainWindow::~MainWindow()
{
    qDebug()<<"MainWindow exit";
    delete ui;
}

/* 窗口1 中按钮槽函数 通过信号和槽操作 */
void MainWindow::on_btn_open_1_clicked()
{
    emit showchild();
}

void MainWindow::on_btn_close_1_clicked()
{
    emit closechild();
}

void MainWindow::on_btn_send_1_clicked()
{
    emit sendText("主窗口通知窗口1");
}

/* 窗口2 中按钮槽函数 直接操作 */
void MainWindow::on_btn_open_2_clicked()
{
    from_2->show();
}

void MainWindow::on_btn_close_2_clicked()
{
    from_2->QWidget::close();
}

void MainWindow::on_btn_send_2_clicked()
{
    from_2->setlabelText("主窗口通知窗口2");
}

void MainWindow::set_mian_label(const QString str){
//    ui->groupBox();

}

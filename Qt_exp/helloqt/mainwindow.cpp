#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),                            // 基类的构造函数，这里 parent 好像每个子类都有
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit_account->text().isEmpty())
    {
        QMessageBox::information(this,"提示","账号不能为空",QMessageBox::Yes);
        return ;
    }
    if(ui->lineEdit_passwd->text().isEmpty())
    {
        QMessageBox::information(this,"提示","密码不能为空",QMessageBox::Yes);
        return;
    }
    QMessageBox::information(this,"正确","登录成功",QMessageBox::Ok);
}

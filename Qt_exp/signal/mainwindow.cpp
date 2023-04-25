#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "form.h"
#include "ui_form.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    from_1.reset(new Form("窗口1","窗口1通知主窗口","窗口1通知窗口2"));
    from_2.reset(new Form("窗口2","窗口2通知主窗口","窗口2通知窗口1"));

    // 设置按钮和子窗口状态同步
    from_1->hide();
    from_2->hide();
    ui->btn_send_1->setEnabled(false);
    ui->btn_send_2->setEnabled(false);
    // 设置 主窗口-->窗口1 的信号连接
    connect(this,SIGNAL(showchild()),from_1.data(),SLOT(showMyself()));
    connect(this,SIGNAL(closechild()),from_1.data(),SLOT(closeMyself()));
    connect(this,SIGNAL(sendText(const QString)),
            from_1.data(),SLOT(setlabelText(const QString)));

    // 设置 窗口1 --> 主窗口和 窗口2 的信号连接
    connect(from_1.data(),SIGNAL(sendMainText(const QString)),
            this,SLOT(set_mian_label(const QString)));
    connect(from_1.data(),SIGNAL(sendOtherText(const QString)),
            from_2.data(),SLOT(setlabelText(const QString)));
    connect(from_1.data(),SIGNAL(close()),
            this,SLOT(form_close_handle()));

    // 设置 窗口2 --> 主窗口和 窗口1 的信号连接
    connect(from_2.data(),SIGNAL(sendMainText(const QString)),
            this,SLOT(set_mian_label(const QString)));
    connect(from_2.data(),SIGNAL(sendOtherText(const QString)),
            from_1.data(),SLOT(setlabelText(const QString)));
    connect(from_2.data(),SIGNAL(close()),
            this,SLOT(form_close_handle()));
}

MainWindow::~MainWindow()
{
    qDebug()<<"MainWindow exit";
    delete ui;
}

/* 窗口1 中按钮槽函数 通过信号和槽操作 */
void MainWindow::on_btn_open_1_clicked()
{
    ui->btn_send_1->setEnabled(true);
    emit showchild();
}

void MainWindow::on_btn_close_1_clicked()
{
    ui->btn_send_1->setEnabled(false);
    emit closechild();
}

void MainWindow::on_btn_send_1_clicked()
{
    emit sendText("主窗口通知窗口1");
}

/* 窗口2 中按钮槽函数 直接操作 */
void MainWindow::on_btn_open_2_clicked()
{
    ui->btn_send_2->setEnabled(true);
    from_2->show();
}

void MainWindow::on_btn_close_2_clicked()
{
    ui->btn_send_2->setEnabled(false);
    from_2->QWidget::close();
}

void MainWindow::on_btn_send_2_clicked()
{
    from_2->setlabelText("主窗口通知窗口2");
}

void MainWindow::set_mian_label(const QString str){
    // 判断这个信号是从 哪个窗口来的
    Form* p = qobject_cast<Form*>(sender());
    if(p == from_1.data()){             // 来自窗口1
        ui->label->setText(str);
    }
    else if(p ==from_2.data()){         // 来自窗口2
        ui->label_2->setText(str);
    }
}

void MainWindow::form_close_handle(){
    // 判断这个信号是从 哪个窗口来的
    Form* p = qobject_cast<Form*>(sender());
    if(p == from_1.data()){             // 来自窗口1
        ui->btn_send_1->setEnabled(false);
    }
    else if(p ==from_2.data()){         // 来自窗口2
        ui->btn_send_2->setEnabled(false);
    }
}

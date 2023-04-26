#include "form.h"
#include "ui_form.h"
#include <QDebug>

Form::Form(const QString name,
           const QString send_main_str,
           const QString send_other_str,
           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    sendstr_to_main(send_main_str),
    sendstr_to_other(send_other_str)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);      // 主窗口 关闭后 子窗口也关闭
    ui->groupBox->setTitle(name);
}

Form::~Form()
{
    qDebug()<<"Form exit";
    delete ui;
}

/* 通过信号和槽机制 操作自身 */
void Form::showMyself()
{
    this->QWidget::show();
}

void Form::closeMyself()
{
    this->QWidget::close();
}

void Form::setlabelText(const QString str)
{
    ui->label->setText(str);
}

/* 按钮对应的槽函数 */
void Form::on_btn_hide_clicked()
{
    this->hide();
}

void Form::on_btn_close_clicked()
{
    this->QWidget::close();
}

void Form::on_btn_send_to_main_clicked()
{
    emit sendMainText(sendstr_to_main);
}

void Form::on_btn_send_to_other_clicked()
{
    emit sendOtherText(sendstr_to_other);
}

/* 窗体关闭事件 到 主窗口的信号 */
void Form::closeEvent(QCloseEvent*event)
{
    (void)event;
    emit form_close();
}


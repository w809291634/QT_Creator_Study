#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"

extern QEvent::Type _postEvent;
QEvent::Type _postEvent=static_cast<QEvent::Type>(QEvent::registerEventType());


MainWindow* _mian_win=nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _mian_win=this;
    ui->setupUi(this);
    thread_1=new MyThread();
    thread_1->start();
    connect(thread_1,SIGNAL(finished()),this,SLOT(thread_1_fin()));
    connect(thread_1,SIGNAL(finished()),thread_1,SLOT(deleteLater()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::thread_1_fin()
{
    thread_1=nullptr;
}

void MainWindow::on_send_custom_event_clicked()
{
    QCoreApplication::postEvent(thread_1, new QEvent(_postEvent));
}

void MainWindow::on_send_quit_clicked()
{
    if(thread_1!=nullptr)
        thread_1->quit();
}

void MainWindow::on_send_exit_clicked()
{
    if(thread_1!=nullptr)
        thread_1->exit();
}


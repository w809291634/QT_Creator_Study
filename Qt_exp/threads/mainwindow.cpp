#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* _main_win=nullptr;
static MyThread* QSemaphore_thread1;
static MyThread* QSemaphore_thread2;
static QSemaphore* sem;

static void QSemaphore_thread1_cb(void)__attribute__((noreturn));
static void QSemaphore_thread2_cb(void)__attribute__((noreturn));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->QSemaphore_bar1->setValue(0);
    ui->QSemaphore_bar2->setValue(0);
}

MainWindow::~MainWindow()
{
    QSemaphore_thread1->exit(0);
    QSemaphore_thread2->exit(0);
    delete ui;
}

void app_main(MainWindow* instance)
{
    _main_win=instance;
    sem=new QSemaphore();

    QSemaphore_thread1=new MyThread(_main_win);
    QSemaphore_thread2=new MyThread(_main_win);
    QSemaphore_thread1->set_callback(QSemaphore_thread1_cb);
    QSemaphore_thread2->set_callback(QSemaphore_thread2_cb);
    QObject::connect(QSemaphore_thread1,SIGNAL(MsgSignal(const QString&)),_main_win,SLOT(update_QSemaphore_bar(const QString&)));
    QObject::connect(QSemaphore_thread2,SIGNAL(MsgSignal(const QString&)),_main_win,SLOT(update_QSemaphore_bar(const QString&)));
    QSemaphore_thread1->start();
    QSemaphore_thread2->start();
}

void MainWindow::on_QSemaphore_btn_clicked(bool checked)
{
    (void)(checked);
    sem->release();
}

void MainWindow::update_QSemaphore_bar(const QString& msg)
{
    MyThread* p = qobject_cast<MyThread*>(sender());
    if(p == QSemaphore_thread1){
        int value=ui->QSemaphore_bar1->value()+1;
        ui->QSemaphore_bar1->setValue(value);
    }
    else if(p ==QSemaphore_thread2){
        int value=ui->QSemaphore_bar2->value()+1;
        ui->QSemaphore_bar2->setValue(value);
    }
    qDebug()<<msg;
}

static void QSemaphore_thread1_cb(void)
{
    while(1)
    {
        if(sem->tryAcquire(1,INT_MAX)){
            emit QSemaphore_thread1->MsgSignal("QSemaphore_thread1 emit");
            qDebug()<<"QSemaphore_thread1";
            QSemaphore_thread1->sleep(1);
        }
    }
}

static void QSemaphore_thread2_cb(void)
{
    while(1)
    {
        if(sem->tryAcquire(1,INT_MAX)){
            emit QSemaphore_thread2->MsgSignal("QSemaphore_thread2 emit");
            QSemaphore_thread2->sleep(1);
        }
    }
}







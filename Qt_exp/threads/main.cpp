#include "mainwindow.h"
#include <QApplication>
#include "mythread.h"

MyThread* QSemaphore_thread1;
void QSemaphore_thread1_cb(void);
MyThread* QSemaphore_thread2;
void QSemaphore_thread2_cb(void);

void QSemaphore_thread1_cb(void)
{
    qDebug()<<"here1";
    QSemaphore_thread1->sleep(1000);
}

void QSemaphore_thread2_cb(void)
{
    qDebug()<<"here1";
    QSemaphore_thread2->sleep(1000);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSemaphore_thread1=new MyThread(&w);
    QSemaphore_thread2=new MyThread(&w);

    w.show();

    return a.exec();
}

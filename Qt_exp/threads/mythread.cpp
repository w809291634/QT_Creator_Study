#include "mythread.h"

MyThread::MyThread(QObject * parent):
    QThread(parent)
{

}

MyThread::~MyThread()
{
    qDebug()<<QString("%1 exit").arg(this->objectName());
}

void MyThread::run(){
    if(callback!=nullptr){
        callback();
    }
}

void MyThread::set_callback(void (*cb)(void))
{
    if(cb!=nullptr)callback=cb;
}

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
        callback(para);
    }
    // 执行 事件循环 接收quit会退出
    QThread::exec();
    // 延迟删除
    this->deleteLater();
}


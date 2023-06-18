#include "o_thread.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

O_Thread::O_Thread(QString name):
    QObject()
{
    // 不能够指定父类，否则不能moveToThread
    this->setObjectName(name);
    thread = new QThread();
    thread->setObjectName(name);
    thread->start();
    // 自己的事件在thread中处理
    moveToThread(thread);
    // 绑定启动信号
    connect(this, SIGNAL(_start()), this, SLOT(loop()));

    // 设置空闲状态
    status = IDLE;
}

O_Thread::O_Thread(QString name,Priority priority):
    QObject()
{
    // 不能够指定父类，否则不能moveToThread
    this->setObjectName(name);
    thread = new QThread();
    thread->setObjectName(name);
    thread->start(static_cast<QThread::Priority>(priority));
    // 自己的事件在thread中处理
    moveToThread(thread);
    // 绑定启动信号
    connect(this, SIGNAL(_start()), this, SLOT(loop()));

    // 设置空闲状态
    status = IDLE;
}

O_Thread::~O_Thread()
{
    deleteLater();
}

// 注册回调函数
void O_Thread::RegeditCallBack(_CALLBACK fun,char* para)
{
    callback = fun;
    if(para!=nullptr){
        // 为了管理自己的参数 暂时不知道 void* 指针怎么管理
        // 所以现在只支持 char* 参数
        size_t len=strlen(para)+1;
        parameter = new char[len];
        memset(parameter,0,len);
        memcpy(parameter,para,len);
    }
}

// 设置参数
void O_Thread::SetParameter(char* para)
{
    if(parameter!=nullptr){
        delete[] parameter;
        parameter=nullptr;
    }
    if(para!=nullptr){
        // 为了管理自己的参数 暂时不知道 void* 指针怎么管理
        // 所以现在只支持 char* 参数
        size_t len=strlen(para)+1;
        parameter = new char[len];
        memset(parameter,0,len);
        memcpy(parameter,para,len);
    }
}

// 线程的用户循环loop
void O_Thread::loop()
{
    if(callback!=nullptr){
        {
            QMutexLocker locker(&statusMutex);
            status = RUN;
        }
        while(status == RUN)
        {
            // 回调函数
            // 这个回调函数不要长时间阻塞
            // 回调函数内 扫描频率越快，事件循环响应越快
            callback(parameter);
        }
    }
    {
        QMutexLocker locker(&statusMutex);
        status = IDLE;
    }
}

void O_Thread::start()
{
    if(thread!=nullptr ){
        if(status!=RUN){
            emit _start();
        }else qDebug() << "info: thread is running";
    }
    else qDebug() << "Thread has been deleted";
}

// 停止线程用户循环loop，进入事件循环
void O_Thread::stop()
{
    if(thread!=nullptr){
        QMutexLocker locker(&statusMutex);
        status = STOP;
    }
    else qDebug() << "Thread has been deleted";
}

// 销毁所在线程，对象事件即将不处理
// 如果没有销毁成功，则阻塞
// 完成后可以删除O_Thread实例
// 外部可以直接删除O_Thread实例
void O_Thread::deleteLater()
{
    if(thread!=nullptr){
        uintptr_t address = reinterpret_cast<uintptr_t>(this);
        qDebug() << QString("%1(0x%2) exit")
                    .arg(thread->objectName())
                    .arg(address,0,16);
        stop();
        thread->quit();
        thread->wait();
        thread->deleteLater();
        thread=nullptr;
        if(parameter!=nullptr){
            delete[] parameter;
            parameter=nullptr;
        }
    }
    else qDebug() << "Thread has been deleted";
}

// 返回状态
O_Thread::Status_t O_Thread::get_status() const
{
    return status;
}

// 设置状态
void O_Thread::set_status(O_Thread::Status_t new_status)
{
    QMutexLocker locker(&statusMutex);
    status=new_status;
}

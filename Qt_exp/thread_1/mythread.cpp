#include "mythread.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MyThread::MyThread(QObject * parent):
    QObject(parent)
{
    // execute signals in a new thread
    thread = new QThread();
    thread->start();
    qDebug("HttpConnectionHandler (%p): thread started", static_cast<void*>(this));

    moveToThread(thread);
    readTimer.moveToThread(thread);
    readTimer.setSingleShot(true);

    connect(&readTimer, SIGNAL(timeout()), SLOT(readTimeout()));
    connect(thread, SIGNAL(finished()), this, SLOT(thread_done()));
}

MyThread::~MyThread()
{

}



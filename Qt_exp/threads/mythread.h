#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QtCore>
#include <iostream>
#include <functional>

typedef std::function<void(void*)> CALLBACK;

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject * parent=nullptr);
    ~MyThread();

    void regeditCallBack(CALLBACK fun)/*注册回调函数*/
    {
        callback = fun;
    }

signals:
    void MsgSignal(const QString& msg);
    void MsgSignal(const QString& act,
                   const QString& cnt1,
                   const QString& cnt2);
protected:
    void* para;
    bool _register;
    CALLBACK callback=nullptr;
    virtual void run();
};

#endif // MYTHREAD_H

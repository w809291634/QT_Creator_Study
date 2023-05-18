#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QtCore>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject * parent=nullptr);
    ~MyThread();

    void set_callback(void (*cb)(void));

signals:
    void MsgSignal(const QString& msg);
    void MsgSignal(const QString& act,
                   const QString& cnt1,
                   const QString& cnt2);
protected:
    void (*callback)(void)=nullptr;
    virtual void run();
};

#endif // MYTHREAD_H

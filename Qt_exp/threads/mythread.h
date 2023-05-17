#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject * parent=nullptr);
    ~MyThread();

    void set_callback(void (*cb)(void));

signals:
    void MsgSignal(const QString& msg);

protected:
    void (*callback)(void)=nullptr;
    virtual void run();
};

#endif // MYTHREAD_H

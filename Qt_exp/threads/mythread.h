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
    void (*callback)(void)=nullptr;
signals:
    void MsgSignal(const QString& msg);

protected:

    virtual void run();
};

#endif // MYTHREAD_H

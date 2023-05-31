#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QtCore>

class MainWindow;
class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject * parent=nullptr);
    ~MyThread();

    void set_callback(void (*cb)(void));
    bool event(QEvent *event);

signals:
    void MsgSignal(const QString& msg);
    void MsgSignal(const QString& act,
                   const QString& cnt1,
                   const QString& cnt2);

private:
    /** Configuration settings */
    const QSettings* settings;

    /** The thread that processes events of this connection */
    QThread* thread;

    /** Time for read timeout detection */
    QTimer readTimer;

private slots:

    /** Received from the socket when a read-timeout occured */
    void readTimeout();

    /** Cleanup after the thread is closed */
    void thread_done();
};


#endif // MYTHREAD_H

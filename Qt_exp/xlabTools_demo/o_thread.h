#ifndef O_THREAD_H
#define O_THREAD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QThread>
#include <QtCore>

#include <iostream>
#include <functional>
using namespace std;
typedef std::function<void(char*)> _CALLBACK;

class O_Thread : public QObject
{
    Q_OBJECT
public:
    explicit O_Thread(QString name);
    ~O_Thread();

    enum Priority {
        IdlePriority,

        LowestPriority,
        LowPriority,
        NormalPriority,
        HighPriority,
        HighestPriority,

        TimeCriticalPriority,

        InheritPriority
    };
    enum Status {
        IDLE,
        RUN,
        STOP,       // 进入事件循环
    };
    typedef enum Status Status_t;

    explicit O_Thread(QString name,Priority priority);

    // 线程主体
    QThread* thread = static_cast<QThread*>(nullptr);
    // 回调函数 和 回调函数参数
    _CALLBACK callback = static_cast<_CALLBACK>(nullptr);
    char* parameter = static_cast<char*>(nullptr);

    // 注册回调函数和参数
    void RegeditCallBack(_CALLBACK fun,char* para=nullptr);
    void SetParameter(char* para);

    void start();       // 启动loop函数
    void stop();        // 退出loop函数
    void deleteLater(); // 退出事件循环，并销毁QThread实例

    Status_t get_status()const; // 获取状态
    void set_status(Status_t new_status);   // 设置状态

signals:
    // 启动信号
    void _start();
    // 线程状态信息
    void Status(const QString& info);
    // 自定义信号
    void message(const QString& info);

private slots:
    void loop();

private:
    // 状态标志
    Status_t status;
    QMutex statusMutex;
};

#endif // O_THREAD_H

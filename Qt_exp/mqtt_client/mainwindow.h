#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtCore>
#include <QMainWindow>
#include "QtMqtt/qmqttclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setClientPort(int p);

private slots:
    // 槽函数
    void on_buttonConnect_clicked();
    void on_buttonQuit_clicked();
    void on_buttonPublish_clicked();
    void on_buttonSubscribe_clicked();
    void on_buttonPing_clicked();

    // 日志更新
    void updateLogStateChange();
    // 接收订阅消息
    void client_messageReceived(
            const QByteArray &message,
            const QMqttTopicName &topic);
    // ping命令反馈
    void client_ping_response();

    void brokerDisconnected();



private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
};

#endif // MAINWINDOW_H

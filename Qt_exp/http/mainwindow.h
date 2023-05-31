#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtCore>
#include <QMainWindow>
#include <QSemaphore>
//info
#include <QNetworkInterface>
#include <QHostInfo>
#include <QHostAddress>
#include <QTcpServer>

#include "httpserver.h"
#include "httpclient.h"

#define FIRST_IP_ARRDESS                {"192.168.100",\
                                        "192.168.31"}       // 服务器端 想监听的 优先地址

typedef QMap<QString,QString> StringMap;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /** HTTP 服务器 **/
    // 控件 槽函数
    void on_server_history_clear_clicked();
    void on_server_listen_btn_clicked();
    void on_server_close_btn_clicked();

    // 状态 变化
    void server_listening();
    void server_close();
    void server_info_update(const StringMap& info);

    /** HTTP 客户端 **/
    // 控件 槽函数
    void on_client_send_request_clicked();

    // 客户端请求
    void client_request_finish();

    void on_client_history_clear_clicked();

private:
    void TCP_getLocalIP();
    Ui::MainWindow *ui;

    /** HTTP 服务器 **/
    void init_http_server();
    void init_http_server_ui();

    // http 服务器 和 请求处理对象
    QSettings* m_listenerSettings;
    HttpServerHandler* m_http_server;
    HttpListener* m_HttpListener=static_cast<HttpListener*>(nullptr);

    /** HTTP 客户端 **/
    void init_http_client();
    void init_http_client_ui();

    // http 客户端对象
    HttpClient* m_http_client;

    // 变量
    QNetworkReply* m_post_reply=
            static_cast<QNetworkReply*>(nullptr);
    QNetworkReply* m_get_reply=
            static_cast<QNetworkReply*>(nullptr);
    QSemaphore* m_sem;
};

#endif // MAINWINDOW_H

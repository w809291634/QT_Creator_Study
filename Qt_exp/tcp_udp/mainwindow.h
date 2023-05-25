#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
//info
#include <QNetworkInterface>
#include <QHostInfo>
#include <QHostAddress>
//TCP
#include <QTcpServer>
#include <QTcpSocket>
//UDP
#include <QUdpSocket>
#include <QMessageBox>

#define FIRST_IP_ARRDESS                {"192.168.100",\
                                        "192.168.31"}       // 服务器端 想监听的 优先地址
#define TCP_SERVER_MAX_CONNECTIONS      30
#define TCP_CONNECTING_TIMEOUT          5                  // 客户端 TCP 连接超时时间，单位s

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
    void menu_clicked();

    /* TCP服务器 */
    void on_tcp_s_listen_btn_clicked();
    void on_tcp_s_close_btn_clicked();
    void on_tcp_s_send_btn_clicked();
    void on_tcp_s_history_clear_clicked();
    void on_tcp_s_unconnect_btn_clicked();

    // 连接和数据读写
    void tcp_s_newConnection_trigger();
    void tcp_s_read();
    void tcp_s_write();

    // 获取信息
    void tcp_s_update_ClientInfo();
    void tcp_s_acceptError(QAbstractSocket::SocketError socketError);
    void tcp_s_SocketState_Changed(QAbstractSocket::SocketState SocketState);
    void tcp_s_listening();
    void tcp_s_listen_close();
    void tcp_s_clear();

    /* TCP客户端 */
    void on_tcp_c_connect_btn_clicked();
    void on_tcp_c_disconnect_btn_clicked();
    void on_tcp_c_send_btn_clicked();
    void on_tcp_c_history_clear_clicked();

    // 数据读写
    void tcp_c_read();
    void tcp_c_write();

    // 获取信息
    void tcp_c_SocketState_Changed(QAbstractSocket::SocketState SocketState);
    void tcp_c_Connecting();
    void tcp_c_Connecting_timeout();
    void tcp_c_Connected();
    void tcp_c_Unconnected();
    void tcp_c_clear();

private:
    Ui::MainWindow *ui;
    /* TCP服务器 */
    QSharedPointer<QList<QTcpSocket *>> m_Tcp_ClientList;
    QTcpServer * m_TcpServer;
    QTimer* m_stimer;

    /* TCP客户端 */
    QTcpSocket * m_TcpClient=static_cast<QTcpSocket*>(nullptr);
    QTimer* m_ctimer=static_cast<QTimer*>(nullptr);
    QTimer* m_timeout_timer=static_cast<QTimer*>(nullptr);
};

#endif // MAINWINDOW_H

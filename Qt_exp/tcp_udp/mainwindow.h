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

#define TCP_SERVER_MAX_CONNECTIONS   30

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
    /* TCP服务器 */
    void on_tcp_s_listen_btn_clicked();
    void on_tcp_s_close_btn_clicked();
    void on_tcp_s_send_btn_clicked();

    // 连接和数据读写
    void tcp_s_newConnection_trigger();
    void tcp_s_read();

    // 获取信息
    void tcp_s_update_ClientInfo();
    void tcp_s_acceptError(QAbstractSocket::SocketError socketError);
    void tcp_s_SocketState_Changed(QAbstractSocket::SocketState SocketState);

    /* TCP客户端 */
    void on_tcp_c_connect_btn_clicked();
    void on_tcp_c_disconnect_btn_clicked();
    void on_tcp_c_send_btn_clicked();

    // 获取信息
    void tcp_c_SocketState_Changed(QAbstractSocket::SocketState SocketState);

private:
    Ui::MainWindow *ui;
    /* TCP服务器 */
    QSharedPointer<QList<QTcpSocket *>> m_Tcp_ClientList;
    QTcpServer * m_TcpServer;

    /* TCP客户端 */
    QTcpSocket * m_TcpClient;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QMessageBox>
//info
#include <QNetworkInterface>
#include <QHostInfo>
#include <QHostAddress>
//TCP
#include <QTcpServer>
#include <QTcpSocket>
//UDP
#include <QUdpSocket>
#include <QNetworkDatagram>

#define FIRST_IP_ARRDESS                {"192.168.100",\
                                        "192.168.31"}       // 服务器端 想监听的 优先地址
#define TCP_SERVER_MAX_CONNECTIONS      30
#define TCP_CONNECTING_TIMEOUT          5                  // 客户端 TCP 连接超时时间，单位s

namespace Ui {
class MainWindow;
}

class destination_add
{
public:
    destination_add(){}
    ~destination_add(){}

    QString peerAddress_port;
    QHostAddress peerAddress;
    quint16 peerPort;

    friend bool operator==(const destination_add & A, const destination_add & B){
        if(A.peerPort==B.peerPort
                && A.peerAddress==B.peerAddress
                && A.peerAddress_port==B.peerAddress_port){
            return 1;
        }else return 0;
    }

    bool is_vaild() const{
        if(peerPort==0 && peerAddress_port.isEmpty() && peerAddress.toString().isNull())
            return 0;
        else return 1;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void menu_clicked();
    void init_TCP();
    void init_UDP();

    /********* TCP服务器 *********/
    void on_tcp_s_listen_btn_clicked();
    void on_tcp_s_close_btn_clicked();
    void on_tcp_s_unconnect_btn_clicked();
    void on_tcp_s_send_btn_clicked();
    void on_tcp_s_history_clear_clicked();

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
    QString TCP_getLocalIP();

    /********* TCP客户端 *********/
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

    /********* UDP *********/
    void on_udp_bind_btn_clicked();
    void on_udp_unbind_btn_clicked();
    void on_udp_join_multi_btn_clicked();
    void on_udp_leave_multi_btn_clicked();
    void on_udp_history_clear_clicked();
    void on_udp_unicast_btn_clicked();
    void on_udp_broadcast_btn_clicked();
    void on_udp_multicast_btn_clicked();

    // 数据读写
    void Udp_SocketReadyRead();

    // 获取信息
    void Udp_SocketStateChanged(QAbstractSocket::SocketState SocketState);
    QString UDP_getLocalIP();


    // 状态
    void Udp_Init_Ui();
    void Udp_SocketBind();
    void Udp_SocketUnBind();
    void Udp_SocketJoinMulti();
    void Udp_SocketLeaveMulti();
    void Udp_Destination_Update();






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

    /* UDP */
    QUdpSocket * m_pUdpSocket;
    QSharedPointer<QList<destination_add>> m_destination_add_p=static_cast<QSharedPointer<QList<destination_add>>>(nullptr);      // 保存
    QTimer* dest_update_timer;
};

#endif // MAINWINDOW_H

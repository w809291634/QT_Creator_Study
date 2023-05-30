#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_any_ipv4=QHostAddress::AnyIPv4;

    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("TCP示例");

    // 主菜单 进行 页面切换
    QList<QAction*> actionlist_1 = this->findChildren<QAction*>();
    foreach(auto action, actionlist_1)
    {
        for(auto name:{"TCP","UDP"}){
            if(action->objectName()==name){
                connect(action,SIGNAL(triggered()),this,SLOT(menu_clicked()));
            }
        }
    }

    init_TCP();                                         // 初始化 TCP 的示例
    init_UDP();                                         // 初始化 UDP 的示例
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 菜单栏 action函数
void MainWindow::menu_clicked()
{
    QAction* action= qobject_cast<QAction*>(sender());
    if(action->objectName()=="TCP"){
        ui->stackedWidget->setCurrentIndex(0);
        this->setWindowTitle("TCP示例");
    }
    else if(action->objectName()=="UDP"){
        ui->stackedWidget->setCurrentIndex(1);
        this->setWindowTitle("UDP示例");
    }
}

// 初始化 TCP 的示例
void MainWindow::init_TCP()
{
    QString Priority_ip;
    /******* TCP服务器 *******/
    m_Tcp_ClientList.reset(new QList<QTcpSocket *>);
    m_TcpServer = new QTcpServer(this);
    m_TcpServer->setMaxPendingConnections(TCP_SERVER_MAX_CONNECTIONS);

    // 服务端 UI 初始化
    tcp_s_Init_Ui();
    tcp_s_clear();
    tcp_s_listen_close();

    // 筛选出能够监听的 ip地址
    Priority_ip=TCP_getLocalIP();

    // TCP服务器端的 相关信号 的连接
    connect(m_TcpServer,SIGNAL(newConnection()),
            this, SLOT(tcp_s_newConnection_trigger()));
    connect(m_TcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(tcp_s_acceptError(QAbstractSocket::SocketError)));

    // 更新 已连接 客户端的信息
    QTimer* temp_Timer = new QTimer(this);
    connect(temp_Timer, SIGNAL(timeout()), this, SLOT(tcp_s_update_ClientInfo()));
    temp_Timer->start(100);

    // 清理 状态的定时器
    m_stimer = new QTimer(this);
    m_stimer->setSingleShot(true);
    connect(m_stimer, SIGNAL(timeout()), this, SLOT(tcp_s_clear()));

    /******* TCP客户端 *******/
    m_TcpClient = new QTcpSocket(this);

    // 客户端 UI 初始化
    tcp_c_Init_Ui(Priority_ip);
    tcp_c_clear();
    tcp_c_Unconnected();

    // 清理 状态的定时器
    m_ctimer = new QTimer(this);
    m_ctimer->setSingleShot(true);
    connect(m_ctimer, SIGNAL(timeout()), this, SLOT(tcp_c_clear()));

    // 客户端 连接超时
    m_timeout_timer = new QTimer(this);
    m_timeout_timer->setSingleShot(true);
    connect(m_timeout_timer, SIGNAL(timeout()), this, SLOT(tcp_c_Connecting_timeout()));

    // 客户端 socket的 重要信号连接
    connect(m_TcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(tcp_c_SocketState_Changed(QAbstractSocket::SocketState)));
    connect(m_TcpClient,SIGNAL(readyRead()),this,SLOT(tcp_c_read()));
}

// 初始化 UDP 的示例
void MainWindow::init_UDP()
{
    m_pUdpSocket = new QUdpSocket(this);

    // 设置ttl
    m_pUdpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,16);

    // 用于更新本地可发送对象
    m_Peer_Address_p.reset(new QList<Peer_Address>);
    dest_update_timer= new QTimer(this);
    connect(dest_update_timer, SIGNAL(timeout()), this, SLOT(Udp_Destination_Update()));
    dest_update_timer->start(100);

    // 显示初始 UI
    Udp_Init_Ui();
    Udp_SocketUnBind();
    Udp_SocketLeaveMulti();
    UDP_getLocalIP();

    connect(m_pUdpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(Udp_SocketStateChanged(QAbstractSocket::SocketState)));
    connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(Udp_SocketRead()));
}

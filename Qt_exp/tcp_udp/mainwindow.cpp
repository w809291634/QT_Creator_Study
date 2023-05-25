#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString first_ip;
    ui->setupUi(this);

    QList<QAction*> actionlist_1 = this->findChildren<QAction*>();
    foreach(auto action, actionlist_1)
    {
        for(auto name:{"TCP","UDP"}){
            if(action->objectName()==name){
                connect(action,SIGNAL(triggered()),this,SLOT(menu_clicked()));
            }
        }
    }

    /******* TCP服务器 *******/
    m_Tcp_ClientList.reset(new QList<QTcpSocket *>);
    m_TcpServer = new QTcpServer(this);
    m_TcpServer->setMaxPendingConnections(TCP_SERVER_MAX_CONNECTIONS);

    // 服务端 UI 初始化
    tcp_s_clear();
    tcp_s_listen_close();

    // 筛选出能够监听的 ip地址
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, list)
    {
        QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entryList)
        {
            //过滤 IPv6 地址，只留下 IPv4
            if (entry.ip().protocol() ==QAbstractSocket::IPv4Protocol)
            {
                QHostAddress ip = entry.ip();
                //  筛选出能够监听的 ip地址
                if(m_TcpServer->listen(ip)){
                    char _flag=0;
                    m_TcpServer->close();
                    // 将优先地址 排在 前面
                    for(auto _ip:FIRST_IP_ARRDESS){
                        if(ip.toString().contains(_ip)){
                            ui->tcp_s_ip_cbox->insertItem(0,ip.toString());
                            first_ip=ip.toString();
                            _flag=1;
                            break;
                        }
                    }
                    // 不在优先选择地址，放在队列尾部
                    if(!_flag)
                        ui->tcp_s_ip_cbox->addItem(ip.toString());
                }
            }
        }
    }
    ui->tcp_s_ip_cbox->setCurrentIndex(0);

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
    ui->tcp_cs_ip_ledit->setText(first_ip);              // 用于大概率的搜索 计算机的服务器IP地址
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
    }
    else if(action->objectName()=="UDP"){
        ui->stackedWidget->setCurrentIndex(1);
    }
}

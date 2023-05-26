﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 待 补充
//    this->setWindowIcon(QIcon(":/new/prefix1/res/TitleIcon.png"));
//    this->setWindowTitle(QStringLiteral("UDP Send/Receiver"));

    // 还可以  加入 输入限制
//    m_pSpinPort = new QSpinBox(m_pCentralWidget);                   端口使用spinbox    设置范围
//    m_pSpinPort->setMinimum(1);
//    m_pSpinPort->setMaximum(65535);
//    m_pSpinPort->setValue(3200);


    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
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

    /******* 需要更改 *******/
/*
 * 1、端口设置为sbox
*/

    QString Priority_ip;
    /******* TCP服务器 *******/

    m_Tcp_ClientList.reset(new QList<QTcpSocket *>);
    m_TcpServer = new QTcpServer(this);
    m_TcpServer->setMaxPendingConnections(TCP_SERVER_MAX_CONNECTIONS);

    // 服务端 UI 初始化
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
    ui->tcp_cs_ip_ledit->setText(Priority_ip);              // 用于大概率的搜索 计算机的服务器IP地址
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
    m_destination_add_p.reset(new QList<destination_add>);

    // 显示初始 UI
    Udp_Init_Ui();
    Udp_SocketUnBind();
    Udp_SocketLeaveMulti();
    UDP_getLocalIP();

    connect(m_pUdpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(Udp_SocketStateChanged(QAbstractSocket::SocketState)));
    connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(Udp_SocketReadyRead()));
}

/******* 后面全部移动到 UDP。c *******/

// 获取 本地 能够 监听的地址
QString MainWindow::UDP_getLocalIP()
{
    QString Priority_ip;
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
                if(m_pUdpSocket->bind(ip)){
                    char _flag=0;
                    m_pUdpSocket->abort();
                    // 将优先地址 排在 前面
                    for(auto _ip:FIRST_IP_ARRDESS){
                        if(ip.toString().contains(_ip)){
                            ui->udp_bind_ip_cbox->insertItem(0,ip.toString());
                            ui->udp_destination_ip_cbox->insertItem(0,ip.toString());
                            Priority_ip=ip.toString();
                            _flag=1;
                            break;
                        }
                    }
                    // 不在优先选择地址，放在队列尾部
                    if(!_flag){
                        ui->udp_bind_ip_cbox->addItem(ip.toString());
                        ui->udp_destination_ip_cbox->addItem(ip.toString());
                    }
                }
            }
        }
    }
    ui->udp_destination_ip_cbox->setCurrentIndex(0);
    ui->udp_bind_ip_cbox->setCurrentIndex(0);
    return Priority_ip;
}

/******* UI 状态 *******/
void MainWindow::Udp_Init_Ui()
{
    // 绑定 IP 和 端口的 ui 设置
    ui->udp_bind_port_sbox->setRange(1,65535);
    ui->udp_bind_port_sbox->setValue(9090);

    // 组播 IP 和 端口的 ui 设置
    ui->udp_group_ip_cbox->setEditable(true);  //下拉框可编辑输入
    ui->udp_group_ip_cbox->addItem("239.0.0.1");
    // 正则匹配 D类IP：224.0.0.0~239.255.255.255
    // .必须使用转义字符\，否则.会匹配任意字符
    // C++中"\"在字符串中表示要用"\\"
    QRegExp regExp("^(22[4-9]|23[0-9])(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}$");
    QValidator* pValidator = new QRegExpValidator(regExp, this);
    ui->udp_group_ip_cbox->setValidator(pValidator);

    ui->udp_group_port_sbox->setRange(1,65535);
    ui->udp_group_port_sbox->setValue(9090);

    // 发送目的地址 IP 和 端口的 ui 设置
    ui->udp_destination_port_cbox->setEditable(true);
    ui->udp_destination_port_cbox->setValidator(new QIntValidator(0, 65535,this));

}

// UDP 绑定状态
void MainWindow::Udp_SocketBind()
{
    // 按钮
    ui->udp_bind_btn->setEnabled(false);
    ui->udp_unbind_btn->setEnabled(true);
    // 设置 绑定IP
    ui->udp_bind_ip_cbox->setEnabled(false);
    ui->udp_bind_port_sbox->setEnabled(false);
    // 状态
    ui->statusBar->showMessage("状态：绑定成功");
    ui->udp_listWidget->addItem("UDP套接字绑定成功");
    ui->udp_listWidget->scrollToBottom();
}

// UDP 解除绑定状态
void MainWindow::Udp_SocketUnBind()
{
    // 按钮
    ui->udp_bind_btn->setEnabled(true);
    ui->udp_unbind_btn->setEnabled(false);
    // 设置 绑定IP
    ui->udp_bind_ip_cbox->setEnabled(true);
    ui->udp_bind_port_sbox->setEnabled(true);
    // 状态
    ui->statusBar->showMessage("状态：解除绑定");
    ui->udp_listWidget->addItem("UDP套接字解除绑定");
    ui->udp_listWidget->scrollToBottom();
}


void MainWindow::Udp_SocketJoinMulti()
{
    ui->udp_join_multi_btn->setEnabled(false);
    ui->udp_leave_multi_btn->setEnabled(true);
}

void MainWindow::Udp_SocketLeaveMulti()
{
    ui->udp_join_multi_btn->setEnabled(true);
    ui->udp_leave_multi_btn->setEnabled(false);

}

void MainWindow::Udp_Destination_Update()
{


}

/******* 按钮槽函数 *******/
// 绑定按钮
void MainWindow::on_udp_bind_btn_clicked()
{
    QHostAddress bind_ip(ui->udp_bind_ip_cbox->currentText());
    quint16 bind_port=static_cast<quint16>(ui->udp_bind_port_sbox->value());
    if (!m_pUdpSocket->bind(bind_ip,bind_port)) {
        QMessageBox::warning(this,"绑定失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 解除绑定按钮
void MainWindow::on_udp_unbind_btn_clicked()
{
    m_pUdpSocket->abort();
}

// 加入组按钮
void MainWindow::on_udp_join_multi_btn_clicked()
{

}

// 退出组按钮
void MainWindow::on_udp_leave_multi_btn_clicked()
{

}

// 读数据
void MainWindow::Udp_SocketReadyRead()
{
    while (m_pUdpSocket->hasPendingDatagrams()) {
        QByteArray dataGram;
        dataGram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
        destination_add dest_add;
        m_pUdpSocket->readDatagram(dataGram.data(), dataGram.size(), &dest_add.peerAddress, &dest_add.peerPort);
        dest_add.peerAddress_port=QString("%1,%2").arg(dest_add.peerAddress.toString()).arg(dest_add.peerPort);
        if(!m_destination_add_p->contains(dest_add)){
            m_destination_add_p->append(dest_add);
        }
        QString msg = dataGram.data();
        ui->udp_listWidget->addItem(QString("站点(ip:%1 port:%2)-->%3").arg(dest_add.peerAddress.toString()).arg(dest_add.peerPort).arg(msg));
        ui->udp_listWidget->scrollToBottom();
    }
}

// 清理历史
void MainWindow::on_udp_history_clear_clicked()
{
    ui->udp_listWidget->clear();
}



// 套接字 状态变化
void MainWindow::Udp_SocketStateChanged(QAbstractSocket::SocketState SocketState)
{
    switch(SocketState){
    case QAbstractSocket::UnconnectedState:
        Udp_SocketUnBind();
        break;

    case QAbstractSocket::HostLookupState:
        qDebug()<<"udp ConnectedState";
        break;

    case QAbstractSocket::ConnectingState:
        qDebug()<<"udp ConnectedState";
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<"udp ConnectedState";
        break;

    case QAbstractSocket::BoundState:
        Udp_SocketBind();
        break;

    case QAbstractSocket::ListeningState:
        qDebug()<<"udp ListeningState";
        break;

    case QAbstractSocket::ClosingState:
        qDebug()<<"udp ClosingState";
        break;
    }
}





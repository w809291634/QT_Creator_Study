#include "mainwindow.h"
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
    // 设置ttl
    m_pUdpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,16);

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

/******* 后面全部移动到 UDP.c *******/

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
                Peer_Address _dest;
                _dest.peerAddress=QHostAddress(ip.toString());
                _dest.peerAddress_port=ip.toString()+",9090";
                _dest.peerPort=9090;

                //  筛选出能够监听的 ip地址
                if(m_pUdpSocket->bind(ip)){
                    m_pUdpSocket->abort();

                    // 将优先地址 排在 前面
                    char _flag=0;
                    for(auto _ip:FIRST_IP_ARRDESS){
                        if(ip.toString().contains(_ip)){
                            ui->udp_bind_ip_cbox->insertItem(0,ip.toString());
                            if(m_Peer_Address_p!=nullptr){
                                m_Peer_Address_p->insert(0,_dest);
                            }
                            Priority_ip=ip.toString();
                            _flag=1;
                            break;
                        }
                    }
                    // 不在优先选择地址，放在队列尾部
                    if(!_flag){
                        ui->udp_bind_ip_cbox->addItem(ip.toString());
                        if(m_Peer_Address_p!=nullptr){
                            m_Peer_Address_p->append(_dest);
                        }
                    }
                }
            }
        }
    }

    ui->udp_destination_cbox->setCurrentIndex(0);
    ui->udp_bind_ip_cbox->setCurrentIndex(0);
    return Priority_ip;
}

/******* UI 状态 *******/
void MainWindow::Udp_Init_Ui()
{
    // 绑定 IP 和 端口的 ui 设置
    ui->udp_bind_port_sbox->setRange(1,65535);
    ui->udp_bind_port_sbox->setValue(9000);

    // 绑定 广播 IP 和 端口的 ui 设置
    ui->udp_bind_bCast_ip_cbox->addItem("0.0.0.0");
    ui->udp_bind_bCast_port_sbox->setRange(1,65535);
    ui->udp_bind_bCast_port_sbox->setValue(9100);
    ui->udp_bind_bCast_ip_cbox->setEnabled(false);
    ui->udp_bind_bCast_port_sbox->setEnabled(false);

    // 组播 IP 和 端口的 ui 设置
    ui->udp_group_ip_cbox->setEditable(true);  //下拉框可编辑输入
    ui->udp_group_ip_cbox->addItem("239.0.0.1");

    // 正则匹配 D类IP：224.0.0.0~239.255.255.255
    // .必须使用转义字符\，否则.会匹配任意字符
    // C++中"\"在字符串中表示要用"\\"
    QRegExp regExp("^(22[4-9]|23[0-9])"
                   "(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}$");
    QValidator* pValidator = new QRegExpValidator(regExp, this);
    ui->udp_group_ip_cbox->setValidator(pValidator);

    ui->udp_group_port_sbox->setRange(1,65535);
    ui->udp_group_port_sbox->setValue(9100);

    // 发送目的地址 IP 和 端口的 ui 设置
    // 正则匹配 表达只能输入 0.0.0.0,1~255.255.255.255,65535
    // 逗号前面为 IP地址0.0.0.0~255.255.255.255  后面为 端口号1-65535
    ui->udp_destination_cbox->setEditable(true);
    QRegExp dest_regExp("^((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))"
                        "(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}"
                        "(\\,([1-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5]))$");
    pValidator = new QRegExpValidator(dest_regExp, this);
    ui->udp_destination_cbox->setValidator(pValidator);
}

// UDP 绑定状态
void MainWindow::Udp_SocketBind()
{
    // 获取 本地IP 和 端口
    QString localAddress = m_pUdpSocket->localAddress().toString();
    quint16 localPort = m_pUdpSocket->localPort();

    // 触发 绑定 IP 的按钮
    if(localAddress==ui->udp_bind_ip_cbox->currentText()&&
            localPort==ui->udp_bind_port_sbox->value()){
        ui->udp_bind_btn->setEnabled(false);
        ui->udp_bind_ip_cbox->setEnabled(false);
        ui->udp_bind_port_sbox->setEnabled(false);
    }

    // 触发 绑定 广播IP 的按钮
    if(localAddress==ui->udp_bind_bCast_ip_cbox->currentText()&&
            localPort==ui->udp_bind_bCast_port_sbox->value())
        ui->udp_bind_bCast_btn->setEnabled(false);

    ui->udp_unbind_btn->setEnabled(true);

    // 显示信息
    QString info = QString("绑定成功:IP地址:%1 端口:%2")
            .arg(localAddress)
            .arg(localPort);
    ui->statusBar->showMessage(info);
    ui->udp_listWidget->addItem(info);
    ui->udp_listWidget->scrollToBottom();

    // 如果 udp_bind_ip_cbox 中没有这个IP地址，则添加并显示
    if(ui->udp_bind_ip_cbox->findText(localAddress)==-1)
        ui->udp_bind_ip_cbox->addItem(localAddress);

    ui->udp_current_ip_lEdit->setText(localAddress);
    ui->udp_current_port_lEdit->setText(QString("%1").arg(localPort));

    // 更新标志位
    m_Bound=true;
}

// UDP 解除绑定状态
void MainWindow::Udp_SocketUnBind()
{
    // 按钮
    ui->udp_bind_btn->setEnabled(true);
    ui->udp_bind_bCast_btn->setEnabled(true);
    ui->udp_unbind_btn->setEnabled(false);
    // 设置 绑定IP
    ui->udp_bind_ip_cbox->setEnabled(true);
    ui->udp_bind_port_sbox->setEnabled(true);
    ui->udp_current_ip_lEdit->setEnabled(false);
    ui->udp_current_port_lEdit->setEnabled(false);
    ui->udp_current_ip_lEdit->setText("请绑定...");
    ui->udp_current_port_lEdit->setText("请绑定...");
    // 状态
    QString info("UDP套接字解除绑定");
    ui->statusBar->showMessage(info);
    ui->udp_listWidget->addItem(info);
    ui->udp_listWidget->scrollToBottom();

    // 更新标志位
    m_Bound=false;
}

// 加入 组播
void MainWindow::Udp_SocketJoinMulti()
{
    // 获取 本地IP 和 端口
    QString localAddress = m_pUdpSocket->localAddress().toString();
    quint16 localPort = m_pUdpSocket->localPort();

    // 按钮 变化
    ui->udp_join_multi_btn->setEnabled(false);
    ui->udp_leave_multi_btn->setEnabled(true);

    // 发送组消息按钮
    ui->udp_multicast_btn->setEnabled(true);

    // 编辑栏
    ui->udp_group_ip_cbox->setEnabled(false);
    ui->udp_group_port_sbox->setEnabled(false);

    // 信息
    QString join_info=QString("IP:%1 端口:%2 加入组成功").arg(localAddress).arg(localPort);
    QString group_info=QString("组:IP:%1 端口:%2")
            .arg(m_multicastAddr.toString()).arg(m_multicastPort);

    ui->statusBar->showMessage(join_info);
    ui->udp_listWidget->addItem(join_info);
    ui->udp_listWidget->addItem(group_info);
    ui->udp_listWidget->scrollToBottom();

}

// 退出 组播
void MainWindow::Udp_SocketLeaveMulti()
{
    // 获取 本地IP 和 端口
    QString localAddress = m_pUdpSocket->localAddress().toString();
    quint16 localPort = m_pUdpSocket->localPort();

    // 按钮 变化
    ui->udp_join_multi_btn->setEnabled(true);
    ui->udp_leave_multi_btn->setEnabled(false);
    // 发送组消息按钮
    ui->udp_multicast_btn->setEnabled(false);
    // 编辑栏
    ui->udp_group_ip_cbox->setEnabled(true);
    ui->udp_group_port_sbox->setEnabled(true);

    // 信息
    QString join_info=QString("IP:%1 端口:%2 退出组成功")
            .arg(localAddress).arg(localPort);

    ui->statusBar->showMessage(join_info);
    ui->udp_listWidget->addItem(join_info);
    ui->udp_listWidget->scrollToBottom();
}

// 更新本地 目的地 信息
void MainWindow::Udp_Destination_Update()
{
    static int last_len=-1;
    if(m_Peer_Address_p!=nullptr){
        int len=m_Peer_Address_p->length();
        if(len!=last_len){
            // 需要 更新显示
            last_len=len;
            // 先 清理 后添加
            ui->udp_destination_cbox->clear();
            foreach(auto destination,*m_Peer_Address_p.data()){
                if(destination.is_vaild()){
                    ui->udp_destination_cbox->addItem(destination.peerAddress_port);
                }
            }
            ui->udp_destination_cbox->setCurrentIndex(0);
        }
    }
}

/******* 按钮槽函数 *******/
// 绑定按钮
void MainWindow::on_udp_bind_btn_clicked()
{
    m_pUdpSocket->abort();
    QHostAddress bind_ip(ui->udp_bind_ip_cbox->currentText());
    quint16 bind_port=static_cast<quint16>(ui->udp_bind_port_sbox->value());

    // 设置 同一个地址和端口 可以多次绑定
    if (!m_pUdpSocket->bind(bind_ip,bind_port,
                            QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        QMessageBox::warning(this,"绑定失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 绑定广播地址
void MainWindow::on_udp_bind_bCast_btn_clicked()
{
    m_pUdpSocket->abort();
    // 需要绑定到 0.0.0.0 地址上 ，端口也需要统一
    QHostAddress bind_ip(ui->udp_bind_bCast_ip_cbox->currentText());
    quint16 bind_port=static_cast<quint16>(ui->udp_bind_bCast_port_sbox->value());

    // 设置 同一个地址和端口 可以多次绑定
    // IP可以使用QHostAddress::Any或者不输入参数。
    if (!m_pUdpSocket->bind(bind_ip,bind_port,QAbstractSocket::ShareAddress|
                            QAbstractSocket::ReuseAddressHint)) {
        QMessageBox::warning(this,"绑定失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 解除绑定按钮
void MainWindow::on_udp_unbind_btn_clicked()
{
    on_udp_leave_multi_btn_clicked();       // 先退出组
    m_pUdpSocket->abort();
}

// 加入组按钮
void MainWindow::on_udp_join_multi_btn_clicked()
{
    QString ip = ui->udp_group_ip_cbox->currentText();
    m_multicastAddr = QHostAddress(ip);                                 // 获取组播的IP地址
    m_multicastPort =
            static_cast<quint16>(ui->udp_group_port_sbox->value());     // 选择自己绑定的一个端口

    m_pUdpSocket->abort();


    if (m_pUdpSocket->bind(QHostAddress::AnyIPv4, m_multicastPort,
                           QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        if(m_pUdpSocket->joinMulticastGroup(m_multicastAddr))
            Udp_SocketJoinMulti();
        else {
            Udp_SocketLeaveMulti();
        }
    }

}

// 退出组按钮
void MainWindow::on_udp_leave_multi_btn_clicked()
{
    m_pUdpSocket->leaveMulticastGroup(m_multicastAddr);     // 退出组播
    Udp_SocketLeaveMulti();                                 // 更新退出组的状态
}

// 单播
void MainWindow::on_udp_unicast_btn_clicked()
{
    QString notes("单播消息");
    QByteArray msg = ui->udp_send_textEdit->toPlainText().toUtf8();
    // 根据选择 获取 目标地址和端口
    QString target = ui->udp_destination_cbox->currentText();
    // 搜索出 储存的 目标地址
    foreach(auto peer,*m_Peer_Address_p.data()){
        if(target==peer.peerAddress_port
                && peer.is_vaild())
        {
            Udp_SocketWrite(msg,peer.peerAddress,peer.peerPort,notes);
        }
    }
}

// 广播
void MainWindow::on_udp_broadcast_btn_clicked()
{
    QString notes("广播消息");
    QByteArray msg = ui->udp_send_textEdit->toPlainText().toUtf8();
    Peer_Address peer;

    // 选择 统一的 广播端口
    peer.peerPort = static_cast<quint16>(ui->udp_bind_bCast_port_sbox->value());
    // 设置 IP 为广播地址，向全网段发送广播消息
    peer.peerAddress = QHostAddress::Broadcast;

    Udp_SocketWrite(msg,peer.peerAddress,peer.peerPort,notes);
}

// 组播
void MainWindow::on_udp_multicast_btn_clicked()
{
    QString notes("组播消息");
    QByteArray msg = ui->udp_send_textEdit->toPlainText().toUtf8();

    Udp_SocketWrite(msg,m_multicastAddr,m_multicastPort,notes);
}

// 清理历史
void MainWindow::on_udp_history_clear_clicked()
{
    ui->udp_listWidget->clear();
}

/******* 数据和状态 *******/

// 读数据
void MainWindow::Udp_SocketRead()
{
    while(m_pUdpSocket->hasPendingDatagrams()) {
        QByteArray dataGram;
        dataGram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
        // 发送方
        Peer_Address peer;
        QHostAddress* peerAddress = &peer.peerAddress;
        quint16* peerPort = &peer.peerPort;

        // 读取数据
        m_pUdpSocket->readDatagram(dataGram.data(), dataGram.size()
                                   , peerAddress, peerPort);
        peer.peerAddress_port=QString("%1,%2")
                .arg(peerAddress->toString())
                .arg(*peerPort);
        // 接收方
        QString localAddress = m_pUdpSocket->localAddress().toString();
        quint16 localPort = m_pUdpSocket->localPort();

        // 判断地址 格式是否 有效
        if(peer.is_vaild()){
            // 如果没有储存 进行储存
            if(!m_Peer_Address_p->contains(peer)){
                m_Peer_Address_p->append(peer);
            }
            // 获取 数据报 里面的数据
            QString msg = dataGram.data();
            ui->udp_listWidget->addItem(QString("站点(ip:%1 port:%2)-->"
                                                "本站(ip:%3 port:%4) %5")
                                        .arg(peerAddress->toString())
                                        .arg(*peerPort)
                                        .arg(localAddress)
                                        .arg(localPort)
                                        .arg(msg));
            ui->udp_listWidget->scrollToBottom();
        }
    }
}

// 写数据
void MainWindow::Udp_SocketWrite(const QByteArray &datagram,
                                 const QHostAddress &host,
                                 quint16 port,
                                 QString notes)
{
    // 没有考虑 数据量 比较大的情况
    if(datagram.size()==m_pUdpSocket->writeDatagram(datagram, host, port)){
        QString localAddress = m_pUdpSocket->localAddress().toString();
        quint16 localPort = m_pUdpSocket->localPort();

        QString info=QString("[%1] "
                             "本站(ip:%2 port:%3)-->"
                             "站点(ip:%4 port:%5) %6")
                            .arg(notes)
                            .arg(localAddress)
                            .arg(localPort)
                            .arg(host.toString())
                            .arg(port)
                            .arg(QString(datagram));
        ui->udp_listWidget->addItem(info);
        ui->udp_listWidget->scrollToBottom();
    }
}

// 套接字 状态变化
void MainWindow::Udp_SocketStateChanged(QAbstractSocket::SocketState SocketState)
{
    switch(SocketState){
    case QAbstractSocket::UnconnectedState:
        ui->statusBar->showMessage("状态：套接字解除绑定");
        Udp_SocketUnBind();
        break;

    case QAbstractSocket::HostLookupState:
        ui->statusBar->showMessage("状态：套接字正在搜索");
        break;

    case QAbstractSocket::ConnectingState:
        ui->statusBar->showMessage("状态：套接字正在连接");
        break;

    case QAbstractSocket::ConnectedState:
        ui->statusBar->showMessage("状态：套接字已连接");
        break;

    case QAbstractSocket::BoundState:
        Udp_SocketBind();
        break;

    case QAbstractSocket::ListeningState:
        ui->statusBar->showMessage("状态：套接字监听状态");
        break;

    case QAbstractSocket::ClosingState:
        ui->statusBar->showMessage("状态：套接字关闭状态");
        break;
    }
}








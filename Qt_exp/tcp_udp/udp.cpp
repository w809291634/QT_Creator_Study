#include "mainwindow.h"
#include "ui_mainwindow.h"

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
                            ui->udp_bind_ip_cbox_2->insertItem(0,ip.toString());
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
                        ui->udp_bind_ip_cbox_2->addItem(ip.toString());
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

    // 组播示例单独添加 特殊地址
    ui->udp_bind_ip_cbox_2->insertItem(0,m_any_ipv4.toString());
    ui->udp_bind_ip_cbox_2->setCurrentIndex(0);

    return Priority_ip;
}

// TAB 页切换
void MainWindow::on_tabWidget_2_currentChanged(int index)
{
    (void)index;
    on_udp_leave_multi_btn_clicked();
    m_pUdpSocket->abort();
}

// 初始化 UI
void MainWindow::Udp_Init_Ui()
{
    /********* UDP 单播和广播*********/
    // 绑定 IP 和 端口的 ui 设置
    ui->udp_bind_port_sbox->setRange(1,65535);
    ui->udp_bind_port_sbox->setValue(9090);

    // 绑定 广播 IP 和 端口的 ui 设置
    ui->udp_bind_special_ip->setText(m_any_ipv4.toString());
    ui->udp_bind_special_ip->setEnabled(false);
    ui->udp_destination_port->setRange(1,65535);
    ui->udp_destination_port->setValue(9090);
    ui->udp_destination_port->setEnabled(true);

    // 发送目的地址 IP 和 端口的 ui 设置
    // 正则匹配 表达只能输入 0.0.0.0,1~255.255.255.255,65535
    // 逗号前面为 IP地址0.0.0.0~255.255.255.255  后面为 端口号1-65535
    ui->udp_destination_cbox->setEditable(true);
    QRegExp dest_regExp("^((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))"
                        "(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}"
                        "(\\,([1-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5]))$");
    QValidator* pValidator = new QRegExpValidator(dest_regExp, this);
    ui->udp_destination_cbox->setValidator(pValidator);

    /********* UDP 组播*********/
    // 组播 IP 和 端口的 ui 设置
    ui->udp_group_ip_cbox->setEditable(true);  //下拉框可编辑输入
    ui->udp_group_ip_cbox->addItem("239.0.0.1");

    // 正则匹配 D类IP：224.0.0.0~239.255.255.255
    // .必须使用转义字符\，否则.会匹配任意字符
    // C++中"\"在字符串中表示要用"\\"
    QRegExp regExp("^(22[4-9]|23[0-9])"
                   "(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}$");
    pValidator = new QRegExpValidator(regExp, this);
    ui->udp_group_ip_cbox->setValidator(pValidator);

    ui->udp_group_port_sbox->setRange(1,65535);
    ui->udp_group_port_sbox->setValue(9090);
}

// UDP 绑定状态
void MainWindow::Udp_SocketBind()
{
    // 更新标志位
    m_Bound=true;

    // 获取 本地IP 和 端口
    QString localAddress = m_pUdpSocket->localAddress().toString();
    quint16 localPort = m_pUdpSocket->localPort();

    // 显示信息
    QString info = QString("绑定成功:IP地址:%1 端口:%2")
            .arg(localAddress)
            .arg(localPort);
    ui->statusBar->showMessage(info);

    /** UDP 单播和广播 **/
    {
        // 触发 绑定 IP 的按钮
        if(localAddress==ui->udp_bind_ip_cbox->currentText()){
            ui->udp_bind_btn->setEnabled(false);
            ui->udp_bind_ip_cbox->setEnabled(false);
            ui->udp_bind_special_btn->setEnabled(true);
        }
        // 触发 绑定 广播IP 的按钮
        if(localAddress==ui->udp_bind_special_ip->text()){
            ui->udp_bind_btn->setEnabled(true);
            ui->udp_bind_ip_cbox->setEnabled(true);
            ui->udp_bind_special_btn->setEnabled(false);
        }
        ui->udp_bind_port_sbox->setEnabled(false);
        ui->udp_unbind_btn->setEnabled(true);

        // 如果没有这个IP地址，则添加并显示
        if(ui->udp_bind_ip_cbox->findText(localAddress)==-1)
            ui->udp_bind_ip_cbox->addItem(localAddress);

        // 显示当前IP
        ui->udp_current_ip_lEdit->setText(localAddress);
        ui->udp_current_port_lEdit->setText(QString("%1").arg(localPort));

        // 显示信息
        ui->udp_listWidget->addItem(info);
        ui->udp_listWidget->scrollToBottom();
    }

    /** UDP 组播 **/
    {
        // 按钮
        ui->udp_join_multi_btn->setEnabled(false);
        ui->udp_leave_multi_btn->setEnabled(true);

        // 复选器
        ui->udp_bind_ip_cbox_2->setEnabled(false);
        ui->udp_group_port_sbox->setEnabled(false);

        // 如果没有这个IP地址，则添加并显示
        if(ui->udp_bind_ip_cbox_2->findText(localAddress)==-1)
            ui->udp_bind_ip_cbox_2->addItem(localAddress);

        // 显示当前IP
        ui->udp_current_ip_lEdit_2->setText(localAddress);
        ui->udp_current_port_lEdit_2->setText(QString("%1").arg(localPort));

        ui->udp_listWidget_2->addItem(info);
        ui->udp_listWidget_2->scrollToBottom();
    }
}

// UDP 解除绑定状态
void MainWindow::Udp_SocketUnBind()
{
    // 更新标志位
    m_Bound=false;

    QString info("UDP套接字解除绑定");
    ui->statusBar->showMessage(info);

    // 更新信息
    ui->udp_listWidget->addItem(info);
    ui->udp_listWidget->scrollToBottom();

    /** UDP 单播和广播 **/
    {
        ui->udp_bind_btn->setEnabled(true);
        ui->udp_bind_special_btn->setEnabled(true);
        ui->udp_unbind_btn->setEnabled(false);

        // 设置 绑定IP
        ui->udp_bind_ip_cbox->setEnabled(true);
        ui->udp_bind_port_sbox->setEnabled(true);
        ui->udp_current_ip_lEdit->setEnabled(false);
        ui->udp_current_port_lEdit->setEnabled(false);
        ui->udp_current_ip_lEdit->setText("请绑定...");
        ui->udp_current_port_lEdit->setText("请绑定...");
    }

    /** UDP 组播 **/
    {
        // 按钮
        ui->udp_join_multi_btn->setEnabled(true);
        ui->udp_leave_multi_btn->setEnabled(false);
        // 复选器
        ui->udp_bind_ip_cbox_2->setEnabled(true);
        ui->udp_bind_ip_cbox_2->setEnabled(true);
        ui->udp_current_ip_lEdit_2->setEnabled(false);
        ui->udp_current_port_lEdit_2->setEnabled(false);
        ui->udp_current_ip_lEdit_2->setText("请绑定...");
        ui->udp_current_port_lEdit_2->setText("请绑定...");
        // 信息
        ui->udp_listWidget_2->addItem(info);
        ui->udp_listWidget_2->scrollToBottom();
    }
}

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
                m_Peer_Address_p->insert(0,peer);
            }
            // 获取 数据报 里面的数据
            QString msg = dataGram.data();
            // 信息
            QString info = QString("站点(ip:%1 port:%2)-->"
                                  "本站(ip:%3 port:%4) %5")
                          .arg(peerAddress->toString())
                          .arg(*peerPort)
                          .arg(localAddress)
                          .arg(localPort)
                          .arg(msg);

            if(ui->tabWidget_2->currentIndex()==0)
            {
                ui->udp_listWidget->addItem(info);
                ui->udp_listWidget->scrollToBottom();
            }
            else if(ui->tabWidget_2->currentIndex()==1)
            {
                ui->udp_listWidget_2->addItem(info);
                ui->udp_listWidget_2->scrollToBottom();
            }
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
        // 信息
        QString info=QString("[%1] "
                             "本站(ip:%2 port:%3)-->"
                             "站点(ip:%4 port:%5) %6")
                            .arg(notes)
                            .arg(localAddress)
                            .arg(localPort)
                            .arg(host.toString())
                            .arg(port)
                            .arg(QString(datagram));
        if(ui->tabWidget_2->currentIndex()==0)
        {
            ui->udp_listWidget->addItem(info);
            ui->udp_listWidget->scrollToBottom();
        }
        else if(ui->tabWidget_2->currentIndex()==1)
        {
            ui->udp_listWidget_2->addItem(info);
            ui->udp_listWidget_2->scrollToBottom();
        }
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

/********* UDP 单播和广播*********/

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

/****槽函数****/
// 绑定按钮
void MainWindow::on_udp_bind_btn_clicked()
{
    m_pUdpSocket->abort();
    QHostAddress bind_ip(ui->udp_bind_ip_cbox->currentText());
    quint16 bind_port=static_cast<quint16>(ui->udp_bind_port_sbox->value());

    // 设置 同一个地址和端口 普通绑定，不允许绑定统一地址
    if (!m_pUdpSocket->bind(bind_ip,bind_port)) {
        QMessageBox::warning(this,"绑定失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 绑定特殊地址，接收广播消息
void MainWindow::on_udp_bind_special_btn_clicked()
{
    m_pUdpSocket->abort();

    QHostAddress bind_ip(ui->udp_bind_special_ip->text());
    quint16 bind_port=static_cast<quint16>(ui->udp_bind_port_sbox->value());

    //设置 同一个地址和端口 可以多次绑定
    //IP可以使用QHostAddress::Any或者不输入参数。
    if (!m_pUdpSocket->bind(bind_ip,bind_port,QAbstractSocket::ShareAddress|
                            QAbstractSocket::ReuseAddressHint)) {
        QMessageBox::warning(this,"绑定失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 解除绑定按钮
void MainWindow::on_udp_unbind_btn_clicked()
{
    m_pUdpSocket->abort();
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

    // 选择 一个 广播 端口
    peer.peerPort = static_cast<quint16>(ui->udp_destination_port->value());
    // 设置 IP 为广播地址，向全网段发送广播消息
    peer.peerAddress = QHostAddress::Broadcast;

    Udp_SocketWrite(msg,peer.peerAddress,peer.peerPort,notes);
}

// 清理历史
void MainWindow::on_udp_history_clear_clicked()
{
    ui->udp_listWidget->clear();
}

/********* UDP 组播*********/

/****状态****/
// 加入 组播
void MainWindow::Udp_SocketJoinMulti()
{
    m_multicast=true;

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
    QString group_info=QString("组:IP:%1")
            .arg(m_multicastAddr.toString());

    ui->statusBar->showMessage(join_info);
    ui->udp_listWidget_2->addItem(join_info);
    ui->udp_listWidget_2->addItem(group_info);
    ui->udp_listWidget_2->scrollToBottom();
}

// 退出 组播
void MainWindow::Udp_SocketLeaveMulti()
{
    m_multicast=false;
    m_pUdpSocket->abort();

    // 清理信息
    m_multicastAddr=QHostAddress("");
    m_multicastPort=0;

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
    ui->udp_listWidget_2->addItem(join_info);
    ui->udp_listWidget_2->scrollToBottom();
}

/****槽函数****/

// 加入组按钮
void MainWindow::on_udp_join_multi_btn_clicked()
{
    QString ip = ui->udp_group_ip_cbox->currentText();
    // 获取组IP和端口
    m_multicastAddr = QHostAddress(ip);
    m_multicastPort =
            static_cast<quint16>(ui->udp_group_port_sbox->value());
    // 获取绑定地址
    QHostAddress _bind_ip=QHostAddress(ui->udp_bind_ip_cbox_2->currentText());

    if (m_pUdpSocket->bind(_bind_ip, m_multicastPort,
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
    if(m_multicast){
        if(m_pUdpSocket->leaveMulticastGroup(m_multicastAddr)){
            Udp_SocketLeaveMulti();
        }
    }
}

// 组播消息
void MainWindow::on_udp_multicast_btn_clicked()
{
    QString notes("组播消息");
    QByteArray msg = ui->udp_send_textEdit_2->toPlainText().toUtf8();

    Udp_SocketWrite(msg,m_multicastAddr,m_multicastPort,notes);
}

// 清理历史
void MainWindow::on_udp_history_clear_2_clicked()
{
    ui->udp_listWidget_2->clear();
}



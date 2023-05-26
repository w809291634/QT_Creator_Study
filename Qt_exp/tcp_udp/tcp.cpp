#include "mainwindow.h"
#include "ui_mainwindow.h"

// 获取 本地 能够 监听的地址
QString MainWindow::TCP_getLocalIP()
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
                if(m_TcpServer->listen(ip)){
                    char _flag=0;
                    m_TcpServer->close();
                    // 将优先地址 排在 前面
                    for(auto _ip:FIRST_IP_ARRDESS){
                        if(ip.toString().contains(_ip)){
                            ui->tcp_s_ip_cbox->insertItem(0,ip.toString());
                            Priority_ip=ip.toString();
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
    return Priority_ip;
}

/*************** TCP服务器 ***************/
// 服务器 开启监听后的 UI变化
void MainWindow::tcp_s_listening()
{
    ui->tcp_server_listWidget->addItem(QString("服务器开始监听"));
    ui->tcp_server_listWidget->scrollToBottom();

    ui->tcp_s_listen_btn->setEnabled(false);
    ui->tcp_s_close_btn->setEnabled(true);
    ui->tcp_s_ip_cbox->setEnabled(false);
    ui->tcp_s_port_ledit->setEnabled(false);
    ui->tcp_s_send_btn->setEnabled(true);
}

// 服务器 停止监听后的 UI变化
void MainWindow::tcp_s_listen_close()
{
    ui->tcp_server_listWidget->addItem(QString("服务器停止监听"));
    ui->tcp_server_listWidget->scrollToBottom();

    ui->tcp_s_listen_btn->setEnabled(true);
    ui->tcp_s_close_btn->setEnabled(false);
    ui->tcp_s_ip_cbox->setEnabled(true);
    ui->tcp_s_port_ledit->setEnabled(true);
    ui->tcp_s_send_btn->setEnabled(false);
}

// 监听开始 按钮触发
void MainWindow::on_tcp_s_listen_btn_clicked()
{
    QHostAddress ip_add(ui->tcp_s_ip_cbox->currentText());
    unsigned short port = ui->tcp_s_port_ledit->text().toUShort();

    // 服务器开始监听
    if(m_TcpServer->listen(ip_add, port)){
        tcp_s_listening();
    }else{
        tcp_s_listen_close();
        QMessageBox::warning(this,"监听失败","请检查服务器IP地址或者端口是否占用","确认");
    }
}

// 监听停止 按钮触发
void MainWindow::on_tcp_s_close_btn_clicked()
{
    m_TcpServer->close();               // 停止监听
    tcp_s_listen_close();               // 进行UI变化
}

// 服务器端 断开 所有的连接
void MainWindow::on_tcp_s_unconnect_btn_clicked()
{
    foreach(auto Client,*m_Tcp_ClientList.data()){
        Client->abort();                // 终止连接
        Client->close();
        delete Client;                  // 释放内存
    }
    m_Tcp_ClientList->clear();          // 清理所有的存储套接字
}

// 服务器发送数据按钮
void MainWindow::on_tcp_s_send_btn_clicked()
{
    tcp_s_write();
}

// 服务器 清除 历史按钮
void MainWindow::on_tcp_s_history_clear_clicked()
{
    ui->tcp_server_listWidget->clear();
}

// 服务器收到 新的客户端 连接通知
void MainWindow::tcp_s_newConnection_trigger()
{
    QTcpSocket* temp_socket=m_TcpServer->nextPendingConnection();

    // 有 新的服务端 套接字
    if(temp_socket!=nullptr){
        // 判断这个套接字是否存储
        if(!m_Tcp_ClientList->contains(temp_socket))
            m_Tcp_ClientList->append(temp_socket);

        ui->tcp_server_listWidget->
                addItem(QString("IP地址:%1 端口:%2 已连接服务器")
                        .arg(temp_socket->peerAddress().toString())
                        .arg(temp_socket->peerPort()));
        ui->tcp_server_listWidget->scrollToBottom();
        // 服务端 套接字 信号连接
        connect(temp_socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this,SLOT(tcp_s_SocketState_Changed(QAbstractSocket::SocketState)));
        connect(temp_socket,SIGNAL(readyRead()),this,SLOT(tcp_s_read()));
    }
}

// 服务器 读取 客户端数据
void MainWindow::tcp_s_read()
{
    //获得信号的发送者的套接字
    QTcpSocket *temp_socket = dynamic_cast<QTcpSocket*>(sender());
    //读取数据
    QString msg = temp_socket->readAll();
    QString ip = temp_socket->peerAddress().toString();
    quint16 port = temp_socket->peerPort();
    //调用文本浏览框，显示内容
    ui->tcp_server_listWidget->addItem(QString("客户端(ip:%1 port:%2)-->%3").arg(ip).arg(port).arg(msg));
    ui->tcp_server_listWidget->scrollToBottom();
}

// 服务器 发送 数据
void MainWindow::tcp_s_write()
{
    // 获取 当前 选择的客户端套接字
    QString current_item=ui->tcp_s_select_cbox->currentText();
    foreach(auto Client,*m_Tcp_ClientList.data()){
        if(QString("%1,%2").arg(Client->peerAddress().toString()).
                arg(Client->peerPort())==current_item)
        {
            if(Client->state()== QAbstractSocket::ConnectedState){
                QString msg = ui->tcp_s_send_textEdit->toPlainText();           // 取出发送信息框的内容
                if(msg.isEmpty())return;
                if(Client->write(msg.toUtf8())==msg.toUtf8().size()){
                    QString ip = Client->peerAddress().toString();
                    quint16 port = Client->peerPort();
                    ui->tcp_server_listWidget->addItem(QString("客户端(ip:%1 port:%2)<--%3").arg(ip).arg(port).arg(msg));   // 添加到历史
                    ui->tcp_server_listWidget->scrollToBottom();
                    ui->tcp_s_send_state->setText("发送成功");
                    m_stimer->start(1000);

                }
            }
            break;
        }
    }
}

// TCP 服务器 错误信息提取
void MainWindow::tcp_s_acceptError(QAbstractSocket::SocketError socketError)
{
    QString str=QString("TCP服务器错误码:%1").arg(socketError);
    ui->tcp_server_listWidget->addItem(str);
    ui->tcp_server_listWidget->scrollToBottom();
}

// 更新 已经连接的 客户端信息
void MainWindow::tcp_s_update_ClientInfo()
{
    static int list_len=-1;
    if(list_len!=m_Tcp_ClientList->length()){
        list_len=m_Tcp_ClientList->length();
        // 重新填充 项目
        ui->tcp_s_select_cbox->clear();
        foreach(auto Client,*m_Tcp_ClientList.data()){
            ui->tcp_s_select_cbox->addItem(
                        QString("%1,%2").arg(Client->peerAddress().toString())
                        .arg(Client->peerPort()));
        }
    }

    if(m_Tcp_ClientList->length()){
        ui->tcp_s_unconnect_btn->setEnabled(true);
    }else{
        ui->tcp_s_unconnect_btn->setEnabled(false);
    }
}

// 服务端 套接字 状态变化
void MainWindow::tcp_s_SocketState_Changed(QAbstractSocket::SocketState SocketState)
{
    //获得信号的发送者的套接字
    QTcpSocket *temp_socket = dynamic_cast<QTcpSocket*>(sender());

    switch(SocketState){
    case QAbstractSocket::UnconnectedState:
        qDebug()<<"tcp_s UnconnectedState";
        m_Tcp_ClientList->removeOne(temp_socket);
        ui->tcp_server_listWidget->
                addItem(QString("IP地址:%1 端口:%2 已断开服务器")
                        .arg(temp_socket->peerAddress().toString())
                        .arg(temp_socket->peerPort()));
        ui->tcp_server_listWidget->scrollToBottom();
        break;

    case QAbstractSocket::HostLookupState:
        qDebug()<<"tcp_s HostLookupState";
        break;

    case QAbstractSocket::ConnectingState:
        qDebug()<<"tcp_s ConnectingState";
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<"tcp_s ConnectedState";
        if(!m_Tcp_ClientList->contains(temp_socket))
            m_Tcp_ClientList->append(temp_socket);
        break;

    case QAbstractSocket::BoundState:
        qDebug()<<"tcp_s BoundState";
        break;

    case QAbstractSocket::ListeningState:
        qDebug()<<"tcp_s ListeningState";
        break;

    case QAbstractSocket::ClosingState:
        m_Tcp_ClientList->removeOne(temp_socket);
        break;
    }
}

//  清除状态
void MainWindow::tcp_s_clear()
{
    ui->tcp_s_send_state->setText("");
}

/*************** TCP客户端 ***************/
// TCP 客户端 正在连接
void MainWindow::tcp_c_Connecting()
{
    ui->tcp_c_ip_ledit->setText("连接服务器中...");
    ui->tcp_c_port_ledit->setText("连接服务器中...");
    ui->tcp_c_connect_btn->setEnabled(false);
    ui->tcp_cs_ip_ledit->setEnabled(false);
    ui->tcp_cs_port_ledit->setEnabled(false);
    ui->tcp_c_disconnect_btn->setEnabled(true);
    ui->tcp_c_send_btn->setEnabled(true);

    m_timeout_timer->start(TCP_CONNECTING_TIMEOUT*1000);
}

// TCP 客户端 连接超时
void MainWindow::tcp_c_Connecting_timeout()
{
    m_TcpClient->abort();
    QMessageBox::warning(this,"连接超时","请检查服务器IP地址或者端口是否设置正确","确认");
}

// TCP 客户端 连接成功后的UI变化
void MainWindow::tcp_c_Connected()
{
    ui->tcp_c_ip_ledit->setEnabled(false);
    ui->tcp_c_port_ledit->setEnabled(false);

    ui->tcp_c_ip_ledit->setText(m_TcpClient->localAddress().toString());
    ui->tcp_c_port_ledit->setText(QString("%1").arg(m_TcpClient->localPort()));

    ui->tcp_c_connect_btn->setEnabled(false);
    ui->tcp_cs_ip_ledit->setEnabled(false);
    ui->tcp_cs_port_ledit->setEnabled(false);
    ui->tcp_c_disconnect_btn->setEnabled(true);
    ui->tcp_c_send_btn->setEnabled(true);

    ui->tcp_client_listWidget->addItem("已连接服务器");
    ui->tcp_client_listWidget->scrollToBottom();

    if(m_timeout_timer!=nullptr)
        m_timeout_timer->stop();
}

// TCP 客户端 连接失败后的UI变化
void MainWindow::tcp_c_Unconnected()
{
    ui->tcp_c_ip_ledit->setEnabled(false);
    ui->tcp_c_port_ledit->setEnabled(false);
    ui->tcp_c_ip_ledit->clear();
    ui->tcp_c_port_ledit->clear();

    ui->tcp_c_connect_btn->setEnabled(true);
    ui->tcp_cs_ip_ledit->setEnabled(true);
    ui->tcp_cs_port_ledit->setEnabled(true);
    ui->tcp_c_disconnect_btn->setEnabled(false);
    ui->tcp_c_send_btn->setEnabled(false);

    ui->tcp_client_listWidget->addItem("已断开服务器");
    ui->tcp_client_listWidget->scrollToBottom();

    if(m_timeout_timer!=nullptr)
        m_timeout_timer->stop();
}

// 触发连接服务器
void MainWindow::on_tcp_c_connect_btn_clicked()
{
    QString ip = ui->tcp_cs_ip_ledit->text();
    unsigned short port = ui->tcp_cs_port_ledit->text().toUShort();

    // 连接服务器
    m_TcpClient->connectToHost(QHostAddress(ip), port);
}

// 触发断开连接服务器
void MainWindow::on_tcp_c_disconnect_btn_clicked()
{
    m_TcpClient->abort();
    m_TcpClient->disconnectFromHost();
}

// 发送信息 按钮触发
void MainWindow::on_tcp_c_send_btn_clicked()
{
    tcp_c_write();
}

// 清除 客户端 历史
void MainWindow::on_tcp_c_history_clear_clicked()
{
    ui->tcp_client_listWidget->clear();
}

// 客户端 发送数据到 服务器
void MainWindow::tcp_c_read()
{
    //获得信号的发送者的套接字
    QTcpSocket *temp_socket = dynamic_cast<QTcpSocket*>(sender());
    //读取数据
    QString msg = temp_socket->readAll();
    QString ip = temp_socket->peerAddress().toString();
    quint16 port = temp_socket->peerPort();
    //调用文本浏览框，显示内容
    ui->tcp_client_listWidget->addItem(QString("服务器(ip:%1 port:%2)-->%3").arg(ip).arg(port).arg(msg));
    ui->tcp_client_listWidget->scrollToBottom();
}

// 客户端 读取 服务器 发送来的数据
void MainWindow::tcp_c_write()
{
    if(m_TcpClient->state()== QAbstractSocket::ConnectedState){
        QString msg = ui->tcp_c_send_textEdit->toPlainText();           // 取出发送信息框的内容
        if(msg.isEmpty())return;
        if(m_TcpClient->write(msg.toUtf8())==msg.toUtf8().size()){
            QString ip = m_TcpClient->peerAddress().toString();
            quint16 port = m_TcpClient->peerPort();
            ui->tcp_client_listWidget->addItem(QString("服务器(ip:%1 port:%2)<--%3").arg(ip).arg(port).arg(msg));   // 添加到历史
            ui->tcp_client_listWidget->scrollToBottom();
            ui->tcp_c_send_state->setText("发送成功");
            m_ctimer->start(1000);
        }
    }
}

// 套接字 状态 变化
void MainWindow::tcp_c_SocketState_Changed(QAbstractSocket::SocketState SocketState)
{
    switch(SocketState){
    case QAbstractSocket::UnconnectedState:
        qDebug()<<"tcp_c UnconnectedState";
        tcp_c_Unconnected();
        break;

    case QAbstractSocket::HostLookupState:
//        qDebug()<<"tcp_c HostLookupState";
        break;

    case QAbstractSocket::ConnectingState:
        tcp_c_Connecting();
        qDebug()<<"tcp_c ConnectingState";
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<"tcp_c ConnectedState";
        tcp_c_Connected();
        break;

    case QAbstractSocket::BoundState:
//        qDebug()<<"tcp_c BoundState";
        break;

    case QAbstractSocket::ListeningState:
//        qDebug()<<"tcp_c ListeningState";
        break;

    case QAbstractSocket::ClosingState:
        qDebug()<<"tcp_c ClosingState";
        tcp_c_Unconnected();
        break;
    }
}

//  清除状态
void MainWindow::tcp_c_clear()
{
    ui->tcp_c_send_state->setText("");
}

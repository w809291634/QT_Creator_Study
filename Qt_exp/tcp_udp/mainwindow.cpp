#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* TCP服务器 */
    m_Tcp_ClientList.reset(new QList<QTcpSocket *>);
    m_TcpServer = new QTcpServer(this);


    ui->tcp_s_close_btn->setEnabled(false);

    // QNetworkInterface 类提供主机的 IP 地址和网络接口的列表
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, list)
    {
        QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entryList)
        {
            //过滤 IPv6 地址，只留下 IPv4
            if (entry.ip().protocol() ==QAbstractSocket::IPv4Protocol)
            {
                QString ip = entry.ip().toString();
                ui->tcp_s_ip_cbox->addItem(ip);
            }
        }
    }
    // 相关信号 的连接
    connect(m_TcpServer,SIGNAL(newConnection()),
            this, SLOT(tcp_s_newConnection_trigger()));
    connect(m_TcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(tcp_s_acceptError(QAbstractSocket::SocketError)));


    // 更新 已连接 客户端的信息
    QTimer* temp_Timer = new QTimer(this);
    connect(temp_Timer, SIGNAL(timeout()), this, SLOT(tcp_s_update_ClientInfo()));
    temp_Timer->start(1000);


    /* TCP客户端 */
    m_TcpClient = new QTcpSocket(this);

    ui->tcp_c_disconnect_btn->setEnabled(false);
    connect(m_TcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(tcp_c_SocketState_Changed(QAbstractSocket::SocketState)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*************** TCP服务器 ***************/

// 监听开始 按钮触发
void MainWindow::on_tcp_s_listen_btn_clicked()
{
    QHostAddress ip_add(ui->tcp_s_ip_cbox->currentText());
    unsigned short port = ui->tcp_s_port_ledit->text().toUShort();

    // 服务器开始监听
    if(m_TcpServer->listen(ip_add, port)){
        ui->tcp_s_listen_btn->setEnabled(false);
        ui->tcp_s_close_btn->setEnabled(true);

    }else{

    }
}

// 监听停止 按钮触发
void MainWindow::on_tcp_s_close_btn_clicked()
{
    ui->tcp_s_listen_btn->setEnabled(true);
    ui->tcp_s_close_btn->setEnabled(false);

    m_Tcp_ClientList->clear();          // 清理所有的存储套接字
    m_TcpServer->close();
}

// 服务器发送数据按钮
void MainWindow::on_tcp_s_send_btn_clicked()
{

}

// 有新的连接通知
void MainWindow::tcp_s_newConnection_trigger()
{
    bool socket_added=false;
    QTcpSocket* temp_socket=m_TcpServer->nextPendingConnection();
    qDebug()<<"server socket localAddress"<<temp_socket->localAddress();
    qDebug()<<"server socket peerAddress"<<temp_socket->peerAddress();

    foreach(auto Client,*m_Tcp_ClientList.data()){
        if(Client->peerAddress()==temp_socket->peerAddress()){
            socket_added=true;
            break;
        }
    }
    if(!socket_added)
        m_Tcp_ClientList->append(temp_socket);

    ui->tcp_server_listWidget->
            addItem(QString("%1已连接服务器").arg(temp_socket->peerAddress().toString()));

    connect(temp_socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(tcp_s_SocketState_Changed(QAbstractSocket::SocketState)));
    connect(temp_socket,SIGNAL(readyRead()),this,SLOT(tcp_s_read()));
}

// 服务器读取客户端数据
void MainWindow::tcp_s_read()
{
    //获得信号的发送者的套接字
    QTcpSocket *msocket = dynamic_cast<QTcpSocket*>(sender());
    //读取数据
    QString msg = msocket->readAll();
    //还可以读取对方的ip
    QString ip =  msocket->peerAddress().toString();
    //去掉前面的#ffff
    ip.remove("::ffff:");
    //调用文本浏览框，显示内容
    ui->tcp_server_listWidget->addItem(ip+":"+msg);
}

// TCP 服务器 错误信息提取
void MainWindow::tcp_s_acceptError(QAbstractSocket::SocketError socketError)
{
    QString str=QString("Server NUM:%1").arg(socketError);
    ui->tcp_server_listWidget->addItem(str);
}

// 更新 已经连接的 客户端信息
void MainWindow::tcp_s_update_ClientInfo()
{
    static int list_len=-1;
    if(list_len!=m_Tcp_ClientList->length()){
        list_len=m_Tcp_ClientList->length();
        ui->tcp_s_select_cbox->clear();
        foreach(auto Client,*m_Tcp_ClientList.data()){
            ui->tcp_s_select_cbox->addItem(Client->peerAddress().toString());
        }
    }
//    qDebug()<<m_TcpServer->isListening();
}

// 套接字 状态变化
void MainWindow::tcp_s_SocketState_Changed(QAbstractSocket::SocketState SocketState)
{
    (void)SocketState;
}


/*************** TCP客户端 ***************/

void MainWindow::on_tcp_c_connect_btn_clicked()
{
    QString ip = ui->tcp_c_ip_ledit->text();
    unsigned short port = ui->tcp_c_port_ledit->text().toUShort();

    // 连接服务器
    m_TcpClient->connectToHost(QHostAddress(ip), port);
}

void MainWindow::on_tcp_c_disconnect_btn_clicked()
{
    ui->tcp_c_connect_btn->setEnabled(true);
    ui->tcp_c_disconnect_btn->setEnabled(false);

    m_TcpClient->disconnectFromHost();
}

void MainWindow::on_tcp_c_send_btn_clicked()
{
    if(m_TcpClient->state()== QAbstractSocket::ConnectedState){
        QString msg = ui->tcp_c_send_textEdit->toPlainText();   // 拿到发送信息框的内容

        // 发送
        m_TcpClient->write(msg.toUtf8());
        // 将发送出去的信息放到历史记录框中
        ui->tcp_client_listWidget->addItem("客户端 say：" + msg);
    }
}

void MainWindow::tcp_c_SocketState_Changed(QAbstractSocket::SocketState SocketState)
{
    switch(SocketState){
    case QAbstractSocket::UnconnectedState:
        ui->tcp_c_connect_btn->setEnabled(true);
        ui->tcp_c_disconnect_btn->setEnabled(false);
        qDebug()<<"tcp_c UnconnectedState";
        break;

    case QAbstractSocket::HostLookupState:
        qDebug()<<"tcp_c HostLookupState";
        break;

    case QAbstractSocket::ConnectingState:
        qDebug()<<"tcp_c ConnectingState";
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<"tcp_c ConnectedState";
        ui->tcp_c_connect_btn->setEnabled(false);
        ui->tcp_c_disconnect_btn->setEnabled(true);
        qDebug()<<"client socket localAddress"<<m_TcpClient->localAddress();
        qDebug()<<"client socket peerAddress"<<m_TcpClient->peerAddress();
        break;

    case QAbstractSocket::BoundState:
        qDebug()<<"tcp_c BoundState";
        break;

    case QAbstractSocket::ListeningState:
        qDebug()<<"tcp_c ListeningState";
        break;

    case QAbstractSocket::ClosingState:
        qDebug()<<"tcp_c ClosingState";
        break;
    }
}

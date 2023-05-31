﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sem = new QSemaphore();
    m_sem->release();
    this->setWindowTitle("http示例");

    TCP_getLocalIP();
    // 更新 服务器状态
    server_close();
    init_http_server_ui();
    init_http_server();

    init_http_client_ui();
    init_http_client();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_sem;
}

// 获取 本地 能够 监听的地址
void MainWindow::TCP_getLocalIP()
{
    QTcpServer TcpServer;
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
                if(TcpServer.listen(ip)){
                    bool add=true;
                    TcpServer.close();
                    // 将优先地址 排在 前面
                    for(auto _ip:FIRST_IP_ARRDESS){
                        if(ip.toString().contains(_ip)){
                            ui->server_ip_cbox->insertItem(0,ip.toString());
                            ui->c_server_ip_cbox->insertItem(0,ip.toString());
                            add=false;
                            break;
                        }
                    }
                    // 不在优先选择地址，放在队列尾部
                    if(add){
                        ui->server_ip_cbox->addItem(ip.toString());
                        ui->c_server_ip_cbox->addItem(ip.toString());
                    }

                }
            }
        }
    }
    // 服务器 添加AnyIPv4(0.0.0.0)
    ui->server_ip_cbox->insertItem(1,
                QHostAddress(QHostAddress::AnyIPv4).toString());
    ui->server_ip_cbox->setCurrentIndex(0);
    ui->c_server_ip_cbox->setCurrentIndex(0);
}

/************ HTTP 服务器 ************/
/** 初始化 **/
// 初始化 HTTP 服务器UI
void MainWindow::init_http_server_ui()
{
    // 设置端口限制
    ui->server_port_sbox->setRange(1,65535);
    ui->server_port_sbox->setValue(8080);
    ui->server_port_sbox->setEnabled(true);

    // 设置http协议控件属性
    ui->request_header_tEdit->setReadOnly(true);
    ui->request_para_lEdit->setReadOnly(true);
    ui->request_method_lEdit->setReadOnly(true);
    ui->request_version_lEdit->setReadOnly(true);
    ui->request_path_lEdit->setReadOnly(true);
    ui->request_ip_lEdit->setReadOnly(true);
    ui->request_body_lEdit->setReadOnly(true);
}

// 初始化服务器
void MainWindow::init_http_server()
{
    // 首次启动HTTP服务器
    m_http_server = new HttpServerHandler(this);
    qRegisterMetaType<StringMap>("StringMap");
    connect(m_http_server,SIGNAL(HttpRequestInfo(const StringMap &)),
            this,SLOT(server_info_update(const StringMap &)),Qt::QueuedConnection);
    // 加载配置文件
    m_listenerSettings = new QSettings(
                ":/QtWebApp/config/webapp.ini",
                QSettings::IniFormat, this);

    // 只获取 配置文件中的 listener 组参数
    m_listenerSettings->beginGroup("listener");
}

/** 状态 **/
// 服务器处于监听状态
void MainWindow::server_listening()
{
    ui->server_listen_btn->setEnabled(false);
    ui->server_close_btn->setEnabled(true);
    ui->server_ip_cbox->setEnabled(false);
    ui->server_port_sbox->setEnabled(false);
}

// 服务器处于监听关闭
void MainWindow::server_close()
{
    ui->server_listen_btn->setEnabled(true);
    ui->server_close_btn->setEnabled(false);
    ui->server_ip_cbox->setEnabled(true);
    ui->server_port_sbox->setEnabled(true);
}

/** 槽函数 **/
// 清除历史槽函数
void MainWindow::on_server_history_clear_clicked()
{
    ui->server_listWidget->clear();
}

// 服务器开始监听
void MainWindow::on_server_listen_btn_clicked()
{
    // 更新 HTTP 服务器的 监听IP和端口
    QString host=ui->server_ip_cbox->currentText();
    QString port=ui->server_port_sbox->text();
    m_listenerSettings->setValue("host",host);
    m_listenerSettings->setValue("port",port);

    // 开启监听
    if(m_HttpListener!=nullptr){
        m_HttpListener->listen();
    }
    else{
        m_HttpListener = new HttpListener(m_listenerSettings,
                                          m_http_server, this);
    }
    if(m_HttpListener->isListening()){
        server_listening();
    }else{
        server_close();
    }
}

// 服务器停止监听
void MainWindow::on_server_close_btn_clicked()
{
    if(m_HttpListener!=nullptr){
        m_HttpListener->close();
        server_close();
    }
}

// 更新请求信息
void MainWindow::server_info_update(const StringMap & info)
{
    ui->request_header_tEdit->setText(info["HeaderMap"]);
    ui->request_para_lEdit->setText(info["Parameter"]);
    ui->request_method_lEdit->setText(info["Method"]);
    ui->request_version_lEdit->setText(info["Version"]);
    ui->request_path_lEdit->setText(info["Path"]);
    ui->request_ip_lEdit->setText(info["PeerAddress"]);
    ui->request_body_lEdit->setText(info["Body"]);
    if(info.contains("message")){
        ui->server_listWidget->addItem(info["message"]);
        ui->server_listWidget->scrollToBottom();
    }
}

/************ HTTP 客户端 ************/
/** 初始化 **/
// 初始化 http 客户端
void MainWindow::init_http_client()
{
    m_http_client = new HttpClient(this);
}

// 初始化 http 客户端 UI
void MainWindow::init_http_client_ui()
{
    // 服务器端口设置
    ui->c_server_port_sbox->setRange(1,65535);
    ui->c_server_port_sbox->setValue(8080);
    ui->c_server_port_sbox->setEnabled(true);

    // 服务器地址 IP 可以编辑
    ui->c_server_ip_cbox->setEditable(true);
    // 正则匹配 表达只能输入 0.0.0.0~255.255.255.255
    QRegExp IP_regExp("^((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))"
                        "(\\.((\\d)|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))){3}$");
    QValidator* pValidator = new QRegExpValidator(IP_regExp, this);
    ui->c_server_ip_cbox->setValidator(pValidator);

    // 设置参数限制
    ui->request_para1->setRange(1,65535);
    ui->request_para1->setValue(100);
    ui->request_para1->setEnabled(true);

    ui->request_para2->setRange(1,65535);
    ui->request_para2->setValue(200);
    ui->request_para2->setEnabled(true);

    // 设置默认路径
    ui->request_path->setText("/home/zonesion");

    // URL 设置为只读
    ui->request_URL->setReadOnly(true);
}

/** 槽函数 **/
// 清理历史
void MainWindow::on_client_history_clear_clicked()
{
    ui->client_listWidget->clear();
}

// 客户端发送请求
void MainWindow::on_client_send_request_clicked()
{
    if(m_sem->tryAcquire()){
        QNetworkRequest request;
        // 定义URL的基本格式
        QString scheme = "http://";
        QString serverAddr = ui->c_server_ip_cbox->currentText();
        QString port = ui->c_server_port_sbox->text();
        QString path = ui->request_path->text();
        QString parameter = QString("/?para1=%1&para2=%2")
                .arg(ui->request_para1->text())
                .arg(ui->request_para2->text());
        QString requestHeader = scheme
                + serverAddr + QString(":") + port
                + path + parameter;

        if (ui->post_rbtn->isChecked())
        {
            // 获取 发送的消息 正文
            QByteArray data=ui->client_send_tEdit->toPlainText().toUtf8();
            // 设置url地址
            request.setUrl(QUrl(requestHeader));
            ui->request_URL->setText(requestHeader);
            // 设置 Content-Type 设置body的数据格式
            request.setHeader(QNetworkRequest::ContentTypeHeader,
                              QVariant("application/json"));
            // 默认为 keep-alive,我们可以使用 建立 一次 连接就关闭
            request.setRawHeader("Connection", "close");

            // 发送 POST 请求
            m_post_reply = m_http_client->m_manager->post(request, data);
            if(m_post_reply!=nullptr){
                m_post_reply->setObjectName("post_reply");
                QObject::connect(m_post_reply, SIGNAL(finished()), this, SLOT(client_request_finish()));
                ui->client_send_request->setEnabled(false);
            }
        }
        else if (ui->get_rbtn->isChecked())
        {
            // 客户端 可以主动 断开 连接
            m_http_client->abort();
            request.setUrl(QUrl(requestHeader));
            // 设置 Content-Type 设置body的数据格式
            request.setHeader(QNetworkRequest::ContentTypeHeader,
                              QVariant("application/json"));

            m_get_reply = m_http_client->m_manager->get(request);
            if(m_get_reply!=nullptr){
                m_get_reply->setObjectName("get_reply");
                QObject::connect(m_get_reply, SIGNAL(finished()), this, SLOT(client_request_finish()));
                ui->client_send_request->setEnabled(false);
            }
        }
    }
}

// 客户端请求完成
void MainWindow::client_request_finish()
{
    QNetworkReply* reply= qobject_cast<QNetworkReply*>(sender());
    if(reply->error()==QNetworkReply::NoError){
        QUrl url=reply->url();
        QByteArray replyData = reply->readAll();
        if(reply->objectName()=="post_reply"){
            /* POST应用反馈 */
            QString msg(replyData);
            QString info=QString("IP:%1--> %2").arg(url.host()).arg(QString(replyData));
            ui->client_listWidget->addItem(info);
            ui->client_listWidget->scrollToBottom();
        }
        else if(reply->objectName()=="get_reply"){
            /* GET应用反馈 */
            QString msg(replyData);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
            if(jsonDoc.isObject()){
                QJsonObject data=jsonDoc.object();
                QJsonValue sum =data.value("sum");
                if(sum.isString()){
                    QString info=QString("IP:%1--> sum:%2").arg(url.host()).arg(sum.toString());
                    ui->client_listWidget->addItem(info);
                    ui->client_listWidget->scrollToBottom();
                }
            }
        }
    }else{
        QString err;
        switch(static_cast<int>(reply->error())){
        case QNetworkReply::ConnectionRefusedError:
            err="连接被拒绝错误";
            break;
        case QNetworkReply::HostNotFoundError:
            err="远程主机关闭错误";
            break;
        case QNetworkReply::TimeoutError:
            err="超时";
            break;
        case QNetworkReply::OperationCanceledError:
            err="操作取消错误";
            break;
        }

        QString err_info=QString("客户端连接错误: %1").arg(err);
        ui->client_listWidget->addItem(err_info);
        ui->client_listWidget->scrollToBottom();
    }

    reply->deleteLater();
    m_sem->release();
    ui->client_send_request->setEnabled(true);
}

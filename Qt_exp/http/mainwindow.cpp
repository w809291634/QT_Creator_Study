#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("http示例");

//    init_http_server();
    init_http_client();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init_http_server()
{
    // 加载配置文件
    QSettings* listenerSettings = new QSettings(
                ":/QtWebApp/config/webapp.ini",
                QSettings::IniFormat, this);

    // 只获取 配置文件中的 listener组参数
    listenerSettings->beginGroup("listener");

    // 启动HTTP服务器
    m_http_server = new HttpServer(this);
    new HttpListener(listenerSettings, m_http_server, this);
}


void MainWindow::init_http_client()
{
    // 设置服务器IP
    ui->textEdit_addr->setText("127.0.0.1");
    ui->textEdit_port->setText("8080");
    m_http_client = new HttpClient(this);
}

void MainWindow::client_request_finish()
{

}

void MainWindow::on_pushButton_clicked()
{
    m_http_client->abort();
    qDebug() << "Connetting....";
    // 这些 重要的变量 可以 移动到 client类里面去

    QNetworkRequest request;
    QString scheme = "http";
    QString serverAddr = ui->textEdit_addr->toPlainText();
    QString port = ui->textEdit_port->toPlainText();
    QString requestHeader = scheme + QString("://") + serverAddr + QString(":") + port;

    if (ui->radioButton->isChecked())
    {
        //获取对应的参数数据
        QByteArray data;
        data.append("a=");
        data.append(ui->textEdit_parm_1->toPlainText().toUtf8());
        data.append("&b=");
        data.append(ui->textEdit_parm_2->toPlainText().toUtf8());

        QNetworkAccessManager manager;
        request.setUrl(QUrl(requestHeader));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

        // 发送 POST 请求
        QNetworkReply *reply = manager.post(request, data);
        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        //对请求的返回异常进行处理
        QByteArray replyData = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error();
        }
        ui->textEdit_res->setText(QString(replyData));
        reply->deleteLater();

    }
    else
    {
        QString fullRequest = requestHeader + QString("/?a=%1&b=%2")
                .arg(ui->textEdit_parm_1->toPlainText().toInt())
                .arg(ui->textEdit_parm_2->toPlainText().toInt());
        request.setUrl(QUrl(fullRequest));
        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.get(request);
        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        //对请求的返回异常进行处理
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error();
        }
        QByteArray replyData = reply->readAll();
        ui->textEdit_res->setText(QString(replyData));
        reply->deleteLater();
    }
}

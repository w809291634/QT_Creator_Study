#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_http_server();
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
    qDebug()<<listenerSettings->allKeys();

    // 启动HTTP服务器
    new HttpListener(listenerSettings, new HttpServer(this), this);
}

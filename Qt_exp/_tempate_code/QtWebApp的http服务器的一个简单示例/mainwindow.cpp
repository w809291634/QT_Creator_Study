#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the configuration file
    QSettings* listenerSettings = new
            QSettings(":/etc/webapp.ini", QSettings::IniFormat, this);
    listenerSettings->beginGroup("listener");
    // Start the HTTP server
    new HttpListener(listenerSettings,
                     new HelloWorldController(this), this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



HelloWorldController::HelloWorldController(QObject* parent)
    : HttpRequestHandler(parent)
{
    // empty
}

HelloWorldController::~HelloWorldController()
{
    // empty
}

void HelloWorldController::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray a = "ddhsi11223123odhsodi";
    (void)request;
    qDebug() << "**************************************";
    qDebug() << a;
    response.write(a);
}

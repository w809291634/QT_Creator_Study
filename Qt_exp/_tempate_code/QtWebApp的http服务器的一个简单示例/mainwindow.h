#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QWebSocket>
#include <QWebSocketServer>
#include "httprequesthandler.h"
#include "httplistener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};



// http 测试代码

using namespace stefanfrings;

class HelloWorldController : public HttpRequestHandler {
    Q_OBJECT
public:
    HelloWorldController(QObject* parent = nullptr);
    virtual ~HelloWorldController();

    virtual void service(HttpRequest& request, HttpResponse& response);
};


#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtCore>
#include <QMainWindow>

#include "httpserver.h"
#include "httpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init_http_server();
    void init_http_client();

private slots:
    // 控件 槽函数
    void on_pushButton_clicked();
    void client_request_finish();

private:
    Ui::MainWindow *ui;
    HttpServer* m_http_server;
    HttpClient* m_http_client;
};





#endif // MAINWINDOW_H

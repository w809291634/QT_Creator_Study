#include "QtHttpForC.h"
#include <QButtonGroup>
#include <QTcpSocket>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "QByteArray"

QtHttpForC::QtHttpForC(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_Buttongroup = new QButtonGroup(this);
    m_Buttongroup->addButton(ui.radioButton, 1);
    m_Buttongroup->addButton(ui.radioButton_2, 2);
    ui.textEdit_addr->setText("127.0.0.1");
    ui.textEdit_port->setText("8080");
    m_Socket = new QTcpSocket();
}

QtHttpForC::~QtHttpForC()
{

}

void QtHttpForC::on_pushButton_clicked()
{
    m_Socket->abort();
    qDebug() << "Connetting....";
    QNetworkRequest request;
    QString scheme = "http";
    QString serverAddr = ui.textEdit_addr->toPlainText();
    QString port = ui.textEdit_port->toPlainText();
    QString requestHeader = scheme + QString("://") + serverAddr + QString(":") + port;

    if (m_Buttongroup->checkedId() == 1)
    {
        //获取对应的参数数据
        QByteArray data;
        data.append("a=");
        data.append(ui.textEdit_parm_1->toPlainText().toUtf8());
        data.append("&b=");
        data.append(ui.textEdit_parm_2->toPlainText().toUtf8());

        QNetworkAccessManager manager;
        request.setUrl(QUrl(requestHeader));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

        QNetworkReply *reply = manager.post(request, data);
        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        //对请求的返回异常进行处理
        QByteArray replyData = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error();
            delete m_Socket;
        }
        ui.textEdit_res->setText(QString(replyData));
        reply->deleteLater();

    }
    else
    {
        QString fullRequest = requestHeader + QString("/?a=%1&b=%2").arg(ui.textEdit_parm_1->toPlainText().toInt()).arg(ui.textEdit_parm_2->toPlainText().toInt());
        request.setUrl(QUrl(fullRequest));
        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.get(request);
        QEventLoop eventLoop;
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        //对请求的返回异常进行处理
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error();
            delete m_Socket;
        }
        QByteArray replyData = reply->readAll();
        ui.textEdit_res->setText(QString(replyData));
        reply->deleteLater();
    }
}

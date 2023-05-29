#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QJsonObject>

class HttpClient : public QTcpSocket
{
    Q_OBJECT
public:
    HttpClient(QObject *parent=nullptr);
    ~HttpClient();
};

#endif // HTTPCLIENT_H

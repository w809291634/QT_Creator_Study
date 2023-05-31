#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QSharedPointer>

class HttpClient : public QTcpSocket
{
    Q_OBJECT
public:
    HttpClient(QObject *parent=nullptr);
    ~HttpClient();

    QNetworkAccessManager* m_manager;
};

#endif // HTTPCLIENT_H

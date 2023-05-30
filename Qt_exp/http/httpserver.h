#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "httprequesthandler.h"
#include "httplistener.h"

#include <QJsonObject>
#include <QJsonDocument>

typedef QMap<QString,QString> StringMap;

using namespace stefanfrings;

class HttpServerHandler : public HttpRequestHandler
{
    Q_OBJECT
public:
    HttpServerHandler(QObject* parent = nullptr);
    ~HttpServerHandler();

    virtual void service(HttpRequest& request,
                         HttpResponse& response);
Q_SIGNALS:
    void HttpRequestInfo(const StringMap& info);

};


#endif // HTTPSERVER_H

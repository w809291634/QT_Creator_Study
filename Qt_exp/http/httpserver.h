#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

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

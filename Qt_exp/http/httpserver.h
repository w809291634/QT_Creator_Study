#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "httprequesthandler.h"
#include "httplistener.h"

using namespace stefanfrings;

class HttpServer : public HttpRequestHandler
{
    Q_OBJECT
public:
    HttpServer(QObject* parent = nullptr);
    ~HttpServer();

    virtual void service(HttpRequest& request,
                         HttpResponse& response);
};


#endif // HTTPSERVER_H

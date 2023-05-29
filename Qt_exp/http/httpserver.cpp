#include "httpserver.h"

//
HttpServer::HttpServer(QObject* parent)
    : HttpRequestHandler(parent)
{

}

HttpServer::~HttpServer()
{

}

void HttpServer::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray a = "123123132";
    (void)request;
    qDebug() << "**************************************";
    qDebug() << a;
    response.write(a);
}

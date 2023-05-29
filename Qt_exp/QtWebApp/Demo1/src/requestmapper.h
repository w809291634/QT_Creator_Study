#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

/**
请求映射器将传入的HTTP请求分派到控制器类
取决于所请求的路径。
*/

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:
    RequestMapper(QObject* parent=0);
    ~RequestMapper();

    /**
      根据URL将传入的HTTP请求分派到不同的控制器。
      @param request 收到的HTTP请求
      @param response 必须用于返回响应
    */
    void service(HttpRequest& request, HttpResponse& response);

};

#endif // REQUESTMAPPER_H

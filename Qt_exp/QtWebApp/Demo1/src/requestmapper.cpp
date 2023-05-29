#include <QCoreApplication>
#include "global.h"
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "controller/dumpcontroller.h"
#include "controller/templatecontroller.h"
#include "controller/formcontroller.h"
#include "controller/fileuploadcontroller.h"
#include "controller/sessioncontroller.h"
#include "controller/logincontroller.h"

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    // 对于以下路径，每个请求都会获得相关控制器的自己的新实例。

    if (path.startsWith("/dump"))
    {        
        DumpController().service(request, response);
    }

    else if (path.startsWith("/template"))
    {
        TemplateController().service(request, response);
    }

    else if (path.startsWith("/form"))
    {
        FormController().service(request, response);
    }

    else if (path.startsWith("/file"))
    {
        FileUploadController().service(request, response);
    }

    else if (path.startsWith("/session"))
    {
        SessionController().service(request, response);
    }

    else if (path.startsWith("/login"))
    {
        LoginController().service(request, response);
    }

    // 所有其他路径都映射到静态文件控制器。
    // 在这种情况下，一个实例用于多个请求。
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // 清除日志缓冲区
    if (logger)
    {
       logger->clear();
    }
}

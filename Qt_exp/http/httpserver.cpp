#include "httpserver.h"

HttpServerHandler::HttpServerHandler(QObject* parent)
    : HttpRequestHandler(parent)
{

}

HttpServerHandler::~HttpServerHandler()
{

}

void HttpServerHandler::service(HttpRequest &request, HttpResponse &response)
{
    /** 用于更新 ui显示 **/
    StringMap info;
    QMultiMap<QByteArray,QByteArray> ParameterMap=request.getParameterMap();
    QString ParameterMap_str;
    QMultiMap<QByteArray,QByteArray> HeaderMap=request.getHeaderMap();
    QString HeaderMap_str;

    // 获取协议头
    QList<QByteArray> keyList = HeaderMap.keys();//存放的就是QMap的key值
    for(int i=0;i<keyList.size();i++)
    {
        HeaderMap_str+= keyList[i]+":"+HeaderMap.value(keyList[i])+";";
    }
    info["HeaderMap"]=HeaderMap_str;
    // 获取参数
    keyList = ParameterMap.keys();//存放的就是QMap的key值
    for(int i=0;i<keyList.size();i++)
    {
        ParameterMap_str+= keyList[i]+":"+ParameterMap.value(keyList[i])+";";
    }
    info["Parameter"]=ParameterMap_str;
    info["Method"]=request.getMethod();
    info["Version"]=request.getVersion();
    info["Path"]=request.getPath();
    info["PeerAddress"]=request.getPeerAddress().toString();
    info["Body"]=request.getBody();
    // 发送信号
    emit HttpRequestInfo(info);

    /** 处理这个请求 **/
    QByteArray a = "123123132";
    response.write(a);
}

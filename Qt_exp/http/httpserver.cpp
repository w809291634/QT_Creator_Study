#include "httpserver.h"
#define SERVER_MESSAGE  "客户端！hello!你好!"

HttpServerHandler::HttpServerHandler(QObject* parent)
    : HttpRequestHandler(parent)
{

}

HttpServerHandler::~HttpServerHandler()
{

}

// 服务器的 HTTP 请求处理函数
void HttpServerHandler::service(HttpRequest &request, HttpResponse &response)
{
    /** 用于更新 ui显示 也可以将 HttpRequestHandler 放在MainWindow类下 **/
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
    qDebug()<<keyList;
    for(int i=0;i<keyList.size();i++)
    {
        ParameterMap_str+= keyList[i]+":"+ParameterMap.value(keyList[i])+";";
    }
    info["Parameter"]=ParameterMap_str;                     // http协议中所有的参数
    info["Method"]=request.getMethod();                     // http协议的方式
    info["Version"]=request.getVersion();                   // http协议版本
    info["Path"]=request.getPath();                         // http协议路径
    info["PeerAddress"]=request.getPeerAddress().toString();// http协议对方地址
    info["Body"]=request.getBody();                         // http协议正文

    /** 处理这个请求 **/
    if(request.getMethod()=="GET"){
        /* GET 请求 计算求和 */
        int para1=ParameterMap.value("para1").toInt();
        int para2=ParameterMap.value("para2").toInt();
        info["message"]=QString("IP:%1--> %2+%3=?")
                                .arg(request.getPeerAddress().toString())
                                .arg(para1)
                                .arg(para2);

        emit HttpRequestInfo(info);                             // 发送这些消息更新UI

        // 封装成 Json字符串 打包发送出去
        QJsonObject obj_root;
        // 添加 求和值
        obj_root.insert("sum",QString("%1").arg(para1+para2));

        QJsonDocument jsonDoc = QJsonDocument(obj_root);
        QString msg=jsonDoc.toJson(QJsonDocument::Compact);

        // 转换为 Utf8 的QByteArray数组
        response.write(msg.toUtf8());
    }else if(request.getMethod()=="POST"){
        /* POST 请求 仅用于显示 */
        info["message"]=QString("IP:%1--> %2")
                .arg(request.getPeerAddress().toString())
                .arg(QString(request.getBody()));

        emit HttpRequestInfo(info);                             // 发送这些消息更新UI
        response.write(QString(SERVER_MESSAGE).toUtf8());
    }
}

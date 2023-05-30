#include "httpclient.h"

HttpClient::HttpClient(QObject *parent)
    :QTcpSocket(parent)
{
    m_manager=new QNetworkAccessManager(this);
}

HttpClient::~HttpClient()
{

}


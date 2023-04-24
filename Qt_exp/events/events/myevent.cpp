#include "myevent.h"
#include <QDebug>

QEvent::Type m_eventType=static_cast<QEvent::Type>(QEvent::registerEventType());
QEvent::Type m_eventType1=static_cast<QEvent::Type>(QEvent::registerEventType());

myevent::myevent():
    QEvent(myEventype())
    {}

myevent::~myevent()
{
    qDebug()<<"myevent exit";
}

QEvent::Type myevent::myEventype()
{
    if (this->myType == QEvent::None)
       this->myType = static_cast<QEvent::Type>(QEvent::registerEventType());//注册事件
    return this->myType;
}

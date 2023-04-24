#include "myevent.h"
#include <QDebug>

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

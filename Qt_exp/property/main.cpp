#include "property.h"
#include <QApplication>
#include <QMetaProperty>
#include <QDebug>

#define LINE "所在行号:" << __LINE__
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Property *p =new Property();

    p->setObjectName("property");

    // 获取当前对象的 元对象
    const QMetaObject *metaobject = p->metaObject();

    // 获取对象的 classInfo 信息
    for (int i = 0; i < metaobject->classInfoCount(); ++i) {
        QMetaClassInfo qMetaclassinfo = metaobject->classInfo(i);   // 获取元对象的 类信息 从Q_CLASSINFO定义获取
        const char *name = qMetaclassinfo.name();
        const char *value = qMetaclassinfo.value();

        qDebug()<< LINE << "name:"<<name<<"value:"<<value;
    }

    // 获取对象的 method 信息
    for (int i = 0; i < metaobject->methodCount(); ++i) {
        QMetaMethod  Qmetamethod = metaobject->method(i);          // 获取元对象的 元方法
        const char *name = Qmetamethod.name();                     // 对象的方法名
        const char *signature = Qmetamethod.methodSignature();

        qDebug()<< LINE << "name:"<<name<<"signature:"<<signature;
    }

    // 获取对象的 property 信息
    for (int i = 0; i < metaobject->propertyCount(); ++i) {
        QMetaProperty metaproperty = metaobject->property(i);       // 获取元对象的元属性
        const char *name = metaproperty.name();                     // 元属性的名称
        QVariant value = p->property(name);                         // 元属性的值

        qDebug()<< LINE << "name:"<<name<<"value:"<<value;
    }

    // 通过 自己的接口 设置 自定义属性值
    p->setData(10);
    qDebug()<< LINE << p->getData();
    qDebug()<< LINE << p->property("data");
    // 通过 元对象的接口 设置 自定义属性值
    p->setProperty("data", 20);
    qDebug()<< LINE << p->getData();
    qDebug()<< LINE << p->property("data");
    // 添加动态属性
    p->setProperty("dynamic", "mydynamic");
    qDebug()<< LINE << p->property("dynamic");

    qDebug()<< LINE << p->property("Version");

    return 0;
}

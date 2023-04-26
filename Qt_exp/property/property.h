#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>

class Property : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int data READ getData WRITE setData NOTIFY dataChangeed)
    Q_CLASSINFO("authoer", "zonesion")
    Q_CLASSINFO("Version", "3.0.0");
    // 元对象 添加 int型data
    // getData() 来读取 deta 的值
    // setData() 来设置 data 的值
    // 当 data 的值发生改变的时候, 会产生 dataChangeed() 信号
public:
    explicit Property(QObject *parent = nullptr);
    int getData();
    void setData(int data);

signals:
    void dataChangeed(int data);

public slots:

private:
    int m_data;
};

#endif // PROPERTY_H

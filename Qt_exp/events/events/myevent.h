#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include <QEvent>

class myevent : public QEvent
{
public:
    myevent();
    ~myevent();
    QEvent::Type myEventype();
private:
    QEvent::Type myType=QEvent::None;
};

#endif // MYEVENT_H

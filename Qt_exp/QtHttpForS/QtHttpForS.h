#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QWidget>
#include "ui_QtHttpForS.h"

QT_BEGIN_NAMESPACE
class QTcpServer;
class QTcpSocket;
QT_END_NAMESPACE

class QtHttpForS : public QWidget
{
    Q_OBJECT

public:
    QtHttpForS(QWidget *parent = Q_NULLPTR);
    ~QtHttpForS();
    int SumNums(QVector<int> nums);

protected slots:
    void newConnectionSlot();
    void errorStringSlot();
    void sendMsg();
private:
    Ui::QtHttpForSClass ui;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
    QString m_SocketInfo;
};

#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QWidget>
#include "ui_QtHttpForC.h"
#include <QTcpServer>

class QtHttpForC : public QWidget
{
    Q_OBJECT

public:
    QtHttpForC(QWidget *parent = Q_NULLPTR);
    ~QtHttpForC();

private slots:
    void on_pushButton_clicked();

private:
    QButtonGroup *m_Buttongroup;
    QTcpSocket *m_Socket;
private:
    Ui::QtHttpForCClass ui;

};

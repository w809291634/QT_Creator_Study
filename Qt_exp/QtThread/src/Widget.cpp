#include "Widget.h"
#include "ui_Widget.h"
#include "ThreadObject.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_obj(nullptr)
  ,m_objThread(nullptr)
  ,m_currentRunLoaclThread(nullptr)
{
    ui->setupUi(this);
    //控件初始化
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->progressBar_heart->setRange(0,100);
    ui->progressBar_heart->setValue(0);

    //
    connect(ui->pushButton_qobjectStart,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2ThreadClicked);
    connect(ui->pushButton_qobjectStart_2,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2Thread2Clicked);
    connect(ui->pushButton_qobjectStop,&QPushButton::clicked
            ,this,&Widget::onButtonObjectThreadStopClicked);
}

Widget::~Widget()
{
    qDebug() << "start destroy widget";

    delete ui;

    if(m_objThread)
    {
        m_objThread->quit();
    }
    m_objThread->wait();
    qDebug() << "end destroy widget";
}


void Widget::progress(int val)
{
    ui->progressBar->setValue(val);
}

void Widget::receiveMessage(const QString &str)
{
    ui->textBrowser->append(str);
}

void Widget::heartTimeOut()
{
    static int s_heartCount = 0;
    ++s_heartCount;
    if(s_heartCount > 100)
    {
        s_heartCount = 0;
    }
    ui->progressBar_heart->setValue(s_heartCount);
}

// QThread QObject方法
// 启动 ObjThread
void Widget::startObjThread()
{
    if(m_objThread)
    {
        return;
    }
    m_objThread= new QThread();
    m_obj = new ThreadObject();
    m_obj->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);
    // 启动线程
    connect(this,&Widget::startObjThreadWork1,m_obj,&ThreadObject::runSomeBigWork1);
    connect(this,&Widget::startObjThreadWork2,m_obj,&ThreadObject::runSomeBigWork2);
    // 返回消息
    connect(m_obj,&ThreadObject::progress,this,&Widget::progress);
    connect(m_obj,&ThreadObject::message,this,&Widget::receiveMessage);

    m_objThread->start();
}

void Widget::onButtonObjectMove2ThreadClicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }

    emit startObjThreadWork1();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 1");
}

void Widget::onButtonObjectMove2Thread2Clicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }
    emit startObjThreadWork2();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 2");
}


void Widget::onButtonObjectThreadStopClicked()
{
    if(m_objThread)
    {
        if(m_obj)
        {
            m_obj->stop();
        }
    }
}

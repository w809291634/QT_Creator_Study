#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);         // 为自身安装事件过滤器

    _myevent=new myevent();
    // 定时器方法一
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(handletimeout()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    qDebug()<<"exit";
//    delete _myevent;
    delete ui;
}

/* 应用程序事件过滤器 */
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    /* 函数头 */
    QString head("[eventFilter]");
    QString mesg=head+QString("事件类型(eventtype):%1").arg(event->type());
    qDebug()<<mesg;

    /* space按键处理 */
    if (object == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            QString mesg=QString("空格按键");
            ui->statusBar->showMessage(head+mesg);
            ui->textEdit->append(head+QString("[%1] %2").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
            return true;                                        // 事件停止传播
        }
    }
    return QWidget::eventFilter(object, event);                 // 回调默认的过滤函数
}

/* 应用程序事件函数 */
bool MainWindow::event(QEvent *event)
{
    QString head("[event]");
    QString mesg=head+QString("事件类型(eventtype):%1").arg(event->type());
    qDebug()<<mesg;

    /* tab按键处理 */
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyevent= static_cast<QKeyEvent *>(event);
        if(keyevent->key() == Qt::Key_Tab){
            QString mesg=QString("[event] tab按键:%1").arg(keyevent->key());
            ui->statusBar->showMessage(mesg);
            ui->textEdit->append(QString("%1 (%2)").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
            return true;                                        //如果返回true事件停止传播
        }
    }

    /* 自定义事件处理 */
//    if (event->type() == MyEvent::myEventype())
//    {
//        MyEvent *myEvent = dynamic_cast<MyEvent*>(event);
//        QString mesg=QString("自定义事件");
//        ui->statusBar->showMessage(mesg);
//        ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
//        return true;
//    }

    return QWidget::event(event);                               // 回调窗口的 事件处理函数
}







// 定时器超时 处理函数
void MainWindow::handletimeout(){



}

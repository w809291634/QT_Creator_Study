#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QDateTime>

QEvent::Type _postEvent=static_cast<QEvent::Type>(QEvent::registerEventType());
QEvent::Type _sendEvent=static_cast<QEvent::Type>(QEvent::registerEventType());

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);         // 为自身安装事件过滤器

    sendEvent.reset(new QEvent(_sendEvent));

    // 定时器方法一
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(handletimeout()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    qDebug()<<"exit";
    delete ui;
}

// 定时器超时 处理函数
void MainWindow::handletimeout(){
    //postevent
    QCoreApplication::postEvent(this, new QEvent(_postEvent));              // postEvent 自动回收 _postEvent

    //sendEvent
    if(!sendEvent.isNull())
        qDebug() <<QCoreApplication::sendEvent(this, sendEvent.data());     // sendEvent 需要手动回收 sendEvent
}

/* 应用程序事件过滤器 事件优先进入 eventFilter 函数*/
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    /* 函数头 */
    QString head("[eventFilter]");
    QString mesg=head+QString("事件类型(eventtype):%1").arg(event->type());
    qDebug()<<QString("%1 (%2)").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));

    /* space按键处理 */
    if (object == this && event->type() == QEvent::KeyPress) {
        if(QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event)){
            if (keyEvent->key() == Qt::Key_Space) {
                QString mesg=QString("空格按键");
                ui->statusBar->showMessage(head+mesg);
                ui->textEdit->append(head+QString("[%1] %2").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
                return true;                                        // 事件停止传播
            }
        }
    }

    /* 自定义事件处理 */
    if (event->type() == _sendEvent)
    {
        QString mesg=QString("自定义事件_sendEvent");
        ui->statusBar->showMessage(mesg);
        ui->textEdit->append(QString("%1 (%2)").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        return true;
    }

    return QWidget::eventFilter(object, event);                 // 回调默认的过滤函数
}

/* 应用程序事件函数 再进入 event 分配到特定的 xxxevent函数*/
bool MainWindow::event(QEvent *event)
{
    QString head("[event]");
    QString mesg=head+QString("事件类型(eventtype):%1").arg(event->type());
    qDebug()<<QString("%1 (%2)").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));

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
    if (event->type() == _postEvent)
    {
        QString mesg=QString("自定义事件_postEvent");
        ui->statusBar->showMessage(mesg);
        ui->textEdit->append(QString("%1 (%2)").arg(mesg).arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        return true;
    }

    return QWidget::event(event);                               // 回调窗口的 事件处理函数
}

/* 鼠标相关事件*/
void MainWindow::mousePressEvent(QMouseEvent * event)
{
    QString mesg;

    if(event->button() == Qt::LeftButton)
        mesg=QString("鼠标左键:(%1,%2)").arg(event->x()).arg(event->y());
    else if(event->button() == Qt::RightButton)
        mesg=QString("鼠标右键:(%1,%2)").arg(event->x()).arg(event->y());
    else if(event->button() == Qt::MiddleButton)
        mesg=QString("鼠标滚轮:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    qDebug() << "Release" << event->x() << ":" << event->y();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString mesg=QString("鼠标双击:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    QString mesg=QString("鼠标移动:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

/* 滚轮相关事件*/
void MainWindow::wheelEvent(QWheelEvent *event)
{
    QString mesg;

    if(event->delta()>0)
        mesg=QString("滚轮上滚");
    else
        mesg=QString("滚轮下滚");

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

/* 键盘相关事件*/
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    QString mesg=QString("键盘按下:(keyvalue,%1)").arg(event->key());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));

}

/* 窗口相关事件*/
void MainWindow::moveEvent(QMoveEvent *event)
{
    event->pos().x();
    event->oldPos().x();
    QString mesg=QString("窗口移动:(pos:%1,%2 old_pos:%3,%4)").arg(event->pos().x()).arg(event->pos().y())
            .arg(event->pos().x()).arg(event->pos().y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    (void)event;
    QString mesg=QString("窗口大小:(%1,%2)").arg(this->width()).arg(this->height());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString dlgTitle="question";
    QString str="关闭窗口";

    QString mesg=QString("关闭窗口事件");
    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));

    int choose=QMessageBox::question(this, dlgTitle, str,QMessageBox::Yes,QMessageBox::No);

    switch (choose) {
    case QMessageBox::Yes:
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    default:
        event->accept();
        break;
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    QString mesg=QString("事件类型:(eventype,%1)").arg(event->type());
    qDebug()<<mesg;

//    if (event->type() == QEvent::KeyPress) {
//        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
//        if (ke->key() == Qt::Key_Tab) {
//            QString mesg=QString("tab按键:(eventype,%1)").arg(event->type());
//            ui->statusBar->showMessage(mesg);
//            ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
//            return true;
//        }
//    }
//    if (event->type() == MyEvent::myEventype())
//    {
//        MyEvent *myEvent = dynamic_cast<MyEvent*>(event);
//        QString mesg=QString("自定义事件");
//        ui->statusBar->showMessage(mesg);
//        ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
//        return true;
//    }

    return QWidget::event(event);
}

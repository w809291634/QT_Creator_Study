#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initInterface();
    this->inttControl();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//
void MainWindow::initInterface()
{

    ui->btn_2->setCheckable(true);
    ui->btn_2->autoExclusive();

    ui->tbtn_2->setCheckable(true);

    QStringList sizelist;
    sizelist<< "12"<<"14"<<"16"<<"18"<<"20";
    ui->comboBox->addItems(sizelist);

    ui->spinBox_R->setRange(0,255);
    ui->spinBox_G->setRange(0,255);
    ui->doubleSpinBox_B->setRange(0,255);
    ui->spinBox_R->setValue(0);
    ui->spinBox_G->setValue(0);
    ui->doubleSpinBox_B->setValue(0);
}

//
void MainWindow::inttControl()
{
    /* 设置主窗口 action 控件的触发槽函数 */
    QList<QAction *> actlist =  this->findChildren<QAction *>(QString(),  Qt::FindDirectChildrenOnly);
    foreach(auto act, actlist)
    {
//        qDebug()<<act->objectName();                                      // 显示查找的对象的名称
        connect(act,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    }

    /* 设置 按钮 界面中 各控件触发的槽函数 */
//    QList<QPushButton*> btnlist = ui->stackedWidget->findChildren<QPushButton*>();
//    foreach(auto btn, btnlist)
//    {
//        connect(btn,SIGNAL(clicked()),this,SLOT(pushbutton_clicked()));
//    }

//    QList<QToolButton*> tbtnlist = ui->stackedWidget->findChildren<QToolButton*>();
//    foreach(auto tbtn, tbtnlist)
//    {
//        connect(tbtn,SIGNAL(clicked()),this,SLOT(toolbutton_clicked()));
//    }

//    QList<QRadioButton*> rbtnlist = ui->stackedWidget->findChildren<QRadioButton*>();
//    foreach(auto rbtn, rbtnlist)
//    {
//        connect(rbtn,SIGNAL(clicked()),this,SLOT(radiobutton_clicked()));
//    }

//    QList<QCheckBox*> cboxlist = ui->stackedWidget->findChildren<QCheckBox*>();
//    foreach(auto cbox, cboxlist)
//    {
//        connect(cbox,SIGNAL(stateChanged(const int)),this,SLOT(checkbox_stateChange(const int)));
//    }
}

void MainWindow::on_setCurrentIndex()
{
    QAction* action= qobject_cast<QAction*>(sender());

    if(action->objectName()=="action_btn")
        ui->stackedWidget->setCurrentIndex(0);
    else if(action->objectName()=="action_text")
        ui->stackedWidget->setCurrentIndex(1);
    else if(action->objectName()=="action_bar")
        ui->stackedWidget->setCurrentIndex(2);
    else if(action->objectName()=="action_datetime")
        ui->stackedWidget->setCurrentIndex(3);
    else if(action->objectName()=="action_etc")
        ui->stackedWidget->setCurrentIndex(4);
    else if(action->objectName()=="action_about")
    {
        QString dlgTitle="about Qt";
        QMessageBox::aboutQt(this, dlgTitle);
    }
    else if(action->objectName()=="action_exit")
        exit(0);

    ui->statusBar->showMessage(QString("切换到:%1页").arg(action->text()));
}


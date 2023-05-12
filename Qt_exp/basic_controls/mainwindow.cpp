#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QMovie>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init_control_attribute();                             //设置控件属性
    this->init_control_communication();                         //设置控件通信

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(handletimeout()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 初始化控件属性 */
void MainWindow::init_control_attribute()
{
    /* 按钮页面控件属性设置 */
    // pushbutton和toolbutton激活切换模式
    ui->btn_2->setCheckable(true);
    // 设置按钮3的样式表
    ui->btn_3->setStyleSheet("QPushButton{border: 0px solid;background-color:#F65345;color:#FFFFFF;border-radius: 15px;}\
                              QPushButton:disabled{background-color:#DCDCDC;border-radius:15px;color:#FFFFFF;}\
                              QPushButton:hover{border: 0px solid;background-color:#f67469;border-radius:15px;color:#FFFFFF;opacity:0.2;}\
                              QPushButton:pressed{background-color:#c44237;border-radius:15px;color:#FFFFFF;}");
    // 设置工具按钮2、3、4的样式表
    ui->tbtn_2->setStyleSheet("QToolButton{border: 0px solid;background-color:#F65345;color:#FFFFFF;}\
                               QToolButton:disabled{background-color:#DCDCDC;color:#FFFFFF;}\
                               QToolButton:hover{border: 0px solid;background-color:#f67469;color:#FFFFFF;opacity:0.2;}\
                               QToolButton:checked{background-color:#c44237;color:#FFFFFF;}");
    ui->tbtn_2->setCheckable(true);
    ui->tbtn_3->setIcon(QIcon(":/icon/icon/物联网.png"));
    ui->tbtn_3->setIconSize(QSize(45,45));

//    ("QToolButton{border-image: url(:/icon/icon/学士帽.png)} \
//                               QToolButton:pressed{border-image: url(:/icon/icon/实验室.png)}");

    QString path1=(":/icon/icon/学士帽.png");
    QString path2=(":/icon/icon/实验室.png");
    QString styleSheet=QString( "QToolButton{border-image:url(%1);background-color:transparent;border:none;}"
                                "QToolButton:pressed{border-image:url(%2);background-color:transparent;border:none;}")
                                .arg(path1).arg(path2);
    ui->tbtn_4->setStyleSheet(styleSheet);
    // comboBox添加字号
    QStringList sizelist;
    sizelist<< "12"<<"14"<<"16"<<"18"<<"20";
    ui->comboBox->addItems(sizelist);

    // spinBox设置范围和基础值
    ui->spinBox_R->setRange(0,255);
    ui->spinBox_G->setRange(0,255);
    ui->doubleSpinBox_B->setRange(0,255);
    ui->spinBox_R->setSingleStep(10);                       //上下按钮单击时的步长
    ui->spinBox_G->setSingleStep(10);
    ui->doubleSpinBox_B->setSingleStep(10.0);
    ui->spinBox_R->setValue(0);
    ui->spinBox_G->setValue(0);
    ui->doubleSpinBox_B->setValue(0);

    // checkbox1和2启动排他属性
    ui->cBox_1->setAutoExclusive(true);
    ui->cBox_2->setAutoExclusive(true);

    /* 文本控件属性设置 */
    // QLabel属性设置
    ui->lab_1->setText("123");
    // 设置QLabel2显示图片
    QPixmap pix(":/img/img/user.png");
    ui->lab_2->setPixmap(pix);
    ui->lab_2->setScaledContents(true);
    ui->lab_2->setMaximumSize(50,50);
    // 设置QLabel3显示动画
    QMovie *movie = new QMovie(":/img/img/loading.gif");
    movie->setScaledSize(ui->lab_2->size());
    movie->start();
    ui->lab_3->setMovie(movie);
    ui->lab_3->setScaledContents(true);
    ui->lab_3->setMaximumSize(50,50);
    // 设置QLabel4显示文本
    ui->lab_4->setText("<p style=\"color:red;font-size:16px;\"> hello <b style=\"color:black;font-size:22px;\">QLabel</b></p>");

    // lineEdit属性设置
    ui->lineEdit_1->setText("只读");
    ui->lineEdit_1->setReadOnly(true);                  //设置为只读
    ui->lineEdit_2->setPlaceholderText("默认显示");
    QRegExp regx("^[1-9][0-9]*");                       // 正则表达式，只允许输入正数
//    QRegExp regx("^[1-9][0-9]*12$");                    // 正则表达式，只允许输入正数，同时必须是12这两个数字为末尾
    ui->lineEdit_3->setValidator(new QRegExpValidator(regx, this)); //设置正则表达式
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
}

/* 初始化主窗口 控件的信号处理函数 */
void MainWindow::init_control_communication()
{
    /* 设置主窗口 action 控件的触发槽函数 */
    QList<QAction *> actlist =  this->findChildren<QAction *>(QString(),  Qt::FindDirectChildrenOnly);
    foreach(auto act, actlist)
    {
        //qDebug()<<act->objectName();                                      // 显示查找的对象的名称
        connect(act,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    }

    /* 设置 按钮 界面中 各控件触发的槽函数 */
    QList<QPushButton*> btnlist = ui->stackedWidget->findChildren<QPushButton*>();
    foreach(auto btn, btnlist)
    {
        connect(btn,SIGNAL(clicked()),this,SLOT(pushbutton_clicked()));
    }
    QList<QToolButton*> tbtnlist = ui->stackedWidget->findChildren<QToolButton*>();
    foreach(auto tbtn, tbtnlist)
    {
        connect(tbtn,SIGNAL(clicked()),this,SLOT(toolbutton_clicked()));
    }
    QList<QRadioButton*> rbtnlist = ui->stackedWidget->findChildren<QRadioButton*>();
    foreach(auto rbtn, rbtnlist)
    {
        connect(rbtn,SIGNAL(clicked()),this,SLOT(radiobutton_clicked()));
    }
    QList<QCheckBox*> cboxlist = ui->stackedWidget->findChildren<QCheckBox*>();
    foreach(auto cbox, cboxlist)
    {
        connect(cbox,SIGNAL(stateChanged(const int)),this,SLOT(checkbox_stateChange(const int)));
    }
    /* 设置 文本 界面中 各控件触发的槽函数 */
    QList<QLineEdit*> linelist = ui->stackedWidget->findChildren<QLineEdit*>();
    foreach(auto line, linelist)
    {
        connect(line,SIGNAL(textEdited(const QString)),this,SLOT(lineedit_textEdited(const QString)));
        connect(line,SIGNAL(returnPressed()),this,SLOT(lineedit_returnPressed()));
    }
}

/* 主窗口 页面索引处理回调函数 */
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

/* 按钮页面槽函数 */
// pushbutton槽函数
void MainWindow::pushbutton_clicked()
{
    QPushButton* btn= qobject_cast<QPushButton*>(sender());
    if(btn->objectName()=="btn_2")
    {
        if(btn->isChecked())
            ui->statusBar->showMessage(QString("%1按下").arg(btn->objectName()));
        else
            ui->statusBar->showMessage(QString("%1弹起").arg(btn->objectName()));
        return;
    }
    ui->statusBar->showMessage(QString("%1被点击").arg(btn->objectName()));
}
// toolbutton槽函数
void MainWindow::toolbutton_clicked()
{
    QToolButton* tbtn= qobject_cast<QToolButton*>(sender());
    if(tbtn->objectName()=="tbtn_2")
    {
        if(tbtn->isChecked())
            ui->statusBar->showMessage(QString("%1按下").arg(tbtn->objectName()));
        else
            ui->statusBar->showMessage(QString("%1弹起").arg(tbtn->objectName()));
        return;
    }
    ui->statusBar->showMessage(QString("%1被点击").arg(tbtn->objectName()));
}
// radiobutton槽函数
void MainWindow::radiobutton_clicked()
{
    QRadioButton* rbtn= qobject_cast<QRadioButton*>(sender());
    ui->statusBar->showMessage(QString("%1被选中").arg(rbtn->objectName()));
}
// checkbox槽函数
void MainWindow::checkbox_stateChange(const int state)
{
    QCheckBox* cbox= qobject_cast<QCheckBox*>(sender());
    if(state)
        ui->statusBar->showMessage(QString("%1被选中").arg(cbox->objectName()));
    else
        ui->statusBar->showMessage(QString("%1未选中").arg(cbox->objectName()));
}
// fontComboBox槽函数
void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    qApp->setFont(f);
    ui->statusBar->showMessage(QString("当前字体设置为:%1").arg(f.family()));
}
// comboBox槽函数
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QFont font = qApp->font();                          // 获取当前APP的字体
    font.setPointSize(arg1.toInt());                    // 修改字号
    qApp->setFont(font);                                // 设置字体
    this->update();
    ui->statusBar->showMessage(QString("当前字号:%1").arg(arg1));
}
// spinBox槽函数
#define  SPINBOX_SET_COLOR  {ui->gBox_Box->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));\
                            ui->statusBar->showMessage(QString("字体颜色:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));}
void MainWindow::on_spinBox_R_valueChanged(int arg1)
{
    (void)arg1;
    SPINBOX_SET_COLOR;
}
void MainWindow::on_spinBox_G_valueChanged(int arg1)
{
    (void)arg1;
    SPINBOX_SET_COLOR;
}
void MainWindow::on_doubleSpinBox_B_valueChanged(double arg1)
{
    (void)arg1;
    SPINBOX_SET_COLOR;
}

/* 文本页面槽函数 */
// lineedit文本变化触发
void MainWindow::lineedit_textEdited(const QString str)
{
    //QLineEdit* line= qobject_cast<QLineEdit*>(sender());
    ui->textBrowser->clear();
    ui->textBrowser->append(str);
}
// lineedit输入完成后回车触发槽函数
void MainWindow::lineedit_returnPressed()
{
    QLineEdit* line= qobject_cast<QLineEdit*>(sender());
    QString str= line->text();
    if(str.isEmpty())
        return;
    line->clear();
    ui->plainTextEdit->appendPlainText(">>> "+ str);
}

/* 时间日期页面槽函数 */
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->dateEdit->setDate(date);
    ui->statusBar->showMessage(QString("当前日期:%1").arg(date.toString("yyyy-MM-dd")));
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    ui->calendarWidget->setSelectedDate(date);
    ui->statusBar->showMessage(QString("当前日期:%1").arg(date.toString("yyyy-MM-dd")));
}

void MainWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
    ui->statusBar->showMessage(QString("当前时间:%1").arg(time.toString("hh:mm:ss")));
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->statusBar->showMessage(QString("当前时间日期:%1").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss")));
}

/* 文本页面槽函数 */
void MainWindow::handletimeout()
{
    static int curr=0;
    ui->horizontalScrollBar->setValue(curr);
    ui->verticalScrollBar->setValue(curr);
    if(curr++ > ui->horizontalScrollBar->maximum())
        curr=0;
}

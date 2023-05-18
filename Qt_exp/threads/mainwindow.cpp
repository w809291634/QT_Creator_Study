#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"

/* 变量声明 */
// 信号量
MainWindow* _main_win;
static MyThread* QSemaphore_thread1;
static MyThread* QSemaphore_thread2;
static QSemaphore* g_sem;

// 互斥量
static MyThread* QMutex_thread1;
static MyThread* QMutex_thread2;
static MyThread* QMutex_thread3;
static QMutex* g_mut;
static QMutex* g_mut2;
static const uint32_t g_accum_times=100000;

// 等待条件
static MyThread* QWait_thread1;
static MyThread* QWait_thread2;
static QMutex* g_qwait_mut;
static QWaitCondition* g_qwait;
static int8_t g_QWait_data;             // 当前 产量可用 数量
static uint8_t g_thread1_sleep=1;       // 生产者的休息间隔
static uint8_t g_thread2_sleep=1;       // 消耗者的休息间隔
#define MAX_MARGIN      20
#define MAX_SLEEP       10

/* 函数声明 */
// 信号量
static void QSemaphore_thread1_cb(void);
static void QSemaphore_thread2_cb(void);
// 互斥量
static void QMutex_thread1_cb(void);
static void QMutex_thread2_cb(void);
static void QMutex_thread3_cb(void);
// 等待条件
static void QWait_thread1_cb(void);
static void QWait_thread2_cb(void);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->thread()->setObjectName("mainwindow");

    /* 信号量 例子 */
    ui->QSemaphore_bar1->setValue(0);
    ui->QSemaphore_bar2->setValue(0);
    ui->QSemaphore_bar1->setRange(0,10);
    ui->QSemaphore_bar2->setRange(0,10);

    /* 互斥量 例子 */
    ui->mut_accum_times->setText(QString("%1").arg(g_accum_times));

    /* 等待条件 例子 */
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,MAX_MARGIN);
    ui->pro_speed->setText(QString("%1%").arg(uint8_t((double(MAX_SLEEP)-g_thread1_sleep)/MAX_SLEEP*100)));
    ui->consu_speed->setText(QString("%1%").arg(uint8_t((double(MAX_SLEEP)-g_thread2_sleep)/MAX_SLEEP*100)));
    QList<QPushButton*> btn_list = ui->groupBox_3->findChildren<QPushButton*>();
    foreach(auto btn, btn_list)
    {
        for(auto name:{"consume_add","produce_add","produce_sub","consume_sub"}){
            if(btn->objectName()==name){
                qDebug()<<btn->objectName();
                connect(btn,SIGNAL(clicked()),this,SLOT(QWaitCondition_btn_clicked()));
            }
        }
    }
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete g_sem;
    delete g_mut;
    delete g_mut2;
    delete g_qwait_mut;
}

// 主窗口关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    (void)event;
#define THREAD_EXIT(THREAD) {THREAD->requestInterruption();while(THREAD->isRunning())this->thread()->msleep(1);}
    THREAD_EXIT(QSemaphore_thread1);
    THREAD_EXIT(QSemaphore_thread2);

    THREAD_EXIT(QMutex_thread1);
    THREAD_EXIT(QMutex_thread2);
    QMutex_thread3->terminate();

    QWait_thread1->terminate();
    QWait_thread2->terminate();
}

// 主示例程序 初始化
void app_main(MainWindow* instance)
{
#define INIT_THREAD(THREAD,PARENT,NAME,CALLBACK,PRIORITY)   do{ THREAD=new MyThread(PARENT);\
                                                    THREAD->setObjectName(QStringLiteral(NAME));\
                                                    THREAD->set_callback(CALLBACK);\
                                                    THREAD->start(PRIORITY);\
                                                    }while(0);
    _main_win=instance;
    /* 信号量 */
    g_sem=new QSemaphore();
    INIT_THREAD(QSemaphore_thread1,_main_win,"QSemaphore_thread1",QSemaphore_thread1_cb,QThread::LowestPriority);
    INIT_THREAD(QSemaphore_thread2,_main_win,"QSemaphore_thread2",QSemaphore_thread2_cb,QThread::LowestPriority);
    // 设置 信号 连接
    QObject::connect(QSemaphore_thread1,SIGNAL(MsgSignal(const QString&)),_main_win,SLOT(update_QSemaphore_bar(const QString&)));
    QObject::connect(QSemaphore_thread2,SIGNAL(MsgSignal(const QString&)),_main_win,SLOT(update_QSemaphore_bar(const QString&)));

    /* 互斥量 */
    g_mut =new QMutex();
    g_mut2 =new QMutex();
    INIT_THREAD(QMutex_thread1,_main_win,"QMutex_thread1",QMutex_thread1_cb,QThread::InheritPriority);
    INIT_THREAD(QMutex_thread2,_main_win,"QMutex_thread2",QMutex_thread2_cb,QThread::InheritPriority);
    INIT_THREAD(QMutex_thread3,_main_win,"QMutex_thread3",QMutex_thread3_cb,QThread::IdlePriority);
    // 设置 信号 连接
    QObject::connect(QMutex_thread3,
                     SIGNAL(MsgSignal(const QString&,const QString&,const QString&)),
                     _main_win,
                     SLOT(update_QMutex_data(const QString&,const QString&,const QString&)));

    /* QWaitCondition */
    g_qwait_mut=new QMutex();
    g_qwait=new QWaitCondition();
    INIT_THREAD(QWait_thread1,_main_win,"QWait_thread1",QWait_thread1_cb,QThread::InheritPriority);
    INIT_THREAD(QWait_thread2,_main_win,"QWait_thread2",QWait_thread2_cb,QThread::InheritPriority);
}

/* 信号量 线程使用示例 */
// 信号量ui控制
void MainWindow::on_QSemaphore_btn_clicked(bool checked)
{
    (void)(checked);
    g_sem->release();
    ui->QSemaphore_residue->setText(QString("%1").arg(g_sem->available()));
}

// 信号量ui更新
void MainWindow::update_QSemaphore_bar(const QString& msg)
{
    MyThread* p = qobject_cast<MyThread*>(sender());
    if(p == QSemaphore_thread1){
        int value=ui->QSemaphore_bar1->value()+1;
        if(value>ui->QSemaphore_bar1->maximum()){
            ui->QSemaphore_bar1->setValue(0);
        }else
            ui->QSemaphore_bar1->setValue(value);
    }
    else if(p ==QSemaphore_thread2){
        int value=ui->QSemaphore_bar2->value()+1;
        if(value>ui->QSemaphore_bar2->maximum()){
            ui->QSemaphore_bar2->setValue(0);
        }else
            ui->QSemaphore_bar2->setValue(value);
    }
    ui->QSemaphore_residue->setText(QString("%1").arg(g_sem->available()));
    qDebug()<< QString("%1 emit msg:'%2'").arg(p->objectName()).arg(msg);
}
static void QSemaphore_thread1_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        if(g_sem->tryAcquire()){
            emit QSemaphore_thread1->MsgSignal("QSemaphore_thread1 emit");
            QSemaphore_thread1->sleep(1);
        }
    }
}

static void QSemaphore_thread2_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        if(g_sem->tryAcquire()){
            emit QSemaphore_thread2->MsgSignal("QSemaphore_thread2 emit");
            QSemaphore_thread2->sleep(1);
        }
    }
}

/* 互斥量 线程使用示例 */
static uint32_t g_data;
static uint32_t g_cnt1;
static uint32_t g_cnt2;
static uint8_t g_enable_Mutex;
static uint8_t g_Mutex_start_test=0;

void MainWindow::update_QMutex_data(const QString& act,
                                    const QString& cnt1,
                                    const QString& cnt2)
{
    ui->mut_act_times->setText(act);
    ui->mut_thread1_cnts->setText(cnt1);
    ui->mut_thread2_cnts->setText(cnt2);
    ui->QMutex_cbox->setEnabled(true);
}
void MainWindow::on_QMutex_run_btn_clicked()
{
    g_mut2->lock();
    g_Mutex_start_test=1;
    g_mut2->unlock();
    ui->QMutex_cbox->setEnabled(false);
}

void MainWindow::on_QMutex_cbox_clicked(bool checked)
{
    if(checked)g_enable_Mutex=1;
    else g_enable_Mutex=0;
}

// 互斥量例程 累加线程1
static void QMutex_thread1_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        if(g_Mutex_start_test){
            if(g_enable_Mutex)g_mut->lock();
            if(g_cnt1 < g_accum_times){
                g_data++;g_cnt1++;
            }
            if(g_enable_Mutex)g_mut->unlock();
        }
        else {
            QMutex_thread1->msleep(10);
        }
    }
}

// 互斥量例程 累加线程2
static void QMutex_thread2_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        if(g_Mutex_start_test){
            if(g_enable_Mutex)g_mut->lock();
            if(g_cnt2 < g_accum_times){
                g_data++;g_cnt2++;
            }
            if(g_enable_Mutex)g_mut->unlock();
        }
        else {
            QMutex_thread2->msleep(10);
        }
    }
}

// 互斥量例程 管理线程
static void QMutex_thread3_cb(void)
{
    uint32_t times=0;
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        if(g_Mutex_start_test){
            times=0;
            if(g_cnt1>= g_accum_times && g_cnt2>= g_accum_times)
            {
                emit QMutex_thread3->MsgSignal(QString("%1").arg(g_data),
                                               QString("%1").arg(g_cnt1),
                                               QString("%1").arg(g_cnt2));
                g_mut2->lock();
                g_Mutex_start_test=0;
                g_mut2->unlock();
            }
        }else {
            // 复位值
            g_cnt1=0;g_cnt2=0;g_data=0;
            if(times++>100){
                emit QMutex_thread3->MsgSignal(QString("%1").arg(g_data),
                                               QString("%1").arg(g_cnt1),
                                               QString("%1").arg(g_cnt2));
                times=0;
            }
            QMutex_thread3->msleep(10);
        }
    }
}

/* QWaitCondition 线程使用示例 */

void MainWindow::QWaitCondition_btn_clicked()
{
    QPushButton* btn= qobject_cast<QPushButton*>(sender());
    if(btn->objectName()=="produce_add")
        g_thread1_sleep--;
    else if(btn->objectName()=="produce_sub")
        g_thread1_sleep++;
    else if(btn->objectName()=="consume_add")
        g_thread2_sleep--;
    else if(btn->objectName()=="consume_sub")
        g_thread2_sleep++;

    if(g_thread1_sleep>MAX_SLEEP)g_thread1_sleep=MAX_SLEEP;
    if(g_thread1_sleep<1)g_thread1_sleep=1;
    ui->pro_speed->setText(QString("%1%").arg(uint8_t((double(MAX_SLEEP)-g_thread1_sleep)/MAX_SLEEP*100)));

    if(g_thread2_sleep>MAX_SLEEP)g_thread2_sleep=MAX_SLEEP;
    if(g_thread2_sleep<1)g_thread2_sleep=1;
    ui->consu_speed->setText(QString("%1%").arg(uint8_t((double(MAX_SLEEP)-g_thread2_sleep)/MAX_SLEEP*100)));
}

void MainWindow::handleTimeout()
{
    ui->progressBar->setValue(g_QWait_data);
}

// 生成者
static void QWait_thread1_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        g_qwait_mut->lock();
        if(g_QWait_data >= MAX_MARGIN)
            g_qwait->wait(g_qwait_mut);
        else{
            g_QWait_data++;
        }
        g_qwait_mut->unlock();
        g_qwait->wakeAll();
        QWait_thread1->msleep(g_thread1_sleep);
    }
}

// 消费者
static void QWait_thread2_cb(void)
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        g_qwait_mut->lock();
        if( g_QWait_data <= 0)
            g_qwait->wait(g_qwait_mut);
        else
            g_QWait_data--;
        g_qwait_mut->unlock();
        g_qwait->wakeAll();
        QWait_thread2->msleep(g_thread2_sleep);
    }
}


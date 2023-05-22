#include "mainwindow.h"
#include "ui_mainwindow.h"

// 只创建一个主窗口，所有初始化在构造函数内
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Serial.reset(new QSerialPort());
    // 设置 串口 读数据 的处理函数
    connect(Serial.data(),SIGNAL(readyRead()),SLOT(serial_read()));
    init_ui();

    // 首次刷新
    get_com();
    get_time();
    serial_ui_update();

    // 输入限制
    QRegExp Regexp_Pass,Regexp_Sys,Regexp_Num,Regexp_ID_Num;
    Regexp_Pass.setPattern("[0-9a-fA-F]{1,12}");
    Regexp_Sys.setPattern("[0-9a-fA-F]{1,12}");
    Regexp_Num.setPattern("[0-9a-fA-F]{1,12}");
    Regexp_ID_Num.setPattern("[0-9a-fA-F]{1,10}");                  // 10位的字符ID

    ui->ID_Write_lineEdit->setValidator(new QRegExpValidator(Regexp_ID_Num,ui->ID_Write_lineEdit));

    /* 定时器 */
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_com()));          // 每100ms读取一次串口
    timer->start(100);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_time()));         // 每1s读取一次当前时间
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化 界面 UI
void MainWindow::init_ui()
{
    ui->stackedWidget->setCurrentIndex(0);
    /* 初始化 串口 UI */
    // comboBox添加字号
    QStringList Seriallist;
    Seriallist<< "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->baud_cbbox->addItems(Seriallist);
    ui->baud_cbbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    /* 初始化 调试界面 UI */
    show_debug = true;
    ui->show_debug_btn->setText("》\n》\n》");
    ui->debug_data->show();

    /* 初始化 ID卡 页面 UI */

}

// 初始化串口
bool MainWindow::init_serial()
{
    //获取当前选择的串口号和波特率
    QString _Com = ui->com_cbbox->currentText();
    QString _Baud = ui->baud_cbbox->currentText();

    //按选择的设置串口,数据位,停止位,校验位,硬件流控已默认设置
    Serial->setPortName(_Com);
    Serial->setDataBits(QSerialPort::Data8);
    Serial->setStopBits(QSerialPort::OneStop);
    Serial->setParity(QSerialPort::NoParity);
    Serial->setFlowControl(QSerialPort::NoFlowControl);

    //按选择的波特率设置串口波特率
    if(_Baud == "1200") Serial->setBaudRate(QSerialPort::Baud1200);
    else if(_Baud == "2400") Serial->setBaudRate(QSerialPort::Baud2400);
    else if(_Baud == "4800") Serial->setBaudRate(QSerialPort::Baud4800);
    else if(_Baud == "9600") Serial->setBaudRate(QSerialPort::Baud9600);
    else if(_Baud == "19200") Serial->setBaudRate(QSerialPort::Baud19200);
    else if(_Baud == "38400") Serial->setBaudRate(QSerialPort::Baud38400);
    else if(_Baud == "57600") Serial->setBaudRate(QSerialPort::Baud57600);
    else Serial->setBaudRate(QSerialPort::Baud115200);

    return Serial->open(QSerialPort::ReadWrite);                 //返回是否打开串口并设置为读写模式
}

// 获取时间
void MainWindow::get_time(void)
{
    QTime time = QTime::currentTime();
    QString Time_str = time.toString("hh:mm:ss");
    ui->time_label->setFont(QFont("",35,QFont::Bold));
    ui->time_label->setText(Time_str);
}

// 获取当前电脑可用串口 并进行 状态判断
void MainWindow::get_com(void)
{
    static unsigned char Last_count = 0;
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
    int count = serialPortInfo.count();                     //有效串口的个数
    if(m_Com_Open == false)
    {
        /* 串口关闭 */
        if(count != Last_count)                             //避免出现串口刷新后自动改变
        {
            ui->com_cbbox->clear();                         //清除下拉选择框中的内容
            for(int i=0;i<count;i++){
                // 添加 复选框 的项目
                ui->com_cbbox->addItem(serialPortInfo.at(i).portName());
            }
            ui->baud_cbbox->setCurrentText("115200");       // 如果有115200的 选项，就选择这个为默认。
            Last_count = static_cast<unsigned char>(count);
        }
    }
    else
    {
        /* 串口打开 */
        // 判断 已经打开 的串口 是否正常连接
        for(int i=0; i<count; i++)
        {
            if(m_Com == serialPortInfo.at(i).portName()) return;
        }
        serial_close();
    }
}

// 关闭 串口的 连接
bool MainWindow::serial_close()
{
    Serial->close();                                         //关闭串口连接
    m_Com_Open = false;
    serial_ui_update();

    return 0;
}

// 打开 串口的 连接
// 返回: 0失败 1成功
bool MainWindow::serial_open()
{
    /* 进行打开串口操作 */
    if(init_serial() == true)                                //串口打开成功
    {
        // 保存已经打开 串口的属性
        m_Com = ui->com_cbbox->currentText();
        m_Baud = ui->baud_cbbox->currentText();
        m_Com_Open = true;
        // 设置 UI
        serial_ui_update();
        return 1;
    }
    else
    {
        m_Com="";
        m_Baud="";
        m_Com_Open = false;
        // 设置 UI
        serial_ui_update();
        // 提示 打开失败原因
        QMessageBox::warning(this,"Connection Error",Serial->errorString());
        return 0;
    }
}

// 串口连接状态改变 UI 界面更新
void MainWindow::serial_ui_update()
{
    if(m_Com_Open == false)
    {
        /* 进行关闭串口后的显示 */
        ui->serial_open_btn->setText("打开串口");                  //按键字变化
        ui->com_cbbox->setEnabled(true);
        ui->baud_cbbox->setEnabled(true);
        // ID 页面
        ui->ID_read_btn->setEnabled(false);
        ui->ID_write_btn->setEnabled(false);
    }
    else
    {
        /* 进行打开串口后的显示 */
        ui->serial_open_btn->setText("关闭串口");                   //按键文字变化
        ui->com_cbbox->setEnabled(false);
        ui->baud_cbbox->setEnabled(false);
        // ID 页面
        ui->ID_read_btn->setEnabled(true);
        ui->ID_write_btn->setEnabled(true);
    }
}

// 串口打开 按钮的 槽函数
void MainWindow::on_serial_open_btn_clicked()
{
    if(m_Com_Open == true)
    {
        serial_close();
    }
    else
    {
        serial_open();
    }
}

// 串口 写
void MainWindow::serial_write()
{

}

// 串口 读
int MainWindow::serial_read()
{
    QByteArray RecData_AA;
//    if(Show_Mode != 1  && !IsStop && IsOpen)
//        Rec_Num += Serial.bytesAvailable();
    this->thread()->msleep(30);
    RecData_AA = Serial->readAll();                              // 读取串口的数据
//    RecData = RecData_AA.toHex(' ').toUpper();
//    switch(Mode)
//    {
//    case 1: Uart_DataHandle(5, 1);                              // ID串口数据处理（基础）
//            ID_Analysis();
//        break;
//    case 2: Uart_DataHandle(5, 1);                              // IC串口数据处理（基础）
//            IC_Data_Look();
//        break;
//    }
//    ui->label_Rec_Num->setText(QString::number(Rec_Num));

    return 0;
}

// 显示 调试 界面
void MainWindow::on_show_debug_btn_clicked()
{
    if(show_debug)
    {
        show_debug = false;
        ui->show_debug_btn->setText("《\n《\n《");
        ui->debug_data->hide();
    }
    else
    {
        show_debug = true;
        ui->show_debug_btn->setText("》\n》\n》");
        ui->debug_data->show();
    }
}

// 显示 ID卡 应用界面
void MainWindow::on_IDcard_page_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 显示 IC卡 应用界面
void MainWindow::on_ICcard_page_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::StringToHex(QString str,QByteArray &send_data)
{


}

/* ID卡 应用界面 槽函数 */
void MainWindow::on_ID_read_btn_clicked()
{

}

void MainWindow::on_ID_write_btn_clicked()
{

}

void MainWindow::on_ID_data_clear_btn_clicked()
{

}

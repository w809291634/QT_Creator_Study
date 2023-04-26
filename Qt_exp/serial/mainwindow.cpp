#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Serial.reset(new QSerialPort());
    // 设置串口
    SetUart();
    // 设置串口的接收函数
    connect(Serial.data(),SIGNAL(readyRead()),this,SLOT(ReadUart()));

    // 设置串口的数据发送函数
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(uart_sent()));     //每一秒读取一次当前时间
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Get_Com()
{
//    static unsigned char Last_Com = 0;
//    //智能识别当前系统的有效串口号
//    QList<QSerialPortInfo>serialPortInfo = QSerialPortInfo::availablePorts();
//    int count = serialPortInfo.count();                     //有效串口的个数
//    static unsigned char sum = 0;
//    if(IsOpen == false)
//    {
//        if(count != Last_Com)                                   //避免出现串口刷新后自动改变
//        {
//            ui->comboBox_Com->clear();                          //清除下拉选择框中的内容
//            for(int a=0; a<sum; a++)
//            {
//                com_str[a] = "";
//            }
//            for(int i=0;i<count;i++)
//            {
//                //把有效的串口号添加到comboBox_Com中
//                ui->comboBox_Com->addItem(serialPortInfo.at(i).portName());
//                com_str[i] = serialPortInfo.at(i).portName();
//            }
//            sum = count;
//            Last_Com = count;
//        }
//        else
//        {
//            return;
//        }
//    }
//    else
//    {
//        bool off = false;
//        for(int i=0; i<count; i++)
//        {
//            if(isCom == serialPortInfo.at(i).portName())
//                off = true;
//        }
//        if(off == false)
//        {
//            OpenUart_Clicked();
//        }
//    }
}

bool MainWindow::SetUart()
{
    //获取当前选择的串口号和波特率
//    mCom = ui->comboBox_Com->currentText();
//    mBaud = ui->comboBox_Baud->currentText();

    //按选择的设置串口,数据位,停止位,校验位,硬件流控已默认设置
    Serial->setPortName("COM5");
    Serial->setDataBits(QSerialPort::Data8);
    Serial->setStopBits(QSerialPort::OneStop);
    Serial->setParity(QSerialPort::NoParity);
    Serial->setFlowControl(QSerialPort::NoFlowControl);
    Serial->setBaudRate(QSerialPort::Baud115200);

//    //按选择的波特率设置串口波特率
//    if(mBaud == "1200")
//    {
//        Serial.setBaudRate(QSerialPort::Baud1200);
//    }
//    else if(mBaud == "2400")
//    {
//        Serial.setBaudRate(QSerialPort::Baud2400);
//    }
//    else if(mBaud == "4800")
//    {
//        Serial.setBaudRate(QSerialPort::Baud4800);
//    }
//    else if(mBaud == "9600")
//    {
//        Serial.setBaudRate(QSerialPort::Baud9600);
//    }
//    else if(mBaud == "19200")
//    {
//        Serial.setBaudRate(QSerialPort::Baud19200);
//    }
//    else if(mBaud == "38400")
//    {
//        Serial.setBaudRate(QSerialPort::Baud38400);
//    }
//    else if(mBaud == "57600")
//    {
//        Serial.setBaudRate(QSerialPort::Baud57600);
//    }
//    else
//    {
//        Serial.setBaudRate(QSerialPort::Baud115200);
//    }

    return Serial->open(QSerialPort::ReadWrite);                 //返回是否打开串口并设置为读写模式
}

int MainWindow::ReadUart()
{
    QByteArray RecData_AA;
    QString RecData;
//    if(Show_Mode != 1  && !IsStop && IsOpen)
//        Rec_Num += Serial.bytesAvailable();
//    Sleep(30);
    RecData_AA = Serial->readAll();                              // 读取串口的数据
    RecData = RecData_AA.toHex(' ').toUpper();
    qDebug()<<RecData_AA;
    return  0;
//    switch(Mode)
//    {
//    case 1: Uart_DataHandle(5, 1);                              // ID串口数据处理（基础）
//            ID_Analysis();
//        break;
//    case 2: Uart_DataHandle(5, 1);                              // IC串口数据处理（基础）
//            IC_Data_Look();
//        break;
//    case 3: Uart_DataHandle(5, 3);                              // ETC串口数据处理（基础）
//            ETC_Data_Analysis();
//        break;
//    case 4: Uart_DataHandle(2, 2);                              // 2.4G串口数据处理（基础）
//            G_Analysis();
//        break;
//    }
//    ui->label_Rec_Num->setText(QString::number(Rec_Num));
}

void MainWindow::uart_sent()
{
    Serial->write("Send_Data");
}

﻿/*********************************************************************************************
*
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*********************************************************************************************
* 全局变量
*********************************************************************************************/
static unsigned long Rec_Num = 0;   //接收数
static unsigned long Send_Num = 0;  //发送数

static QString RecData_s;   //按长度解析的数据
static unsigned char Mode_flag = 0;
static QString IC_Card[30];     //IC寻卡卡号缓存区
static long balance = 0; //余额
static int Down_Money = 10; //消费
static unsigned char IC_Mess_Read_ok = 0;
static int Ticket_Int = 10;  //票价
static long Balance_Int = 0; //IC APP余额
static QString Mess_Pass = "FFFFFFFFFFFF"; //初始密码
static bool Begin_Look = false;  //IC寻卡标志位
static bool IC_App_Down_Ok = false;//IC余额不足标志位
static bool IC_Bus_Mode = false;  //IC模式标志位
static long gross = 0;      //售票金额
static bool IC_IsUp = false;
static bool IC_Mode = false;
static bool IC_CardApp_Up = false;
static bool IC_CardApp_Down = false;
static bool ID_Binding_OK = true; //ID应用弹窗标志位
static bool Type_flag = true;     //ID类型选择
static bool ID_New_Mode = false;  //ID界面基础或应用标志位

static QString isCom = "";
static QString com_str[30];
static QString ID_Data;                                         // ID卡模式处理过后的数据
static QString IC_Data;                                         // IC卡模式处理过后的数据
static QString ETC_Data;                                        // 900M卡模式处理过后的数据

#define Storage_Time   16

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    IsOpen = false;
    IsShow = true;
    IsStop = false;

    ui->Uart_Data->show();

    ui->tabWidget_ID->removeTab(1);
    ui->tabWidget_IC->removeTab(2);

    ui->comboBox_Baud->setCurrentIndex(7);

    ui->pushButton_IDMode->setEnabled(false);

    //ID
    ui->ID_pushButton_Read_Card->setEnabled(false);
    ui->ID_pushButton_Write_Card->setEnabled(false);

    //IC
    ui->IC_pushButton_Pass_Save->setEnabled(false);
    ui->IC_pushButton_Look_Bign->setEnabled(false);
    ui->IC_pushButton_Sys_Read->setEnabled(false);
    ui->IC_pushButton_Sys_Write->setEnabled(false);
    ui->IC_pushButton_Mess->setEnabled(false);
    ui->IC_pushButton_Up->setEnabled(false);
    ui->IC_pushButton_Down->setEnabled(false);
    ui->IC_pushButton_one->setEnabled(false);

    ui->IC_pushButton_App_Ticket->setEnabled(false);
    ui->IC_pushButton_App_Up_Money->setEnabled(false);

    ui->IC_radioButton_In->setEnabled(false);
    ui->IC_radioButton_Out->setEnabled(false);
    ui->IC_pushButton_Pass_oK->setEnabled(false);

    //获取当前设置的初始化时间
    Time_Morn_Up_Last = ui->ID_timeEdit_Morn_Up->time();
    Time_Morn_Down_Last = ui->ID_timeEdit_Morn_Down->time();
    Time_Afte_Up_Last = ui->ID_timeEdit_Afte_Up->time();
    Time_Afte_Down_Last = ui->ID_timeEdit_Afte_Down->time();
    Time_Morn_Up = Time_Morn_Up_Last;
    Time_Morn_Down = Time_Morn_Down_Last;
    Time_Afte_Up = Time_Afte_Up_Last;
    Time_Afte_Down = Time_Afte_Down_Last;

    //输入限制
    Regexp_Pass.setPattern("[0-9a-fA-F]{1,12}");
    Regexp_Sys.setPattern("[0-9a-fA-F ]{1,47}");
    Regexp_Num.setPattern("[0-9]{1,8}");
    Regexp_Num_Card.setPattern("[0-9a-fA-F]{1,10}");

    ui->IC_lineEdit_Pass_Last->setValidator(new QRegExpValidator(Regexp_Pass,ui->IC_lineEdit_Pass_Last));
    ui->IC_lineEdit_Pass_New->setValidator(new QRegExpValidator(Regexp_Pass,ui->IC_lineEdit_Pass_New));
    ui->IC_lineEdit_Sys_Pass->setValidator(new QRegExpValidator(Regexp_Pass,ui->IC_lineEdit_Sys_Pass));
    ui->IC_lineEdit_Mess_Pass->setValidator(new QRegExpValidator(Regexp_Pass,ui->IC_lineEdit_Mess_Pass));
    ui->IC_lineEdit_Sys_Write->setValidator(new QRegExpValidator(Regexp_Sys,ui->IC_lineEdit_Sys_Write));
    ui->IC_lineEdit_Pass->setValidator(new QRegExpValidator(Regexp_Pass,ui->IC_lineEdit_Pass));
    ui->ID_lineEdit_Write->setValidator(new QRegExpValidator(Regexp_Num_Card,ui->ID_lineEdit_Write));

    //表格
    model_ID->setColumnCount(2);
    model_ID->setHeaderData(0,Qt::Horizontal," 姓  名 ");
    model_ID->setHeaderData(1,Qt::Horizontal," 卡  号 ");
    ui->ID_tableView_ID->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->ID_tableView_ID->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ID_tableView_ID->setModel(model_ID);

    model_Data->setColumnCount(3);
    model_Data->setHeaderData(0,Qt::Horizontal," 姓 名 ");
    model_Data->setHeaderData(1,Qt::Horizontal,"上班时间");
    model_Data->setHeaderData(2,Qt::Horizontal,"下班时间");
    ui->ID_tableView_Data->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->ID_tableView_Data->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ID_tableView_Data->setModel(model_Data);

    Get_Com();
    Get_Time();

    //创建定时器
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Get_Com()));      //每一秒读取一次串口
    connect(timer,SIGNAL(timeout()),this,SLOT(Get_Time()));     //每一秒读取一次当前时间
    timer->start(1000);

    connect(IC_time_Mess,SIGNAL(timeout()),this,SLOT(IC_Read_Mess()));
    connect(&Serial,SIGNAL(readyRead()),SLOT(ReadUart()));
}

//关闭软件事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    (void)event;
}

//析构函数
MainWindow::~MainWindow()
{
    Serial.close();
    delete ui;
}

/*********************************************************************************************
* 名称:Get_Com()
* 功能:获取当前电脑可用串口
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Get_Com()
{
    static unsigned char Last_Com = 0;
    //智能识别当前系统的有效串口号
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
    int count = serialPortInfo.count();                     //有效串口的个数
    if(IsOpen == false)
    {
        if(count != Last_Com)                                   //避免出现串口刷新后自动改变
        {
            ui->comboBox_Com->clear();                          //清除下拉选择框中的内容
            for(int i=0;i<count;i++)
            {
                //把有效的串口号添加到comboBox_Com中
                ui->comboBox_Com->addItem(serialPortInfo.at(i).portName());
            }
            Last_Com = static_cast<unsigned char>(count);
        }
        else
        {
            return;
        }
    }
    else
    {
        for(int i=0; i<count; i++)
        {
            if(isCom == serialPortInfo.at(i).portName()) return;
        }
        OpenUart_Clicked();
    }
}

/*********************************************************************************************
* 名称:Get_Time()
* 功能:获取当前系统时间
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Get_Time()
{
    static unsigned char i = 0;
    time = QTime::currentTime();
    Time_str = time.toString("hh:mm:ss");
    ui->label_Time->setFont(QFont("",35,QFont::Bold));
    ui->label_Time->setText(Time_str);
    if(time.hour() >= 12)
    {
        IsMorn = false;
    }
    else
    {
        IsMorn = true;
    }

    if(i == 3)
    {
        i = 0;
        ui->ID_pushButton_Save->setText("保存");
        ui->IC_label_Pass_State->setText("");
        ui->IC_label_Data_State->setText("");
        ui->ID_label_State->setText("");
    }
    else
    {
        i++;
    }
}

/*********************************************************************************************
* 名称:SetUart()
* 功能:设置串口并打开并设置模式
* 参数:无
* 返回:false-打开失败/true-打开成功
*********************************************************************************************/
bool MainWindow::SetUart()
{
    //获取当前选择的串口号和波特率
    mCom = ui->comboBox_Com->currentText();
    mBaud = ui->comboBox_Baud->currentText();

    //按选择的设置串口,数据位,停止位,校验位,硬件流控已默认设置
    Serial.setPortName(mCom);
    Serial.setDataBits(QSerialPort::Data8);
    Serial.setStopBits(QSerialPort::OneStop);
    Serial.setParity(QSerialPort::NoParity);
    Serial.setFlowControl(QSerialPort::NoFlowControl);

    //按选择的波特率设置串口波特率
    if(mBaud == "1200")
    {
        Serial.setBaudRate(QSerialPort::Baud1200);
    }
    else if(mBaud == "2400")
    {
        Serial.setBaudRate(QSerialPort::Baud2400);
    }
    else if(mBaud == "4800")
    {
        Serial.setBaudRate(QSerialPort::Baud4800);
    }
    else if(mBaud == "9600")
    {
        Serial.setBaudRate(QSerialPort::Baud9600);
    }
    else if(mBaud == "19200")
    {
        Serial.setBaudRate(QSerialPort::Baud19200);
    }
    else if(mBaud == "38400")
    {
        Serial.setBaudRate(QSerialPort::Baud38400);
    }
    else if(mBaud == "57600")
    {
        Serial.setBaudRate(QSerialPort::Baud57600);
    }
    else
    {
        Serial.setBaudRate(QSerialPort::Baud115200);
    }

    return Serial.open(QSerialPort::ReadWrite);                 //返回是否打开串口并设置为读写模式
}

/*********************************************************************************************
* 名称:OpenUart_Clicked()
* 功能:点击打开串口按钮后,关闭或打开串口
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::OpenUart_Clicked()
{
    if(IsOpen == true)
    {
        Serial.close();                                         //关闭串口连接
        IsOpen = false;
        ui->pushButton_Open->setText("打开串口");                 //按键字变化
        IC_timer_read->stop();
        IC_time_Mess->stop();
        //打开或者关闭一些按键的可用性
        ui->comboBox_Baud->setEnabled(true);
        ui->comboBox_Com->setEnabled(true);
        //ID
        ui->ID_pushButton_Read_Card->setEnabled(false);
        ui->ID_pushButton_Write_Card->setEnabled(false);
        //IC
        ui->IC_pushButton_Pass_Save->setEnabled(false);
        ui->IC_pushButton_Look_Bign->setEnabled(false);
        ui->IC_pushButton_Sys_Read->setEnabled(false);
        ui->IC_pushButton_Sys_Write->setEnabled(false);
        ui->IC_pushButton_Mess->setEnabled(false);
        ui->IC_pushButton_Up->setEnabled(false);
        ui->IC_pushButton_Down->setEnabled(false);
        ui->IC_pushButton_one->setEnabled(false);

        ui->IC_pushButton_App_Ticket->setEnabled(false);
        ui->IC_pushButton_App_Up_Money->setEnabled(false);

        ui->IC_radioButton_In->setEnabled(false);
        ui->IC_radioButton_Out->setEnabled(false);
        ui->IC_pushButton_Pass_oK->setEnabled(false);
    }
    else                                                        //串口当前处于未打开状态
    {
        if(SetUart() == true)                                   //串口打开成功
        {
            isCom = ui->comboBox_Com->currentText();
            IsOpen = true;
            ui->pushButton_Open->setText("关闭串口");           //按键文字变化
            if(Mode == 2 && IC_Mode)
            {
                IC_Switch_Mode();
                QTimer::singleShot(500, this, SLOT(IC_App_Ticket_Save()));
                QTimer::singleShot(1000, this, SLOT(IC_Change_Pass()));
            }
            //打开或者关闭一些按键的可用性
            ui->comboBox_Baud->setEnabled(false);
            ui->comboBox_Com->setEnabled(false);
            /* ID */
            Type_flag = ui->ID_radioButton_EM4100->isChecked();
            if(Type_flag)
            {
                ui->ID_pushButton_Read_Card->setEnabled(true);
                ui->ID_pushButton_Write_Card->setEnabled(false);
            }
            else
            {
                ui->ID_pushButton_Read_Card->setEnabled(true);
                ui->ID_pushButton_Write_Card->setEnabled(true);
            }

            /* IC */
            ui->IC_pushButton_Pass_Save->setEnabled(true);
            ui->IC_pushButton_Look_Bign->setEnabled(true);
            ui->IC_pushButton_Sys_Read->setEnabled(true);
            // 根据 扇区和块 使能写按钮
            ui->IC_pushButton_Sys_Write->setEnabled(true);
            if(ui->IC_comboBox_Sys_Lump->currentIndex()==3 ||
                (ui->IC_comboBox_Sys_Region->currentIndex()==0 &&
                 ui->IC_comboBox_Sys_Lump->currentIndex()==0))
                    ui->IC_pushButton_Sys_Write->setEnabled(false);

            ui->IC_pushButton_Mess->setEnabled(true);
            ui->IC_pushButton_Up->setEnabled(true);
            ui->IC_pushButton_Down->setEnabled(true);
            ui->IC_pushButton_one->setEnabled(true);

            ui->IC_pushButton_App_Ticket->setEnabled(true);

            ui->IC_radioButton_In->setEnabled(true);
            ui->IC_radioButton_Out->setEnabled(true);
            ui->IC_pushButton_Pass_oK->setEnabled(true);
            if(IC_Bus_Mode)
                ui->IC_pushButton_App_Up_Money->setEnabled(true);
            else
                ui->IC_pushButton_App_Up_Money->setEnabled(false);

        }
        else
        {
            IsOpen = false;
            //串口打开失败,弹出打开失败原因信息
            QMessageBox::warning(this,"Connection Error",Serial.errorString());
            return;
        }
    }
}

/*********************************************************************************************
* 名称:StringToHex()
* 功能:将带有空格字符串 按照 16进制 进行储存
* 参数:str-需要转换的字符串，带有空格
*     send_data-16进制 数组
* 返回:无
*********************************************************************************************/
void MainWindow::StringToHex(QString str,QByteArray &send_data)
{
    int convert=0;                          // 转换 成功 的次数
    int len = str.length();                 // 字符串 长度
    send_data.resize(len/2);

    QByteArray byte_arr= str.toLatin1();
    char* c_string=byte_arr.data();         // 转换 C 字符串

    for (int i=0; i<len; i++) {
        char _str[3]={0};
        char hstr = str[i].toLatin1();
        if(hstr == ' ') continue;
        memcpy(_str,c_string+i,2);          // 取 需要转换的 两个字节
        send_data[convert]=static_cast<char>(strtol(_str, nullptr, 16));    // 将两个字节字符串 用 16进制单字节 数储存
        i++;                                // 跳过 低位
        convert++;                          // 成功次数
    }
    send_data.resize(convert);
}

/*********************************************************************************************
* 名称:Del_Num()
* 功能:复位计数
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Del_Num()
{
    Rec_Num = 0;
    Send_Num = 0;
    ui->label_Rec_Num->setText("0");
    ui->label_Send_Num->setText("0");
}

/*********************************************************************************************
* 名称:Change_ID()
* 功能:切换成ID考勤系统
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Change_ID()
{
    if(IsOpen)
        OpenUart_Clicked();
    ui->pushButton_IDMode->setEnabled(false);
    ui->pushButton_ICMode->setEnabled(true);
    if(!ID_New_Mode)
        ui->comboBox_Baud->setCurrentIndex(7);
    else
        ui->comboBox_Baud->setCurrentIndex(5);
    ui->stackedWidget->setCurrentIndex(0);
    ui->IC_lineEdit_Mess_Num->clear();
    ui->IC_lineEdit_Mess_Money->clear();

    IC_timer_read->stop();
    disconnect(IC_timer_read,SIGNAL(timeout()),this,SLOT(IC_Send_Look()));
    IC_time_Mess->stop();
    Mode = 1;
}

/*********************************************************************************************
* 名称:Change_IC()
* 功能:切换成公交IC卡
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Change_IC()
{
    if(IsOpen)
        OpenUart_Clicked();
    Mode_Change();
    ui->pushButton_ICMode->setEnabled(false);
    ui->pushButton_IDMode->setEnabled(true);

    ui->stackedWidget->setCurrentIndex(1);//界面切换

    IC_timer_read->stop();
    disconnect(IC_timer_read,SIGNAL(timeout()),this,SLOT(IC_Send_Look()));
    IC_time_Mess->stop();
    Mode = 2;
}

//版本更新
void MainWindow::About_up()
{
    QMessageBox::about(this,"版本更新","当前版本:V1.0."+ ver +"\n最新版本:V1.0."+ ver +"\n当前已是最新版本！");
}


/*********************************************************************************************
* 名称:Show_Data()
* 功能:显示和隐藏串口调试窗口
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Show_Data()
{
    if(IsShow)
    {
        IsShow = false;
        ui->pushButton_IsShow->setText("《\n《\n《");
        ui->Uart_Data->hide();
    }
    else
    {
        IsShow = true;
        ui->pushButton_IsShow->setText("》\n》\n》");
        ui->Uart_Data->show();
    }
}

/*********************************************************************************************
* 名称:Set_Show_Mode()
* 功能:设置串口调试信息显示模式
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Set_Show_Mode()
{
    Show_Mode = static_cast<unsigned char>(ui->comboBox_Show_Mode->currentIndex());
}

/*********************************************************************************************
* 名称:Stop_Rec()
* 功能:设置开始/停止接收调试信息
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Stop_Rec()
{
    if(IsStop)
    {
        IsStop = false;
        ui->pushButton_Rec_Mode->setText("停止接收");
    }
    else
    {
        IsStop = true;
        ui->pushButton_Rec_Mode->setText("开始接收");
    }
}

/*********************************************************************************************
* 名称:Get_Xor()
* 功能:计算异或校验位
* 参数:x--字符串开始位置/y--字符串间隔/z--字符串
* 返回:Data_Fcs--校验位
*********************************************************************************************/
QString MainWindow::Get_Xor(unsigned char x, unsigned char y, QString z)
{
    //求异或和
    char Data_Hex = 0;
    for(int a=x; a<z.length(); a+=y)
    {
        int Data_Int = z.mid(a,2).toInt(nullptr,16);               //将组合数据分解
        Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
    }
    QString Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
    int Data_Fcs_Int = Data_Fcs.toInt(nullptr,16);
    if(Data_Fcs_Int <= 15)
        Data_Fcs = '0'+Data_Fcs;
    return Data_Fcs;
}

/*********************************************************************************************
* 名称:Add_Space()
* 功能:添加空格
* 参数:x--字符串开始位置/z--字符串
* 返回:Data_Fcs--校验位
*********************************************************************************************/
QString MainWindow::Add_Space(unsigned char x, QString z)
{
    QString ss;
    for(int i=x; i<z.length(); i+=2)
    {
        QString aa = z.mid(i,2);
        ss += aa+" ";
    }
    return ss.toUpper();
}

/*********************************************************************************************
* 名称:ReadUart()
* 功能:读取串口数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::ReadUart()
{
    QByteArray RecData_AA;
    if(Show_Mode != 1  && !IsStop && IsOpen)
        Rec_Num += Serial.bytesAvailable();
    this->thread()->msleep(30);
    RecData_AA = Serial.readAll();                              // 读取串口的数据
    RecData = RecData_AA.toHex(' ').toUpper();
    switch(Mode)
    {
    case 1: Uart_DataHandle(5, 1);                              // ID串口数据处理（基础）
            ID_Analysis();
        break;
    case 2: Uart_DataHandle(5, 1);                              // IC串口数据处理（基础）
            IC_Data_Look();
        break;
    }
    ui->label_Rec_Num->setText(QString::number(Rec_Num));
}


/*******************************************************************************************************/
//ID考勤系统
/*******************************************************************************************************/

//ID界面切换
void MainWindow::ID_Change_Mode()
{
    if(ID_New_Mode)        //基础
    {
        ID_New_Mode = false;
        ui->comboBox_Baud->setCurrentIndex(7);
    }
    else            //应用
    {
        ID_New_Mode = true;
        ui->comboBox_Baud->setCurrentIndex(5);
    }
}

//类型选择
void MainWindow::ID_EM4100()
{
    if(IsOpen)
    {
        Type_flag = ui->ID_radioButton_EM4100->isChecked();
        if(!Type_flag)
        {
            ui->ID_pushButton_Read_Card->setEnabled(true);
            ui->ID_pushButton_Write_Card->setEnabled(true);
        }
        else
        {
            ui->ID_pushButton_Write_Card->setEnabled(false);
            ui->ID_pushButton_Read_Card->setEnabled(true);
        }
    }
}

//读取卡号
void MainWindow::ID_Read_Card()
{
    QByteArray Send_Data;
    QString Mold_125 = "AB BA 00 15 00 15";                 //读取125k卡的指令
    StringToHex(Mold_125,Send_Data);                        //先将发送框的内容转换为Hex型
    qDebug()<< Send_Data;       // "\xAB\xBA\x00\x15\x00\x15"
    Serial.write(Send_Data);
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Mold_125+"(发送读取卡号指令)");
}

//写卡号
void MainWindow::ID_Write_Card()
{
    QByteArray Send_Data;
    QString Write_Card[2];
    Write_Card[1] = ui->ID_lineEdit_Write->text();
    if(Write_Card[1].isEmpty())
    {
        QMessageBox::warning(this,"提示","卡号不能为空!");
    }
    else if(Write_Card[1].length() < 10)
    {
        QMessageBox::warning(this,"提示","卡号不能低于10位!");
    }
    else
    {
        Write_Card[0] = "AB BA 00 16 05 ";
        Write_Card[1] = Add_Space(0,Write_Card[1]);
        QString data = Write_Card[0]+Write_Card[1];
        char Data_Hex = 0;
        for(int a=6; a<data.length(); a+=3)
        {
            int Data_Int = data.mid(a,2).toInt(nullptr,16);               //将组合数据分解
            Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
        }
        QString Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
        int Data_Fcs_Int = Data_Fcs.toInt(nullptr,16);
        if(Data_Fcs_Int <= 15)
            Data_Fcs = '0'+Data_Fcs;
        data += Data_Fcs.toUpper();
        StringToHex(data,Send_Data);
        Serial.write(Send_Data);
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        if(Show_Mode != 2 && !IsStop && IsOpen)
            ui->textEdit_Data->append(Time_str+" <-- "+data+"(发送修改卡号指令)");
    }
}

/*********************************************************************************************
* 名称:Message()
* 功能:弹出ID绑定的窗口
* 参数:无
* 返回:0--绑定失败/1--绑定成功
*********************************************************************************************/
bool MainWindow::Message()
{
    bool ok;
    ID_Binding_OK = false;
    ID_Name = QInputDialog::getText(this, tr("ID绑定"), tr("请输入员工姓名(请不要超过10个字符)："), QLineEdit::Normal, tr(""),&ok);
    if(ok && !ID_Name.isEmpty())
    {
        if(ID_Name.length()>10)
        {
            QMessageBox::warning(this,"提示","输入的字符请不要超过10个！");
            ok = false;
        }
        else
        {
            model_ID->setItem(ID_Num,0,new QStandardItem(ID_Name));
            model_ID->setItem(ID_Num,1,new QStandardItem(RecData_ID.remove(" ")));
            ok = true;
        }
    }
    else
        ok = false;
    return ok;
}

/*********************************************************************************************
* 名称:Time_Save()
* 功能:设置时间范围保存
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Time_Save()
{
    //判断当前时间设置的逻辑是否正确
    if((ui->ID_timeEdit_Afte_Up->time() > ui->ID_timeEdit_Afte_Down->time()) ||
            ui->ID_timeEdit_Morn_Up->time() > ui->ID_timeEdit_Morn_Down->time())
    {
        ui->ID_pushButton_Save->setText("保存失败");
        ui->ID_timeEdit_Morn_Up->setTime(Time_Afte_Down_Last);
        ui->ID_timeEdit_Morn_Down->setTime(Time_Morn_Down_Last);   //恢复时间范围为初始值
        Time_Morn_Up = Time_Morn_Up_Last;
        Time_Morn_Down = Time_Morn_Down_Last;
        ui->ID_timeEdit_Afte_Up->setTime(Time_Afte_Up_Last);
        ui->ID_timeEdit_Afte_Down->setTime(Time_Afte_Down_Last);
        Time_Afte_Up = Time_Afte_Up_Last;
        Time_Afte_Down = Time_Afte_Down_Last;
        QMessageBox::warning(this,"提示","下班时间应不早于上班时间，请重新修改上下班时间！");
    }
    else
    {
        Time_Morn_Up = ui->ID_timeEdit_Morn_Up->time();
        Time_Morn_Down = ui->ID_timeEdit_Morn_Down->time();
        Time_Afte_Up = ui->ID_timeEdit_Afte_Up->time();
        Time_Afte_Down = ui->ID_timeEdit_Afte_Down->time();
        ui->ID_pushButton_Save->setText("保存成功");
    }
}

//开启门禁
void MainWindow::ID_Open_Door()
{
    QByteArray Send_Data;
    QString open_door = "AF 00 0F 0F";
    ui->textEdit_Data->append(Time_str+" <-- "+open_door+"(开启门禁)");
    StringToHex(open_door,Send_Data);
    Serial.write(Send_Data);
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
}

//发送考勤状态
void MainWindow::ID_CheckMode(bool mode)
{
    QByteArray Send_Data;
    if(mode)            //正常考勤
    {
        QString str_Y = "AF 00 10 10";
        StringToHex(str_Y,Send_Data);
        Serial.write(Send_Data);
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        ui->textEdit_Data->append(Time_str+" <-- "+str_Y+"(发送考勤状态)");
    }
    else                //早退或迟到
    {
        QString str_N = "AF 00 1F 1F";
        StringToHex(str_N,Send_Data);
        Serial.write(Send_Data);
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        ui->textEdit_Data->append(Time_str+" <-- "+str_N+"(发送考勤状态)");
    }

}

/*********************************************************************************************
* 名称:ID_Analysis()
* 功能:解析串口读取的ID卡数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::ID_Analysis()
{
    QString state;
    bool ID_OK = false;
    QString Mold = ID_Data.mid(0,8);
    if(Mold == "BF 05 0F" && ID_Binding_OK)
    {
        RecData_ID = ID_Data.mid(9,14);
        if(RecData_ID.length() == 14)  //卡号正常
        {
            if(Show_Mode != 1 && !IsStop && IsOpen)
                ui->textEdit_Data->append(Time_str+" --> "+ID_Data+"(读取到员工卡号)");
            for(int i=0; i<10; i++)
            {
                if(RecData_ID == ID[i])
                {
                    //判断当前时间段
                    if(!IsMorn)
                    {
                        if(Sign_Num[i] == 0)
                        {
                            if(time > Time_Afte_Up)
                            {
                                state = "(迟到)";
                                ID_CheckMode(0);
                            }
                            else
                            {
                                state = "(正常)";
                                ID_CheckMode(1);
                            }
                            model_Data->setItem(i,1,new QStandardItem(Time_str+state));
                            model_Data->setItem(i,2,new QStandardItem(""));
                        }
                        else
                        {
                            if(time < Time_Afte_Down)
                            {
                                state = "(早退)";
                                ID_CheckMode(0);
                            }
                            else
                            {
                                state = "(正常)";
                                ID_CheckMode(1);
                            }
                            model_Data->setItem(i,2,new QStandardItem(Time_str+state));
                        }
                    }
                    else
                    {
                        if(Sign_Num[i] == 0)
                        {
                            if(time > Time_Morn_Up)
                            {
                                state = "(迟到)";
                                ID_CheckMode(0);
                            }
                            else
                            {
                                state = "(正常)";
                                ID_CheckMode(1);
                            }
                            model_Data->setItem(i,1,new QStandardItem(Time_str+state));
                            model_Data->setItem(i,2,new QStandardItem(""));
                        }
                        else
                        {
                            if(time < Time_Morn_Down)
                            {
                                state = "(早退)";
                                ID_CheckMode(0);
                            }
                            else
                            {
                                state = "(正常)";
                                ID_CheckMode(1);
                            }
                            model_Data->setItem(i,2,new QStandardItem(Time_str+state));
                        }
                    }
                    if(Sign_Num[i] < 1)                         //签到次数
                        Sign_Num[i]++;
                    else
                        Sign_Num[i] = 0;
                    ID_OK = true;
                    QTimer::singleShot(500, this, SLOT(ID_Open_Door()));   //开门
                    break;
                }
            }
            //如果是第一次刷卡
            if(ID_OK == false)
            {
                if(ID_Num >= 10)
                    QMessageBox::warning(this,"提示","超出记录次数范围！");
                else
                {
                    if(Message())                               //输入有效的姓名
                    {
                        strcpy(Name[ID_Num],ID_Name.toUtf8().data());//储存员工姓名
                        model_Data->setItem(ID_Num,0,new QStandardItem(QString::fromUtf8(Name[ID_Num])));
                        ID[ID_Num] = RecData_ID;
                        ID_Num++;
                        ID_Open_Door();         //开门
                        ID_Binding_OK = true;
                    }
                }
            }
            ID_Data.clear();
        }
    }
    else
    {
        if(ID_Data == "CD DC 00 80 00 80")
        {
            if(Show_Mode != 1 && !IsStop && IsOpen)
                ui->textEdit_Data->append(Time_str+" --> "+ID_Data+"(未读取到卡号)");
        }
        else if(ID_Data == "CD DC 00 81 00 81")
        {
            if(Show_Mode != 1 && !IsStop && IsOpen)
                ui->textEdit_Data->append(Time_str+" --> "+ID_Data+"(修改卡号成功)");
            ui->ID_listWidget_Card_Data->addItem(Time_str+" <-- 修改卡号为 "+ui->ID_lineEdit_Write->text());
        }
        else if(ID_Data.mid(0, 11) == "CD DC 00 81")
        {
            if(Show_Mode != 1 && !IsStop && IsOpen)
                ui->textEdit_Data->append(Time_str+" --> "+ID_Data+"(读取到卡号)");
            ui->ID_listWidget_Card_Data->addItem(Time_str+" <-- 读取到卡号为 "+ID_Data.mid(14, 15).remove(" "));
        }
        ui->ID_listWidget_Card_Data->scrollToBottom();
        ID_Data.clear();
    }
}


/*********************************************************************************************/
//公交非接触IC卡
/*********************************************************************************************/
/************************/
/**********基础***********/
/************************/

//删除数据
void MainWindow::IC_Del_Card()
{
    ui->IC_listWidget_Look_Data->clear();
    for(int i=0; i<30; i++)
        IC_Card[i].clear();
}

//寻卡指令
void MainWindow::IC_Send_Look()
{
    QByteArray Send_Data;
    QString Mold_1356 = "AB BA 00 10 00 10";                    //读取13.56M卡的指令
    StringToHex(Mold_1356,Send_Data);
    Serial.write(Send_Data);
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Mold_1356+"(13.56M寻卡指令)");
    if(Show_Mode != 2 && !IsStop && IsOpen)
    {
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
    }
}

//开始寻卡指令
void MainWindow::IC_Begin_Look()
{

    if(Begin_Look)
    {
        Begin_Look = false;
        ui->IC_pushButton_Look_Bign->setText("开始寻卡");
        IC_timer_read->stop();
        disconnect(IC_timer_read,SIGNAL(timeout()),this,SLOT(IC_Send_Look()));
    }
    else
    {
        Begin_Look = true;
        ui->IC_pushButton_Look_Bign->setText("停止寻卡");
        IC_timer_read->start(1000);
        connect(IC_timer_read,SIGNAL(timeout()),this,SLOT(IC_Send_Look()));
    }

}

//寻卡分析(最多30张)
void MainWindow::IC_Data_Look()
{
    QString State;
    QString IC_Rec;
    static unsigned char IC_Sum = 0;
    bool ok = false;
    if(!IC_Mode)        //基础
    {
        if(IC_Data.mid(0,14) == "CD DC 00 81 04")
        {
            if(Mode_flag == 5)
            {
                ui->IC_lineEdit_Mess_Num->setText(IC_Data.mid(15,11).remove(" "));
                State = "(读取用户信息成功)";
            }
            else
            {
                QString IC_Num = IC_Data.mid(15,11);
                for(int i=0; i<30; i++)
                {
                    if(IC_Num == IC_Card[i])
                    {
                        ok = true;
                        break;
                    }
                }
                if(ok == false)
                {
                    if(IC_Sum >= 30)
                    {
                        QMessageBox::warning(this,"提示","超出最大卡号储存范围!");
                    }
                    else
                    {
                        IC_Card[IC_Sum] = IC_Num;
                        ui->IC_listWidget_Look_Data->addItem(IC_Num.remove(" "));
                        IC_Sum++;
                    }
                }
                State = "(读取到13.56M类型卡号)";
            }
        }
        else if(IC_Data.mid(0,11) == "CD DC 00 81")
        {
            switch(Mode_flag)
            {
            case 0:
                if(IC_CardApp_Up)
                {
                    State = "(充值成功)";
                    IC_CardApp_Up = false;
                    int m = ui->IC_lineEdit_Mess_Money->text().toInt() + ui->IC_lineEdit_Up_Money->text().toInt();
                    ui->IC_lineEdit_Mess_Money->setText(QString::number(m, 10));
                    ui->IC_listWidget_Data->addItem(Time_str+" --> "+ui->IC_lineEdit_Mess_Num->text()+" 充值 "+ui->IC_lineEdit_Up_Money->text());
                    ui->IC_listWidget_Data->scrollToBottom();
                }
                else if(IC_CardApp_Down)
                {
                    State = "(消费成功)";
                    IC_CardApp_Down = false;
                    int m = ui->IC_lineEdit_Mess_Money->text().toInt() - ui->IC_lineEdit_Down_Money->text().toInt();
                    ui->IC_lineEdit_Mess_Money->setText(QString::number(m, 10));
                    ui->IC_listWidget_Data->addItem(Time_str+" <-- "+ui->IC_lineEdit_Mess_Num->text()+" 消费 "+ui->IC_lineEdit_Down_Money->text());
                    ui->IC_listWidget_Data->scrollToBottom();
                }
                IC_Mess_Read_ok++;
                break;
            case 1:
                State = "(修改密码成功)";
                ui->IC_label_Pass_State->setText("修改成功");
                if(ui->IC_radioButton_Pass_A->isChecked()){
                    QMessageBox::information(this,"提示","A密码修改成功,如果遗忘,\n可以通过修改B密码来重置A密码为0");
                }else{
                    QMessageBox::information(this,"提示","A密码重置为0");
                }
                break;
            case 2:
                State = "(读取数据成功)";
                ui->IC_label_Data_State->setText("读取成功");
                ui->IC_listWidget_Sys_Data->addItem("("+QString::number(IC_Data.mid(16,2).toInt(nullptr,16),10)+
                                                    ","+QString::number(IC_Data.mid(19,2).toInt(nullptr,16),10)+
                                                    ") "+IC_Data.mid(21,47));
                ui->IC_listWidget_Sys_Data->scrollToBottom();
                break;
            case 3:
                State = "(写入数据成功)";
                ui->IC_label_Data_State->setText("写入成功");
                break;
            case 4:
                if(IC_Data.mid(12, 2) == "12")
                {
                    State = "(读取用户信息成功)";  //读取余额
                    balance = (IC_Data.mid(60,2)+IC_Data.mid(63,2)+IC_Data.mid(66,2)).toInt(nullptr,16);//余额
                    ui->IC_lineEdit_Mess_Money->setText(QString::number(balance,10));
                    IC_Mess_Read_ok++;
                }
                break;
            case 5:                          //读取卡号
                if(IC_CardApp_Up)
                {
                    State = "(充值成功)";
                    IC_CardApp_Up = false;
                    ui->IC_listWidget_Data->addItem(Time_str+" --> "+ui->IC_lineEdit_Mess_Num->text()+" 充值 "+ui->IC_lineEdit_Up_Money->text());
                    ui->IC_listWidget_Data->scrollToBottom();
                }
                else if(IC_CardApp_Down)
                {
                    State = "(消费成功)";
                    IC_CardApp_Down = false;
                    ui->IC_listWidget_Data->addItem(Time_str+" <-- "+ui->IC_lineEdit_Mess_Num->text()+" 消费 "+ui->IC_lineEdit_Down_Money->text());
                    ui->IC_listWidget_Data->scrollToBottom();
                }
                IC_Mess_Read_ok++;
                break;
            case 6:
                State = "(修改余额成功)";
                break;
            }
            Mode_flag = 0;
        }
        else
        {
            if(!IC_Data.isEmpty())
            {
                switch(Mode_flag)
                {
                case 1:
                    State = "(修改密码失败)";
                    ui->IC_label_Pass_State->setText("修改失败");
                    break;
                case 2:
                    State = "(读取数据失败)";
                    ui->IC_label_Data_State->setText("读取失败");
                    break;
                case 3:
                    State = "(写入数据失败)";
                    ui->IC_label_Data_State->setText("写入失败");
                    break;
                case 4:                         //读取余额
                    State = "(读取用户信息失败)";
                    //ui->IC_lineEdit_Mess_Num->clear();
                    //ui->IC_lineEdit_Mess_Money->clear();
                    break;
                case 5:
                    State = "(读取用户信息失败)";  //读取卡号
                    //ui->IC_lineEdit_Mess_Money->clear();
                    //ui->IC_lineEdit_Mess_Num->clear();
                    break;
                case 6:
                    State = "(修改余额失败)";
                    break;
                default :
                    State = "(未检测到13.56M类型卡)";
                    break;
                }
                Mode_flag = 0;
            }
        }
        if(Show_Mode != 1 && !IsStop && IsOpen && !IC_Data.isEmpty())
            ui->textEdit_Data->append(Time_str+" --> "+IC_Data.toUpper()+State);
        IC_Data.clear();
    }
    else   //应用界面
    {
        QString RecData_IC_App_Data;

        if(RecData_IC_App_Data.isEmpty())
        {
            RecData_IC_App_Data += IC_Data;
        }
        else
        {
            RecData_IC_App_Data += IC_Data;
        }
        for(int x=0; x<RecData_IC_App_Data.length(); x+=3)              //解析储存的数据
        {
            if(RecData_IC_App_Data.mid(x,2) == "BF")
            {
                int RecData_Sum = RecData_IC_App_Data.mid(x+3,2).toInt(nullptr,16);//获取数据长度
                if(RecData_IC_App_Data.length()>=(RecData_Sum+4)*3-1)
                {
                    IC_Rec = RecData_IC_App_Data.mid(x,(RecData_Sum+4)*3);//截取数据长度的数据

                    if(IC_Rec.mid(0,2) == "BF")
                    {
                        if(IC_Rec.mid(6,2) == "0B")
                        {
                            if(IC_Rec.mid(3,2) == "08")
                            {
                                ui->IC_lineEdit_App_Card_Num->setText(IC_Rec.mid(9,11).remove(" ")); //卡号
                                Balance_Int = (IC_Rec.mid(24,2)+IC_Rec.mid(27,2)+IC_Rec.mid(30,2)).toInt(nullptr,16);//余额
                                ui->IC_lineEdit_App_Balance->setText(QString::number(Balance_Int,10));
                                if(Balance_Int >= Ticket_Int)
                                    IC_App_Down_Ok = false;
                                State = "(用户信息)";
                            }
                            else
                            {
                                if(IC_Rec.mid(9,2) == "00")
                                {
                                    if(!IC_IsUp)
                                    {
                                        ui->IC_listWidget_App_Data->addItem(Time_str+" <-- "+ui->IC_lineEdit_App_Card_Num->text()+
                                                                            " 扣费 "+QString::number(Ticket_Int)+"元");
                                        State = "(扣费成功)";
                                    }
                                    else
                                    {
                                        ui->IC_listWidget_App_Data->addItem(Time_str+" --> "+ui->IC_lineEdit_App_Card_Num->text()+
                                                                            " 充值 "+ui->IC_lineEdit_App_Up_Money->text()+"元");
                                        State = "(充值成功)";     
                                    }
                                    ui->IC_lineEdit_App_Balance->setText(QString::number(Balance_Int, 10));
                                    ui->IC_listWidget_App_Data->scrollToBottom();
                                }
                                else            //修改余额失败
                                {
                                    if(!IC_IsUp)
                                        State = "(扣费失败)";
                                    else
                                        State = "(充值失败)";
                                }
                            }
                        }
                        else if(IC_Rec.mid(6,2) == "0C")
                        {
                            State = "(修改票价成功)";//修改票价成功
                            ui->IC_listWidget_App_Data->addItem(Time_str+" ---- 修改票价为"+QString::number(Ticket_Int,10));
                            ui->IC_listWidget_App_Data->scrollToBottom();
                        }
                        else if(IC_Rec.mid(6,2) == "0D")
                        {
                            QString m;
                            State = "(修改模式成功)";//修改模式成功
                            if(!ui->IC_radioButton_In->isChecked())
                                m = "乘车模式";
                            else
                                m = "充值模式";
                            ui->IC_listWidget_App_Data->addItem(Time_str+" ---- 修改模式为"+m);
                            ui->IC_listWidget_App_Data->scrollToBottom();
                        }
                        else if(IC_Rec.mid(6,2) == "0E")
                        {
                            State = "(同步密码成功)";//同步密码成功
                            ui->IC_listWidget_App_Data->addItem(Time_str+" ---- 修改单片机内部密码为"+ui->IC_lineEdit_Mess_Pass->text());
                            ui->IC_listWidget_App_Data->scrollToBottom();
                        }
                    }
                    RecData_IC_App_Data.replace(x,(RecData_Sum+4)*3,"");//删除已经解析完的数据
                    x=-3;
                }
                else
                {
                    break;
                }
            }
        }
        if(Show_Mode != 1 && !IsStop && IsOpen && !IC_Rec.isEmpty())
            ui->textEdit_Data->append(Time_str+" --> "+IC_Rec.toUpper()+State);
    }
    if(IC_Rec.mid(6,2) == "0B" && IC_Rec.mid(3,2) == "08")
    {
       if(!IC_App_Down_Ok && !IC_Bus_Mode )
       {
           IC_App_Down_Money();  //扣费操作
           if(!IC_App_Down_Ok)
           {
               gross += Ticket_Int;
               ui->IC_label_Head_Money->setFont(QFont("",15,QFont::Bold));
               ui->IC_label_Head_Money->setText(QString::number(gross,10));
           }
       }
    }
}

//修改IC卡A或者B密码
void MainWindow::IC_Pass_Change()
{
//#define ENABLE_CONFIRM
    bool ok = false;
    QByteArray Send_Data;
    QString Write_Pass[5];
    Write_Pass[0] = "ABBA";
    int regic = ui->IC_comboBox_Pass_Regic->currentIndex();
    if(ui->IC_radioButton_Pass_A->isChecked())
    {
        Write_Pass[1] = "00140E0"+QString::number(regic,16)+"0A";
    }
    else
    {
        Write_Pass[1] = "00140E0"+QString::number(regic,16)+"0B";
    }
    Write_Pass[2] = ui->IC_lineEdit_Pass_Last->text();
    Write_Pass[3] = ui->IC_lineEdit_Pass_New->text();
    if(Write_Pass[2].isEmpty() || Write_Pass[3].isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空！");
    }
    else if(Write_Pass[2].length()<12 || Write_Pass[3].length()<12)
    {
        QMessageBox::warning(this,"警告","密码不能低于12位！");
    }
    else                    // 可以正常修改密码
    {
#ifdef ENABLE_CONFIRM
        QString Pass_Change_OK = QInputDialog::getText(this, tr("修改密码"), tr("请牢记修改后密码！否则此卡将报废！是否继续(输入“确认”继续)？"), QLineEdit::Normal, tr(""),&ok);
        if(Pass_Change_OK == "确认")
        {
            ok = true;
        }
        else
        {
            ok = false;
        }
#else
        if(QMessageBox::Yes==QMessageBox::critical(this, "注意","确认修改密码？",
                                 QMessageBox::Yes, QMessageBox::No))
            ok = true;
        else ok = false;
#endif
    }
    if(ok == true)
    {
        QString Pass = Write_Pass[0]+Write_Pass[1]+Write_Pass[2]+Write_Pass[3];
        Write_Pass[4] = Get_Xor(4,2,Pass);                  // 校验和
        Pass += Write_Pass[4];
        StringToHex(Pass,Send_Data);
        Serial.write(Send_Data);
        Mode_flag = 1;                                      // 修改密码操作标志位
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        if(Show_Mode != 2 && !IsStop && IsOpen)
            ui->textEdit_Data->append(Time_str+" <-- "+Add_Space(0,Pass)+"(修改密码指令)");
    }
}

//写扇区
void MainWindow::IC_Write_Region()
{
    QByteArray Send_Data;
    QString Sys_Data[5];
    Sys_Data[0] = "ABBA";
    Sys_Data[1] = "0013190"+QString::number(ui->IC_comboBox_Sys_Region->currentIndex(),16);
    QString Pass_Mode;
    if(ui->IC_comboBox_passMode->currentIndex() == 0)
        Pass_Mode = "0A";
    else
        Pass_Mode = "0B";
    Sys_Data[2] = "0"+QString::number(ui->IC_comboBox_Sys_Lump->currentIndex(),16) + Pass_Mode;
    Sys_Data[3] = ui->IC_lineEdit_Sys_Pass->text();//密码
    Sys_Data[4] = ui->IC_lineEdit_Sys_Write->text().remove(' ');//要写入的数据
    QString Sys = Sys_Data[0]+Sys_Data[1]+Sys_Data[2]+Sys_Data[3]+Sys_Data[4];
    Sys += Get_Xor(4,2,Sys);
    StringToHex(Sys,Send_Data);
    Serial.write(Send_Data);
    Mode_flag = 3;
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Add_Space(0,Sys)+"(向指定位置写入数据)");
}

//读扇区
void MainWindow::IC_Read_Region()
{
    QByteArray Send_Data;
    QString Sys_Data[4];
    Sys_Data[0] = "ABBA";
    Sys_Data[1] = "0012090"+QString::number(ui->IC_comboBox_Sys_Region->currentIndex(),16);
    QString Pass_Mode;
    if(ui->IC_comboBox_passMode->currentIndex() == 0)
        Pass_Mode = "0A";
    else
        Pass_Mode = "0B";
    Sys_Data[2] = "0"+QString::number(ui->IC_comboBox_Sys_Lump->currentIndex(),16) + Pass_Mode;

    Sys_Data[3] = ui->IC_lineEdit_Sys_Pass->text();
    QString Sys = Sys_Data[0]+Sys_Data[1]+Sys_Data[2]+Sys_Data[3];
    Sys += Get_Xor(4,2,Sys);
    StringToHex(Sys,Send_Data);
    Serial.write(Send_Data);
    Mode_flag = 2;
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
    {
        ui->textEdit_Data->append(Time_str+" <-- "+Add_Space(0,Sys)+"(读取指定扇区数据)");
    }
}


/************************/
/**********基础二***********/
/************************/

//界面切换
void MainWindow::Mode_Change()
{
    static bool Band_Change = false;
    if(ui->tabWidget_IC->currentIndex() == 0) //切换到基础界面
    {
        IC_Mode = false;
        if(Band_Change)
        {
            if(IsOpen)
                OpenUart_Clicked();

            Band_Change = false;
        }
        ui->comboBox_Baud->setCurrentIndex(7); //设置默认波特率
        IC_time_Mess->stop();
        ui->IC_lineEdit_Mess_Num->clear();
        ui->IC_lineEdit_Mess_Money->clear();
    }
    else if(ui->tabWidget_IC->currentIndex() == 1)       //切换到应用界面
    {
        IC_Mode = false;
        if(Band_Change)
        {
            if(IsOpen)
                OpenUart_Clicked();

            Band_Change = false;
        }
        ui->comboBox_Baud->setCurrentIndex(7); //设置默认波特率
        IC_timer_read->stop();
        if(Begin_Look)
            IC_Begin_Look();   //关闭寻卡功能
    }
    else
    {
        IC_Mode = true;
        Band_Change = true;
        if(IsOpen)
            OpenUart_Clicked();
        ui->comboBox_Baud->setCurrentIndex(5); //设置默认的波特率
        IC_time_Mess->stop();
        ui->IC_lineEdit_Mess_Num->clear();
        ui->IC_lineEdit_Mess_Money->clear();
        IC_timer_read->stop();
        if(Begin_Look)
            IC_Begin_Look();                  //关闭寻卡功能
    }
    IC_Mess_Read_ok = 0;
}

//点击获取用户信息
void MainWindow::IC_Read_Mess_flag()
{
    Mess_Pass = ui->IC_lineEdit_Mess_Pass->text();
    if(Mess_Pass.isEmpty())
    {
        QMessageBox::warning(this,"提示","密码不能为空!");
    }
    else if(Mess_Pass.length() < 12)
    {
        QMessageBox::warning(this,"提示","密码不能少于12位!");
    }
    else
    {
        IC_time_Mess->start(1000);
    }
}

//读取信息
void MainWindow::IC_Read_Mess()
{
    QByteArray Send_Data;
    static bool i = false;
    if(i)
    {
        i = false;
        QString Sys_Data[3];
        Sys_Data[0] = "ABBA";
        Sys_Data[1] = "00120901010A";
        Sys_Data[2] = Mess_Pass;
        QString Mess = Sys_Data[0]+Sys_Data[1]+Sys_Data[2];
        Mess +=  Get_Xor(4,2,Mess);
        StringToHex(Mess,Send_Data);
        Serial.write(Send_Data);
        Mode_flag = 4;
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        if(Show_Mode != 2 && !IsStop && IsOpen)
        {
            ui->textEdit_Data->append(Time_str+" <-- "+Add_Space(0,Mess)+"(读取指定扇区数据)");
        }
    }
    else
    {
        i = true;
        IC_Send_Look();
        Mode_flag = 5;
    }
}

//修改余额
void MainWindow::Change_Money(unsigned long x)
{
    QByteArray Send_Data;
    QString Sys_Data[6];
    Sys_Data[0] = "ABBA";
    Sys_Data[1] = "00131901010A"+ui->IC_lineEdit_Mess_Pass->text();
    Sys_Data[2] = "00000000000000000000000000";
    Sys_Data[3] = QString::number(x / 65536,16);
    if(Sys_Data[3].toInt(nullptr,16) <= 15)
    {
        Sys_Data[3] = "0"+Sys_Data[3];
    }
    Sys_Data[4] = QString::number((x / 256) % 256,16);
    if(Sys_Data[4].toInt(nullptr,16) <= 15)
    {
        Sys_Data[4] = "0"+Sys_Data[4];
    }
    Sys_Data[5] = QString::number((x % 256),16);
    if(Sys_Data[5].toInt(nullptr,16) <= 15)
    {
        Sys_Data[5] = "0"+Sys_Data[5];
    }
    QString Money = Sys_Data[0]+Sys_Data[1]+Sys_Data[2]+Sys_Data[3]+Sys_Data[4]+Sys_Data[5];
    Money += Get_Xor(4,2,Money);
    StringToHex(Money,Send_Data);
    Serial.write(Send_Data);
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Add_Space(0,Money)+"(修改余额操作)");
}

//充值
void MainWindow::Money_Up()
{
    QString Money_str = ui->IC_lineEdit_Up_Money->text();
    if(Money_str.isEmpty())
    {
        QMessageBox::warning(this,"提示","不能为空!");
    }
    else
    {
        unsigned long Money = Money_str.toInt(nullptr,10);
        balance += Money;
        if(balance > 16777215)
        {
            balance -= Money;
            QMessageBox::warning(this,"提示","余额最大16777215元!请修改充值金额!");
        }
        else
        {
            Change_Money(static_cast<unsigned long>(balance));
            IC_CardApp_Up = true;

        }
    }
}

//消费保存
void MainWindow::Down_Money_Save()
{
    QString Down_Money_str = ui->IC_lineEdit_Down_Money->text();
    if(Down_Money_str.isEmpty())
    {
        QMessageBox::warning(this,"提示","不能为空!");
    }
    else
    {
        Down_Money = Down_Money_str.toInt(nullptr,10);
        Money_Down();
    }
}

//消费
void MainWindow::Money_Down()
{
    if(balance < Down_Money)
    {
        QMessageBox::warning(this,"提示","余额不足!请充值!");
    }
    else
    {
        balance -= Down_Money;
        Change_Money(static_cast<unsigned long>(balance));

        IC_CardApp_Down = true;
    }
}


/***********应用************/

//模式切换
void MainWindow::IC_Switch_Mode()
{
    QByteArray Send_Data;
    QString Switch_Mode;

    if(ui->IC_radioButton_Out->isChecked())
    {
        IC_Bus_Mode = false;
        ui->IC_pushButton_App_Up_Money->setEnabled(false);
        ui->IC_label_Pass_Text->setText("密码A：");
    }
    else
    {
        IC_Bus_Mode = true;
        ui->IC_pushButton_App_Up_Money->setEnabled(true);
        ui->IC_label_Pass_Text->setText("密码B：");
    }
    if(!IC_Bus_Mode)
        Switch_Mode = "AF 01 0D 0A 06";
    else
    {
        Switch_Mode = "AF 01 0D 0B 07";

    }
    StringToHex(Switch_Mode,Send_Data);
    Serial.write(Send_Data);
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Switch_Mode+"(修改模式操作)");
}

//修改IC卡A或者B密码
void MainWindow::IC_Change_Pass()
{
    QByteArray Send_Data;
    QString Pass[2];
    if(ui->IC_lineEdit_Pass->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","密码不能为空！");
    }
    else if(ui->IC_lineEdit_Pass->text().length() < 12)
    {
        QMessageBox::warning(this,"提示","密码不能少于12位！");
    }
    else
    {
        Pass[0] = "AF 06 0E ";
        Pass[1] = Add_Space(0,ui->IC_lineEdit_Pass->text());
        QString Data = Pass[0] + Pass[1];
        Data = Data+Get_Xor(3,3,Data);
        StringToHex(Data,Send_Data);
        Serial.write(Send_Data);
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        if(Show_Mode != 2 && !IsStop && IsOpen)
            ui->textEdit_Data->append(Time_str+" <-- "+Data+"(同步密码操作)");
    }
}

//票价保存
void MainWindow::IC_App_Ticket_Save()
{
    QByteArray Send_Data;
    QString Ticket_Save[5];
    Ticket_Save[0] = "AF";
    Ticket_Save[1] = " 02 0C ";

    QString Ticket_Save2_H,Ticket_Save2_L;
    if(ui->IC_lineEdit_App_Ticket->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","票价不能为空！");
    }
    else if(ui->IC_lineEdit_App_Ticket->text().toInt(0,10) > 65535)
    {
        QMessageBox::warning(this,"提示","设置的票价请不要超过65535元！");
    }
    else
    {
        Ticket_Int = ui->IC_lineEdit_App_Ticket->text().toInt(0,10);
        Ticket_Save[2] = QString::number(Ticket_Int,16);
        if(Ticket_Int == 0)
        {
            Ticket_Save[2] = "00 00";
        }
        else if(Ticket_Int <= 255 && Ticket_Int > 0)
        {
            Ticket_Save2_H = "00 ";
            Ticket_Save2_L = Ticket_Save[2].mid(0,2);
            if(Ticket_Save2_L.toInt(0,16) <= 15)
                Ticket_Save2_L = "0"+Ticket_Save2_L;
            Ticket_Save[2] = Ticket_Save2_H+Ticket_Save2_L;
        }
        else if(Ticket_Int <= 4095 && Ticket_Int > 255)
        {

            Ticket_Save2_H = "0"+Ticket_Save[2].mid(0,1);
            Ticket_Save2_L = " "+Ticket_Save[2].mid(1,2);
            Ticket_Save[2] = Ticket_Save2_H+Ticket_Save2_L;
        }
        else
        {
            Ticket_Save2_H = Ticket_Save[2].mid(0,2);
            if(Ticket_Save2_H.toInt(0,16) <= 15)
                Ticket_Save2_H = "0"+Ticket_Save2_H;
            Ticket_Save2_L = " "+Ticket_Save[2].mid(2,2);
            Ticket_Save[2] = Ticket_Save2_H+Ticket_Save2_L;
        }
        QString Data = Ticket_Save[1]+Ticket_Save[2];
        char Data_Hex = 0;
        for(int a=1; a<Data.length(); a+=3)
        {
            int Data_Int = Data.mid(a,2).toInt(0,16);               //将组合数据分解
            Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
        }
        QString Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
        int Data_Fcs_Int = Data_Fcs.toInt(0,16);
        if(Data_Fcs_Int <= 15)
            Data_Fcs = '0'+Data_Fcs;
        Data = Ticket_Save[0]+Data+" "+Data_Fcs;
        StringToHex(Data,Send_Data);
        Serial.write(Send_Data);
        Send_Num += Serial.bytesToWrite();
        ui->label_Send_Num->setText(QString::number(Send_Num));
        if(Show_Mode != 2 && !IsStop && IsOpen)
            ui->textEdit_Data->append(Time_str+" <-- "+Data.toUpper()+"(修改票价操作)");
        IC_App_Down_Ok = false;
    }
}

//发送余额
void MainWindow::IC_App_Send_Balance()
{
    QByteArray Send_Data;
    QString BaLance[5];
    BaLance[0] = "AF";
    BaLance[1] = " 03 0B ";
    QString BaLance2_S,BaLance2_H,BaLance2_L;
    BaLance[2] = QString::number(Balance_Int,16);
    if(Balance_Int == 0)
    {
        BaLance[2] = "00 00 00";
    }
    else if(Balance_Int <= 255 && Balance_Int > 0)
    {
        BaLance2_H = "00 00 ";
        BaLance2_L = BaLance[2].mid(0,2);
        if(BaLance2_L.toInt(0,16) <= 15)
            BaLance2_L = "0"+BaLance2_L;
        BaLance[2] = BaLance2_H+BaLance2_L;
    }
    else if(Balance_Int <= 4095 && Balance_Int > 255)
    {
        BaLance2_H = "00 0"+BaLance[2].mid(0,1);
        BaLance2_L = " "+BaLance[2].mid(1,2);
        BaLance[2] = BaLance2_H+BaLance2_L;
    }
    else if(Balance_Int <= 65535 && Balance_Int > 4095)
    {
        BaLance2_H = "00 "+BaLance[2].mid(0,2);
        BaLance2_L = " "+BaLance[2].mid(2,2);
        BaLance[2] = BaLance2_H+BaLance2_L;
    }
    else if(Balance_Int <= 1048575 && Balance_Int > 65535)
    {
        BaLance2_S = "0"+BaLance[2].mid(0,1);
        BaLance2_H = " "+BaLance[2].mid(1,2);
        BaLance2_L = " "+BaLance[2].mid(3,2);
        BaLance[2] = BaLance2_S+BaLance2_H+BaLance2_L;
    }
    else
    {
        BaLance2_S = BaLance[2].mid(0,2);
        BaLance2_H = " "+BaLance[2].mid(2,2);
        BaLance2_L = " "+BaLance[2].mid(4,2);
        BaLance[2] = BaLance2_S+BaLance2_H+BaLance2_L;
    }
    QString Data = BaLance[1]+BaLance[2];

    char Data_Hex = 0;
    for(int a=1; a<Data.length(); a+=3)
    {
        int Data_Int = Data.mid(a,2).toInt(0,16);               //将组合数据分解
        Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
    }
    QString Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
    int Data_Fcs_Int = Data_Fcs.toInt(0,16);
    if(Data_Fcs_Int <= 15)
        Data_Fcs = '0'+Data_Fcs;
    Data = BaLance[0]+Data+" "+Data_Fcs;
    StringToHex(Data,Send_Data);
    Serial.write(Send_Data);
    Send_Num += Serial.bytesToWrite();
    ui->label_Send_Num->setText(QString::number(Send_Num));
    if(Show_Mode != 2 && !IsStop && IsOpen)
        ui->textEdit_Data->append(Time_str+" <-- "+Data.toUpper()+"(修改余额操作)");
}

//数据复位
void MainWindow::IC_Ticket_Reset()
{
    gross = 0;
    ui->IC_label_Head_Money->setFont(QFont("",15,QFont::Bold));
    ui->IC_label_Head_Money->setText("0");
}

//充值
void MainWindow::IC_App_Up_Money()
{
    unsigned long Up_Balance;
    if(ui->IC_lineEdit_App_Up_Money->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","充值金额不能为空！");
    }
    else
    {
        Up_Balance = ui->IC_lineEdit_App_Up_Money->text().toInt(nullptr,10);
        if(Up_Balance+Balance_Int > 16777215)
        {
            QMessageBox::warning(this,"提示","最大余额不能超过16777215元！");
        }
        else
        {
            Balance_Int += Up_Balance;
            IC_App_Send_Balance();
            IC_App_Down_Ok = false;
            IC_IsUp = true;
        }
    }
}

//扣费
void MainWindow::IC_App_Down_Money()
{
    if(!IC_Bus_Mode)
    {
        if(Balance_Int-Ticket_Int < 0)
        {
            IC_App_Down_Ok = true;
            QMessageBox::warning(this,"提示","余额不足！请充值！");
        }
        else
        {
            Balance_Int -= Ticket_Int;
            IC_App_Send_Balance();
            IC_IsUp = false;
        }
    }
}

void MainWindow::on_IC_comboBox_Sys_Lump_activated(int index)
{
    // index 表示那个块
    switch(index){
    case 0:
        if(ui->IC_comboBox_Sys_Region->currentIndex()!=0)       // 不是0扇区0块就使能
            ui->IC_pushButton_Sys_Write->setEnabled(true);
        else  ui->IC_pushButton_Sys_Write->setEnabled(false);
        break;
    case 3:
        ui->IC_pushButton_Sys_Write->setEnabled(false);
        break;
    default:
        ui->IC_pushButton_Sys_Write->setEnabled(true);
        break;
    }
}

void MainWindow::on_IC_comboBox_Sys_Region_activated(int index)
{
    // index 表示那个扇区
    switch(index){
    case 0:
        if(ui->IC_comboBox_Sys_Lump->currentIndex()==0 ||
           ui->IC_comboBox_Sys_Lump->currentIndex()==3)
            ui->IC_pushButton_Sys_Write->setEnabled(false);
        else  ui->IC_pushButton_Sys_Write->setEnabled(true);
        break;
    default:
        if(ui->IC_comboBox_Sys_Lump->currentIndex()==3)
            ui->IC_pushButton_Sys_Write->setEnabled(false);
        else {
            ui->IC_pushButton_Sys_Write->setEnabled(true);
        }
        break;
    }
}

void MainWindow::on_IC_radioButton_Pass_B_clicked(bool checked)
{
    if(checked)
    {
        ui->IC_lineEdit_Pass_Last->setEnabled(false);
        ui->IC_lineEdit_Pass_Last->setText("FFFFFFFFFFFF");
        ui->IC_lineEdit_Pass_New->setEnabled(false);
        ui->IC_lineEdit_Pass_New->setText("FFFFFFFFFFFF");
    }
    else
        return;
}

void MainWindow::on_IC_radioButton_Pass_A_clicked(bool checked)
{
    if(checked){
        ui->IC_lineEdit_Pass_Last->setEnabled(true);
        ui->IC_lineEdit_Pass_New->setEnabled(true);
        ui->IC_pushButton_Pass_Save->setEnabled(true);
    }
    else
        return;
}


/**
* @funcname      get_Data
* @brief         按要求处理串口数据（根据数据长度）
* @param         lastData-要处理的数据/lastLen-数据长度位置/addLen-附加长度
* @return        cutData-处理后的数据/"error"-数据残缺
* @author        Zhoucj
* @date          2018-08-27
***************************************************************/
QString MainWindow::get_Data(QString lastData, unsigned char lastLen, unsigned char addLen)
{
    unsigned char dataNum = 0;
    QString cutData = "";
    if(lastData.length() >= (lastLen-1)*3)
    {
        dataNum = lastData.mid((lastLen-1) * 3, 2).toInt(nullptr, 16);
        if(lastData.length() >= (lastLen + dataNum + addLen) * 3 - 1)
        {
            cutData = lastData.mid(0, (lastLen + dataNum + addLen) * 3 - 1);
            return cutData;
        }
        else
            return "error";
    }
    else
        return "error";
}

/***************************************************************
* @funcname      Uart_DataHandle
* @brief         数据处理函数
* @param
* @return        无
* @author        Zhoucj
* @date          2018-08-27
***************************************************************/
void MainWindow::Uart_DataHandle(unsigned char lastLen, unsigned char addLen)
{
    static QString Uart_DataBuff;
    QString HandleData = "";
    if(Uart_DataBuff.isEmpty())
        Uart_DataBuff += RecData;
    else
        Uart_DataBuff += " " + RecData;
    HandleData = get_Data(Uart_DataBuff, lastLen, addLen);
    if(HandleData != "error")
    {
        if(Mode == 1)
            ID_Data = HandleData;
        else if(Mode == 2)
            IC_Data = HandleData;
        else if(Mode == 3)
            ETC_Data = HandleData;
        Uart_DataBuff = Uart_DataBuff.remove(HandleData);
    }
    else
    {
        HandleData.clear();
    }
    Uart_DataBuff = Uart_DataBuff.simplified();
}


/**
* @funcname      on_IC_lineEdit_Sys_Write_textChanged
* @brief         IC自动加空格操作
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-08-28
***************************************************************/
void MainWindow::on_IC_lineEdit_Sys_Write_textChanged(const QString &arg1)
{
    static unsigned char space_number = 0;
    unsigned int Next_Hex_Cursor;
    unsigned int Last_Cursor;
    Last_Cursor = ui->IC_lineEdit_Sys_Write->cursorPosition();
    unsigned char strLen = arg1.length();
    strLen -= space_number;
    Next_Hex_Cursor = ui->IC_lineEdit_Sys_Write->cursorPosition();
    if(Last_Cursor != Next_Hex_Cursor)
    {
        ui->IC_lineEdit_Sys_Write->setCursorPosition(Last_Cursor);
    }
    //自动添加空格操作
    if((strLen%2 == 0) && (space_number < strLen/2) && strLen > 0)
    {
        ui->IC_lineEdit_Sys_Write->insert(" ");
    }
    space_number = ui->IC_lineEdit_Sys_Write->text().count(' ');
}



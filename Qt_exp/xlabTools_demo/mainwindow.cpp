﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 建立串口 */
    Serial.reset(new QSerialPort());
    connect(Serial.data(),SIGNAL(readyRead()),SLOT(serial_read()));
    connect(Serial.data(),SIGNAL(error(QSerialPort::SerialPortError)),
            SLOT(serial_error(QSerialPort::SerialPortError)));

    init_app_ui();
    init_app_timer();
    zigbee_app_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********************** UI状态更新相关函数 ***********************/
// 初始化 APP 的界面 ui
void MainWindow::init_app_ui()
{
    // 当前通讯类型
    m_rf_type="zigbee";

    // 主窗口 图标
    this->setWindowIcon(QIcon(""));

    // zigbee 的数据模拟栏
    ui->send_cycle_cBox->setChecked(false);
    ui->SendTime_ledit->setVisible(false);
    ui->SendTime_unit_label->setVisible(false);

    // 数据显示设置
    ui->comboBox_Set_Rec->setCurrentIndex(1);
    ui->comboBox_Set_Send->setCurrentIndex(1);

    // 首次刷新
    serial_get_availablePorts();
    get_time();
    serial_ui_update();

    // 输入限制
    Regexp_Na_Hex.setPattern("^([0-9a-fA-F][0-9a-fA-F])(\\ [0-9a-fA-F][0-9a-fA-F])$");
    Regexp_Data_Ascii.setPattern("[a-zA-Z0-9?=_,.{} ]{1,250}");     //当输入模式为Ascii时限制数据长度最大为FF
    Regexp_Data_Hex.setPattern("[0-9a-fA-F ]{1,749}");              //当输入模式为Hex时限制数据长度最大为FF
    Panid_RegExp.setPattern("(\\d){4}|1[0-6][0-3][0-8][0-3]");
    ui->lineEdit_Panid->setValidator(new QRegExpValidator(Panid_RegExp,this));
    ui->lineEdit_SendNa->setValidator(new QRegExpValidator(Regexp_Na_Hex,ui->lineEdit_SendNa));
    ui->SendData_ledit->setValidator(new QRegExpValidator(Regexp_Data_Ascii,ui->SendData_ledit));
}

// 初始化所有定时器
void MainWindow::init_app_timer()
{
    /** 200ms 周期 **/
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),
            this,SLOT(serial_get_availablePorts()));                // 读取一次串口

    timer->start(200);

    /** 1000ms 周期 **/
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_time()));         // 每1s读取一次当前时间
    timer->start(1000);
}

// 串口 ui 更新
void MainWindow::serial_ui_update()
{
    if(Serial->isOpen() == false)
    {
        /* 进行关闭串口后的显示 */
        ui->pushButton_OpenCom->setText("打开串口");                  //按键字变化
        ui->label_State->setText("打开串口");
        ui->comboBox_Com->setEnabled(true);
        ui->comboBox_Baud->setEnabled(true);
    }
    else
    {
        /* 进行打开串口后的显示 */
        ui->pushButton_OpenCom->setText("关闭串口");                   //按键文字变化
        ui->label_State->setText("关闭串口");
        ui->comboBox_Com->setEnabled(false);
        ui->comboBox_Baud->setEnabled(false);
    }
}

// 获取时间
void MainWindow::get_time()
{
    m_time = QTime::currentTime();
    m_Time_str = m_time.toString("hh:mm:ss");
}

/** 串口管理接口函数 **/
// 获取当前电脑可用串口 并进行 状态判断
void MainWindow::serial_get_availablePorts(void)
{
    static unsigned char Last_count = 0;
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
    int count = serialPortInfo.count();                     //有效串口的个数
    if(Serial->isOpen() == false)
    {
        /* 串口关闭 */
        if(count != Last_count)                             //避免出现串口刷新后自动改变
        {
            ui->comboBox_Com->clear();                         //清除下拉选择框中的内容
            for(int i=0;i<count;i++){
                // 添加 复选框 的项目
                ui->comboBox_Com->addItem(serialPortInfo.at(i).portName());
            }
            ui->comboBox_Com->setCurrentText("COM4");
            ui->comboBox_Baud->setCurrentText("38400");       // 如果有115200的 选项，就选择这个为默认。
            Last_count = static_cast<unsigned char>(count);
        }
    }
    else
    {
        /* 串口打开 */
        // 判断 已经打开 的串口 是否正常连接
        for(int i=0; i<count; i++)
        {
            if(Serial->portName() == serialPortInfo.at(i).portName()) return;
        }
        serial_close();
    }
}

// 打开串口
bool MainWindow::serial_open()
{
    bool ret;
    //获取当前选择的串口号和波特率
    QString _Com = ui->comboBox_Com->currentText();
    QString _Baud = ui->comboBox_Baud->currentText();

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

    ret=Serial->open(QSerialPort::ReadWrite);   // 打开串口并设置为读写模式
    serial_ui_update();
    if(!ret)    // 提示 打开失败原因
        QMessageBox::warning(this,"Connection Error",Serial->errorString());
    else{
        zigbee_read_config();
    }
    return ret;
}

// 关闭串口
void MainWindow::serial_close()
{
    Serial->close();
    serial_ui_update();
}

// 串口 写
// data: 串口写的数据，去掉空格的字节数据  经过StringToHex函数转换而来
// display_string: 显示字符
// notes: 数据的备注【可选】
void MainWindow::serial_write(QByteArray &data,QString display_string,QString notes)
{
    Serial->write(data);
    current_cmd=display_string;
    m_Send_Num += Serial->bytesToWrite();

    // 显示 信息
    if(!display_string.isEmpty()){
        ui->label_SendNum->setText(QString::number(m_Send_Num));
        display_string.remove("\r");display_string.remove("\n");
        QString info=QString("[%1 %2]<--%3").arg(m_Time_str).arg(m_rf_type).arg(display_string);
        ui->info_listWidget->addItem(info+notes);
        ui->info_listWidget->scrollToBottom();
    }
}

// 串口 读
int MainWindow::serial_read()
{
    QByteArray m_rawdata;                       // 串口接收数据
    QString m_hexdata_space;                    // 串口接收数据（带空格hex格式字符串形式）
    // 获取数据
    m_Rec_Num += Serial->bytesAvailable();
    ui->label_RecNum->setText(QString::number(m_Rec_Num));

    m_rawdata = Serial->readAll();                          // 读取串口的数据
    m_hexdata_space = m_rawdata.toHex().toUpper();          // toHex(" ").toUpper();
    m_hexdata_space = Add_Space(0,m_hexdata_space);         // 为了适配5.5.1的aarch编译器

    // 数据处理
    zigbee_data_handle(m_rawdata,m_hexdata_space);
    return 0;
}

void MainWindow::serial_error(QSerialPort::SerialPortError serialPortError)
{
    switch (serialPortError) {
        case QSerialPort::ResourceError:
        serial_ui_update();
        zigbee_cmd_reset();
        break;
        default:
        break;
    }
}

/*********************** 字符处理功能 ***********************/
// 功能:将带有空格的字符串（两个字符表示一个hex字节数据） 计算字符串长度
// 参数:str 要计算的字符串
int MainWindow::Get_DelSpaceString_Length(QString str)
{
    str.remove(" ");
    return str.length()/2;
}

// 功能:添加空格
// 参数:x--字符串开始位置/z--字符串
QString MainWindow::Add_Space(int x, QString z)
{
    QString ss;
    // i索引
    for(int i=x; i<z.length(); i+=2)
    {
        QString aa = z.mid(i,2);
        if(i<z.length()-2)      // 最后两个字符不需要添加空格
            ss += aa+" ";
        else
            ss += aa;
    }
    return ss.toUpper();
}

// 功能：异或校验计算
// 参数：s1：数组中计算的起始索引 s2：数组中计算的结束索引
// 返回：异或计算结果
char MainWindow::xor_count(QByteArray array,unsigned char s1,unsigned char s2)
{
  char i,check_temp;
  check_temp = array[s1];
  for(i = static_cast<char>(s1+1);i<(s2+1);i++)
  {
    check_temp ^= array[i];									                    //异或校验
  }
  return check_temp;
}

// 将带有空格字符串 按照 16进制 进行储存
// 例如："5f 47 87" 转换为："0x5f,0x47,0x87"
// 正常情况字符串中间带有空格
// str-需要转换的字符串，带有空格  send_data-16进制 数组
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
        if(hstr == ' ') continue;           // 跳过空格
        memcpy(_str,c_string+i,2);          // 取 需要转换的 两个字节
        send_data[convert]=static_cast<char>(strtol(_str, nullptr, 16));    // 将两个字节字符串 用 16进制单字节数 储存
        i++;                                // 跳过 低位
        convert++;                          // 成功次数
    }
    send_data.resize(convert);
}

/*********************** zigbee 相关功能函数 ***********************/
void MainWindow::zigbee_app_init()
{
    zigbee_cycle_timer = new QTimer(this);
    connect(zigbee_cycle_timer,SIGNAL(timeout()),
            this,SLOT(zigbee_read_config_handle()));
    connect(zigbee_cycle_timer,SIGNAL(timeout()),
            this,SLOT(zigbee_write_config_handle()));
    zigbee_cycle_timer->start(100);

    zigbee_res_timer = new QTimer(this);
    zigbee_res_timer->setSingleShot(true);
    connect(zigbee_res_timer,SIGNAL(timeout()),
            this,SLOT(zigbee_cmd_timeout()));
    // 进行一次复位
    zigbee_cmd_reset();
}

// zigbee 更新 ui 相关
void MainWindow::zigbee_ui_state_update(){
    /** 读写状态 **/
    if((zigbee_flag & BIT_0) || (zigbee_flag & BIT_1)){
        /* 正在进行读写 */
        ui->pushButton_Read->setEnabled(false);
        ui->pushButton_Write->setEnabled(false);
        ui->send_data_btn->setEnabled(false);
    }else{
        ui->pushButton_Read->setEnabled(true);
        ui->pushButton_Write->setEnabled(true);
        ui->send_data_btn->setEnabled(true);
    }
}

// zigbee 指令状态复位
void MainWindow::zigbee_cmd_reset(bool update_ui)
{
    // 标志位清零
    zigbee_flag=0;

    // 读配置计数清零
    zigbee_count=0;

    // 清理所有信号量
    while(zigbee_Sem.available()){
        zigbee_Sem.tryAcquire();
    }

    // 定时器复位
    zigbee_res_timer->stop();

    // ui 更新
    if(update_ui)
        zigbee_ui_state_update();

    // 清理接收数据
    CLEAN_RECV_DATA;
    ZIGBEE_RECV_CMD_OK;
}

// zigbee 读取配置
// 获取一次类型
void MainWindow::zigbee_read_config()
{
    if(Serial->isOpen()){
        zigbee_cmd_reset();
        zigbee_flag |= BIT_0;
        zigbee_ui_state_update();
        ZIGBEE_RES_TIMER_START;
    }
}

// 发送相关读配置命令 的周期函数
void MainWindow::zigbee_read_config_handle()
{
    /* 如果不需要 读取 配置退出 */
    if(!(zigbee_flag & BIT_0)) return;

    /* 没有信号量 不发送命令 */
    if(!zigbee_Sem.tryAcquire()) return;

    /* 测试 是否 支持AT指令 */
    if(!(zigbee_flag & BIT_2)){
        QString Send_Data = "ATE0\r\n";
        QByteArray byte_arr= Send_Data.toLatin1();
        serial_write(byte_arr,Send_Data);
        ZIGBEE_RES_TIMER_START;      // 激活响应定时器
        return;
    }

    /* 开始读取配置 */
    if( zigbee_at_flag ){
        /* 支持 AT 指令 */
        QString cmd;
        switch(zigbee_count++)
        {
            case 0: cmd = "AT+MAC?\r\n"; break;
            case 1: cmd = "AT+LOGICALTYPE?\r\n"; break;
            case 2: cmd = "AT+CHANNEL?\r\n"; break;
            case 3: cmd = "AT+PANID?\r\n"; break;
            default:zigbee_cmd_reset();return;          // 自动结束一个查询时序
        }
        QByteArray byte_arr= cmd.toLatin1();
        serial_write(byte_arr,cmd);
        ZIGBEE_CMD_HAVE_DATA;
        ZIGBEE_RES_TIMER_START;                         // 重新激活响应定时器
    }else{
        /* 不支持 AT 指令 */
        QString cmd;
        switch(zigbee_count++)
        {
            //查询协调器mac地址
            // FE 07 帧头和长度
            // 29 00 上位机发送数据到协调器
            // 02 固定
            // 00 00 协调器网络地址:00 00
            // 01 02 网络地址查询 MAC 地址
            // 00 00 要查询的网络地址
            case 0: cmd = "FE 07 29 00 02 00 00 01 02 00 00 2F"; break;
            //{TYPE=?,PANID=?,CHANNEL=?}
            case 1: cmd = "FE 1F 29 00 02 00 00 00 00 7B 54 59 50 45 3D 3F 2C 50 41 4E 49 44 3D 3F 2C 43 48 41 4E 4E 45 4C 3D 3F 7D 39"; break;
            default:zigbee_cmd_reset();return;          // 自动结束一个查询时序
        }
        QByteArray byte_arr;
        StringToHex(cmd,byte_arr);
        serial_write(byte_arr,cmd);
        ZIGBEE_CMD_HAVE_DATA;
        ZIGBEE_RES_TIMER_START;                         // 重新激活响应定时器
    }
}

// 串口命令的返回情况1
// 串口数据处理
void MainWindow::zigbee_data_handle(QByteArray& data,QString& display_data)
{
#define  CMD_ERROR          else if(recv_data_whole.contains("ERR: Bad command")){ \
                                QString cmd_flag="ERR: Bad command"; \
                                QString info1=QString("[%1 %2]-->%3"). \
                                        arg(m_Time_str).arg(m_rf_type).arg(cmd_flag); \
                                INFO_LISTWIDGET_UPDATE(info1); \
                                zigbee_cmd_reset(); \
                            }

    bool ok;
    recv_raw_whole+=data;
    recv_data_whole+=QString(data);
    if(recv_show_whole.isEmpty())
        recv_show_whole+=QString(display_data);
    else
        recv_show_whole+=" "+QString(display_data);
//    qDebug()<< "recv_raw_whole:" << recv_raw_whole;
//    qDebug()<< "recv_data_whole:" << recv_data_whole;
//    qDebug()<< "recv_show_whole:" << recv_show_whole;

    /* 长度检查 */
    if(recv_data_whole.length()>RECV_CMD_MAX_LEN)
        zigbee_cmd_reset();

    /** 处理主动上报的显示 **/

    /** 测试 是否 支持AT指令 **/
    if(!(zigbee_flag & BIT_2) && current_cmd.contains("ATE0")){
        if(recv_data_whole.contains("OK\r\n")){
            /* 表示支持AT指令 */
            zigbee_flag |= BIT_2;       // 必须激活标记，否则出现bug
            zigbee_flag |= BIT_3;
            zigbee_at_flag=true;

            // 更新信息
            QString info=QString("[%1 %2]-->%3")
                    .arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // ERR:Bad command
        CMD_ERROR;
        return;
    }

    /** 这里表示AT测试已经完成 **/
    if(zigbee_at_flag){
        /** 支持AT **/
        // 这个命令是否有数据帧
        if(!(zigbee_flag & BIT_6)){
            if(recv_data_whole.contains("OK\r\n")){
                // 更新信息
                QString info=QString("[%1 %2]-->%3")
                        .arg(m_Time_str).arg(m_rf_type).arg("OK");
                INFO_LISTWIDGET_UPDATE(info);
                CLEAN_RECV_DATA;
                ZIGBEE_RECV_CMD_OK;
            }
            // ERR:Bad command
            CMD_ERROR;
            return; // 其他数据不关心
        }
        // "AT+MAC?\r\n" 回复 查询节点的mac地址（路由和终端）
        if(recv_data_whole.contains("+MAC") &&
                recv_data_whole.contains("OK\r\n")){
            QString cmd_flag="+MAC:";
            int index=recv_data_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_data_whole.mid(index);
            int return_index = vaild_recv_data.indexOf("\r\n");
            // 获取信息
            QString mac=vaild_recv_data.mid(cmd_flag.length(),
                                            return_index-cmd_flag.length());
            ui->lineEdit_Add->setText(mac);

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag+mac);
            QString info2=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info1);
            INFO_LISTWIDGET_UPDATE(info2);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // "AT+LOGICALTYPE?\r\n" 回复
        else if(recv_data_whole.contains("+LOGICALTYPE") &&
                recv_data_whole.contains("OK\r\n")){
            // "+LOGICALTYPE:2\r\nOK\r\n"
            QString cmd_flag="+LOGICALTYPE:";
            int index=recv_data_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_data_whole.mid(index);
            int return_index = vaild_recv_data.indexOf("\r\n");

            // 获取信息
            QString type=vaild_recv_data.mid(cmd_flag.length(),
                                             return_index-cmd_flag.length());
            int type_num=type.toInt();
            ui->comboBox_Mold->setCurrentIndex(type_num);

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag+type);
            QString info2=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info1);
            INFO_LISTWIDGET_UPDATE(info2);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // "AT+CHANNEL?\r\n" 回复
        else if(recv_data_whole.contains("+CHANNEL") &&
                recv_data_whole.contains("OK\r\n")){
            // "+CHANNEL:20\r\nOK\r\n"
            QString cmd_flag="+CHANNEL:";
            int index=recv_data_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_data_whole.mid(index);
            int return_index = vaild_recv_data.indexOf("\r\n");

            // 获取信息
            QString channel=vaild_recv_data.mid(cmd_flag.length(),
                                                return_index-cmd_flag.length());
            ui->comboBox_Channel->setCurrentText(channel);

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag+channel);
            QString info2=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info1);
            INFO_LISTWIDGET_UPDATE(info2);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // "AT+PANID?\r\n" 回复
        else if(recv_data_whole.contains("+PANID") &&
                recv_data_whole.contains("OK\r\n")){
            // "+PANID:8199\r\nOK\r\n"
            QString cmd_flag="+PANID:";
            int index=recv_data_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_data_whole.mid(index);
            int return_index = vaild_recv_data.indexOf("\r\n");

            // 获取信息
            QString pandid=vaild_recv_data.mid(cmd_flag.length(),
                                               return_index-cmd_flag.length());
            ui->lineEdit_Panid->setText(pandid);

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag+pandid);
            QString info2=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info1);
            INFO_LISTWIDGET_UPDATE(info2);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // "AT+RESET?\r\n" 回复
        else if(recv_data_whole.contains("+HW:CC2530") &&
                recv_data_whole.contains("+RDY")){
            QString cmd_flag1="+HW:CC2530";
            QString cmd_flag2="+RDY";

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag1);
            QString info2=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag2);
            INFO_LISTWIDGET_UPDATE(info1);
            INFO_LISTWIDGET_UPDATE(info2);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        // ERR:Bad command
        CMD_ERROR;

        // 其他的指令 自动超时复位
    }
    else{
        /** 不支持AT **/
        // 正确的指令响应帧
        if(recv_show_whole.contains("FE 01 69 00 00 68")){
            // "FE 01 69 00 00 68" 正确的指令响应帧
            QString cmd_flag="FE 01 69 00 00 68";
            int index=recv_show_whole.indexOf(cmd_flag);
            // 删除前面的无效字符和这个响应帧
            recv_show_whole=recv_show_whole.mid(index+cmd_flag.length());
            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag);
            INFO_LISTWIDGET_UPDATE(info1);
            // 这个命令是否有数据帧
            if(!(zigbee_flag & BIT_6)){
                CLEAN_RECV_DATA;
                ZIGBEE_RECV_CMD_OK;
                return; // 其他数据不关心
            }
        }
        // 错误的指令响应帧
        else if(recv_show_whole.contains("FE 01 69 00 01 68")){
            zigbee_cmd_reset();
            qDebug()<< "ZIGBEE:指令出现错误";
            return; // 其他数据不关心
        }
        // 获取MAC地址
        if(recv_show_whole.contains("FE 0E 69 80 00 00 01 02")){
            // 类似格式："FE 0E 69 80 00 00 01 02 00 00 00 12 4B 00 1B D8 8D E6 15"
            QString cmd_flag="FE 0E 69 80 00 00 01 02";
            int index=recv_show_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_show_whole.mid(index);
            // 获取信息
            int data_len=vaild_recv_data.mid(3,2).toInt(&ok,16);        // 数据帧长度
            int vaild_len=Get_DelSpaceString_Length(vaild_recv_data);   // 当前接收指令去空格长度
            if(data_len==vaild_len-5){
                /* 数据获取完整，其他情况指令超时自动复位 */
                // 根据获取的数据计算校验
                QString xor_data_frame = vaild_recv_data.mid(3*1,vaild_recv_data.length()-6);
                QByteArray xor_frame_arr;
                StringToHex(xor_data_frame,xor_frame_arr);
                char _xor=xor_count(xor_frame_arr,0,static_cast<unsigned char>(xor_frame_arr.length()-1));
                // 获取校验
                QString xor_frame=vaild_recv_data.mid(vaild_recv_data.length()-2);
                char r_xor=static_cast<char>(xor_frame.toInt(&ok,16));
                if(_xor==r_xor){
                    /* 数据校验正常 */
                    // 获取数据帧
                    QString data_frame=vaild_recv_data.mid(4*3,data_len*3-1);
                    QString mac=data_frame.mid(6*3);
                    mac.replace(" ",":");
                    ui->lineEdit_Add->setText(mac);

                    // 更新信息
                    QString info1=QString("[%1 %2]-->%3").
                            arg(m_Time_str).arg(m_rf_type).arg(vaild_recv_data);
                    INFO_LISTWIDGET_UPDATE(info1);
                    CLEAN_RECV_DATA;
                    ZIGBEE_RECV_CMD_OK;
                }
            }
        }
        // 获取 TYPE PANID CHANNEL
        else if(recv_raw_whole.contains("{TYPE=") &&
                recv_raw_whole.contains("}") &&
                recv_raw_whole.contains("PANID=") &&
                recv_raw_whole.contains("CHANNEL=")){
            // 类似格式：
            // recv_raw_whole:"\xFE\x01i\x00\x00h\xFE&i\x80\x00\x00\x00\x00{TYPE=10000,PANID=8199,CHANNEL=20}\xC7"
            // recv_show_whole:"FE 26 69 80 00 00 00 00 7B 54 59 50 45 3D 31 30 30 30 30 2C 50 41 4E 49 44 3D 38 31 39 39 2C 43 48 41 4E 4E 45 4C 3D 32 30 7D C7"
            /* 使用 QString 进行数据长度判断 */
            QString cmd_flag="FE 26 69 80 00 00 00 00";
            int index=recv_show_whole.indexOf(cmd_flag);
            // 删除前面的无效字符
            QString vaild_recv_data=recv_show_whole.mid(index);
            // 获取信息
            int data_len=vaild_recv_data.mid(3,2).toInt(&ok,16);        // 数据帧长度
            int vaild_len=Get_DelSpaceString_Length(vaild_recv_data);   // 当前接收指令去空格长度
            if(data_len!=vaild_len-5) return;

            /* 使用 QByteArray 进行数据获取 */
            int index1=recv_raw_whole.indexOf("{");
            int index2=recv_raw_whole.indexOf("}");
            if(index1==-1 && index2==-1) return;
            if(index1 > index2) return;
            QByteArray data_frame=recv_raw_whole.mid(index1,index2-index1+1);
            // 删除“{}”
            data_frame.remove(0,1);
            data_frame.remove(data_frame.length()-1,1);
            QList<QByteArray> data_list=data_frame.split(',');
            if(data_list.length()<3) return;
            QByteArray panid_pair=data_list[1];
            QByteArray channel_pair=data_list[2];
            QString panid = panid_pair.replace("PANID=","");
            QString channel = channel_pair.replace("CHANNEL=","");
            ui->lineEdit_Panid->setText(panid);
            ui->comboBox_Channel->setCurrentText(channel);
            ui->comboBox_Mold->setCurrentIndex(0);

            // 更新信息
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(vaild_recv_data);
            INFO_LISTWIDGET_UPDATE(info1);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
    }
}

// 串口命令的返回情况2
// 读取配置超时
void MainWindow::zigbee_cmd_timeout()
{
    /* 测试 是否 支持AT指令 */
    if(!(zigbee_flag & BIT_2) && current_cmd.contains("ATE0")){
        /* 表示不支持AT指令 */
        zigbee_flag |= BIT_2;       // 必须激活标记，否则出现bug
        zigbee_flag &= ~BIT_3;
        zigbee_at_flag=false;
        CLEAN_RECV_DATA;
        ZIGBEE_RECV_CMD_OK;
        return;
    }

    /* 这里AT测试完成 读配置超时 */
    zigbee_cmd_reset();
}

// zigbee 写入配置
void MainWindow::zigbee_write_config()
{
    if(Serial->isOpen()){
        zigbee_cmd_reset();
        zigbee_flag |= BIT_1;
        zigbee_ui_state_update();
        ZIGBEE_RES_TIMER_START;
    }
}

// zigbee 写配置处理函数
void MainWindow::zigbee_write_config_handle()
{
    /* 如果不需要 读取 配置退出 */
    if(!(zigbee_flag & BIT_1)) return;

    /* 没有信号量 不发送命令 */
    if(!zigbee_Sem.tryAcquire()) return;

    /* 开始写入配置 */
    if( zigbee_at_flag ){
        /* 支持 AT 指令 */
        QString cmd;
        switch(zigbee_count++)
        {
            case 0: {
                int type=ui->comboBox_Mold->currentIndex();
                cmd = QString("AT+LOGICALTYPE=%1\r\n").arg(type);
                ZIGBEE_CMD_NO_DATA;
                break;
            }
            case 1: {
                QString channel=ui->comboBox_Channel->currentText();
                cmd = QString("AT+CHANNEL=%1\r\n").arg(channel);
                ZIGBEE_CMD_NO_DATA;
                break;
            }
            case 2: {
                QString Panid=ui->lineEdit_Panid->text();
                cmd = QString("AT+PANID=%1\r\n").arg(Panid);
                ZIGBEE_CMD_NO_DATA;
                break;
            }
            case 3: {
                cmd = "AT+RESET\r\n";
                ZIGBEE_CMD_HAVE_DATA;
                break;
            }
            case 4:{
                zigbee_cmd_reset(false);
                // 需要一定时间延时，否则指令错误
                if(Serial->isOpen())
                    QTimer::singleShot(1000,this,SLOT(zigbee_read_config()));
                break;
            }
            default: zigbee_cmd_reset(); return;
        }
        QByteArray byte_arr= cmd.toLatin1();
        serial_write(byte_arr,cmd);
        ZIGBEE_RES_TIMER_START;                         // 重新激活响应定时器
    }else{
        /* 不支持 AT 指令 */
        // FE 28 29 00 02 00 00 00 00 7B 50 41 4E 49 44 3D 38 31 39
        // 30 2C 4E 4F 44 45 5F 54 59 50 45 3D 30 2C 43 48 41 4E 4E 45 4C 3D 32 30 7D 5c
        QString cmd;
        switch(zigbee_count++)
        {
            case 1: {
                QString panid=ui->lineEdit_Panid->text();
                int node=ui->comboBox_Mold->currentIndex();
                QString channel=ui->comboBox_Channel->currentText();
//                QString app_data=QString("{PANID=%1,NODE_TYPE=%2,CHANNEL=%3}").arg().arg()


                ZIGBEE_CMD_NO_DATA;
                break;
            }
            case 2:{
                zigbee_cmd_reset(false);
                // 需要一定时间延时，否则指令错误
                if(Serial->isOpen())
                    QTimer::singleShot(1000,this,SLOT(zigbee_read_config()));
                break;
            }
            default: zigbee_cmd_reset(); return;
        }
        QByteArray byte_arr= cmd.toLatin1();
        serial_write(byte_arr,cmd);
        ZIGBEE_RES_TIMER_START;                         // 重新激活响应定时器                     // 重新激活响应定时器
    }
}


/*********************** ui槽函数 ***********************/
/** 软件设置 组box **/
// 打开串口按钮
void MainWindow::on_pushButton_OpenCom_clicked()
{
    if(Serial->isOpen() == true)
    {
        serial_close();
    }
    else
    {
        serial_open();
    }
}

// 打开接收 按钮
void MainWindow::on_pushButton_OpenRec_clicked()
{
    m_display_recv=!m_display_recv;
    // 更新状态
    if(m_display_recv){
        /* 处于打开显示接收数据 状态 */
        ui->pushButton_OpenRec->setText("停止接收");
        ui->label_State->setText("正在接收");
    }else{
        /* 处于关闭显示接收数据 状态 */
        ui->pushButton_OpenRec->setText("开始接收");                //按钮上文字变化
        ui->label_State->setText("接收停止");
    }
}

/** 配置 组box **/
/* zigbee */
// 读取 配置
void MainWindow::on_pushButton_Read_clicked()
{
    zigbee_read_config();
}

// 写入 配置
void MainWindow::on_pushButton_Write_clicked()
{
    if(zigbee_at_flag && ui->comboBox_Mold->currentIndex()==0){
        QMessageBox::warning(this,"类型错误","zigbee节点暂不支持修改为协调器");
        return;
    }
    zigbee_write_config();
}

/** 数据显示设置 组box **/



/** 数据清除 组box **/
// 清除数据记录 按钮
void MainWindow::on_pushButton_DelRec_clicked()
{
    ui->info_listWidget->clear();
    QList<QLineEdit *> QLineEditlist =
            ui->groupBox_data_analysis->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
    foreach(auto lineEdit, QLineEditlist)
    {
        lineEdit->clear();
    }
}

// 清除数据模拟 按钮
void MainWindow::on_pushButton_DelSend_clicked()
{

}

/** 数据模拟 组box **/
// 网络地址 行编辑器 自动添加空格
// 网络地址 行编辑器 只显示 hex格式，没有ascii格式
void MainWindow::on_lineEdit_SendNa_textEdited(const QString &arg1)
{
    QString new_text;
    // 记录的长度
    static int last_len=0;
    if(arg1.length()==2 && last_len==1){
        new_text=arg1+" ";
        ui->lineEdit_SendNa->setText(new_text);
    }
    // 记录此次长度，作为下一次的上一次
    if(last_len!=arg1.length()){
        last_len=arg1.length();
    }
}

// 网络地址 行编辑器 输入错误提示
// 网络地址 行编辑器 只显示 hex格式，没有ascii格式
void MainWindow::on_lineEdit_SendNa_inputRejected()
{
    /* hex格式 */
    int cursorPos=ui->lineEdit_SendNa->cursorPosition();    // 获取光标位置
    int h=ui->lineEdit_SendNa->height()/2;
    QPoint point=ui->lineEdit_SendNa->mapToGlobal(QPoint(cursorPos*10,h));  // 相对位置转换为全局位置
    QToolTip::showText(point,"hex格式时两个数值后跟随空格",this);

    // 添加几种情况正确补全
    if(ui->lineEdit_SendNa->displayText().length()==2){
        QString new_text=ui->lineEdit_SendNa->displayText();
        new_text=new_text+" ";
        ui->lineEdit_SendNa->setText(new_text);
    }
}

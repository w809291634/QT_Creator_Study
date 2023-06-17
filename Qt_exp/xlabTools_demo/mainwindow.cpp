#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Serial.reset(new QSerialPort());
    connect(Serial.data(),SIGNAL(readyRead()),SLOT(serial_read()));

    init_app_ui();
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

    // 首次刷新
    serial_get_availablePorts();
    serial_ui_update();

    // 输入限制
    Regexp_Na_Hex.setPattern("^([0-9a-fA-F][0-9a-fA-F])(\\ [0-9a-fA-F][0-9a-fA-F])$");
    Regexp_Data_Ascii.setPattern("[a-zA-Z0-9?=_,.{} ]{1,250}");     //当输入模式为Ascii时限制数据长度最大为FF
    Regexp_Data_Hex.setPattern("[0-9a-fA-F ]{1,749}");              //当输入模式为Hex时限制数据长度最大为FF
    ui->lineEdit_Panid->setValidator(new QIntValidator(1, 16383,ui->lineEdit_Panid));
    ui->lineEdit_SendNa->setValidator(new QRegExpValidator(Regexp_Na_Hex,ui->lineEdit_SendNa));
    ui->SendData_ledit->setValidator(new QRegExpValidator(Regexp_Data_Ascii,ui->SendData_ledit));

    /* 定时器 */
    // 200ms 周期
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),
            this,SLOT(serial_get_availablePorts()));                // 读取一次串口
    timer->start(200);

    // 1000ms 周期
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
        ui->statusBar->showMessage("打开串口");
        ui->comboBox_Com->setEnabled(true);
        ui->comboBox_Baud->setEnabled(true);
    }
    else
    {
        /* 进行打开串口后的显示 */
        ui->pushButton_OpenCom->setText("关闭串口");                   //按键文字变化
        ui->statusBar->showMessage("关闭串口");
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
    if(!ret)    // 提示 打开失败原因
        QMessageBox::warning(this,"Connection Error",Serial->errorString());
    serial_ui_update();
    return ret;
}

// 关闭串口
void MainWindow::serial_close()
{
    Serial->close();
    serial_ui_update();
}

// 串口 写
// display_string: 显示字符
// hex: 去掉空格的字节数据  经过StringToHex函数转换而来
// notes: 数据的备注
void MainWindow::serial_write(QByteArray &hex,QString display_string)
{
    Serial->write(hex);

    m_Send_Num += Serial->bytesToWrite();

    // 显示 信息
    if(!display_string.isEmpty()){
        ui->label_SendNum->setText(QString::number(m_Send_Num));
        display_string.remove("\r");display_string.remove("\n");
        QString info=QString("[%1 %2]<--%3").arg(m_Time_str).arg(m_rf_type).arg(display_string);
        ui->info_listWidget->addItem(info);
    }
}

// 串口 读
int MainWindow::serial_read()
{
    // 获取数据
    m_Rec_Num += Serial->bytesAvailable();
    ui->label_RecNum->setText(QString::number(m_Rec_Num));

    this->thread()->msleep(30);
    m_rawdata = Serial->readAll();                          // 读取串口的数据
    m_hexdata_space = m_rawdata.toHex().toUpper();          // toHex(" ").toUpper();
    m_hexdata_space = Add_Space(0,m_hexdata_space);         // 为了适配5.5.1的aarch编译器

    qDebug()<< m_rawdata;
    qDebug()<< m_hexdata_space;

    // "\xFE" m_rawdata
    // "FE" m_hexdata_space
    // "\x01i\x00\x00h\xFE&i\x80\x00\x00\x00\x00{TYPE=10000,PANID=8199,CHANNEL=20}\xC7"
    // "01 69 00 00 68 FE 26 69 80 00 00 00 00 7B 54 59 50 45 3D 31 30 30 30 30 2C 50 41 4E 49 44 3D 38 31 39 39 2C 43 48 41 4E 4E 45 4C 3D 32 30 7D C7"
//    display_string.remove("\r");display_string.remove("\n");
//    QString info=QString("[%1 %2]<--%3").arg(m_Time_str).arg(m_rf_type).arg(display_string);
    // 数据处理
    zigbee_data_handle();

    // 处理完成清除
    m_hexdata_space.clear();
    m_rawdata.clear();
    return 0;
}

/*********************** 字符处理功能 ***********************/
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

//ZigBee AT测试函数
// 协调器不支持 AT 指令集  所有的节点支持
void MainWindow::zigbee_get_type()
{
    QString Send_Data = "ATE0\r\n";
    QByteArray byte_arr= Send_Data.toLatin1();
    serial_write(byte_arr,Send_Data);



    //"FE 1F 29 00 02 00 00 00 00 7B 54 59 50 45 3D 3F 2C 50 41 4E 49 44 3D 3F 2C 43 48 41 4E 4E 45 4C 3D 3F 7D 39"
//    Serial.write(Send_Data.toStdString().c_str());
//    Get_Time();
//    Send_Data = "["+Time_str+"  ZigBee] <-- "+Send_Data.left(Send_Data.length()-2);
//    ui->listWidget_Rec->addItem(Send_Data);
//    SendData_Count += Serial.bytesToWrite();
//    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
//    ZigBee_AT_Test_OK = true;
//    Test_OK = true;
//    Read_Flag = 0;
//    QTimer::singleShot(1000,this,SLOT(ZigBee_Make_Type()));
//    QTimer::singleShot(3000,this,SLOT(Del_Read_Write_flag()));
}

// zigbee 读取配置
void MainWindow::zigbee_read_config()
{
    zigbee_get_type();
//    QByteArray Read;
//    QString str = "FE 1F 29 00 02 00 00 00 00 7B 54 59 50 45 3D 3F 2C 50 41 4E 49 44 3D 3F 2C 43 48 41 4E 4E 45 4C 3D 3F 7D 39";
//    StringToHex(str,Read);                                 //先将发送框的内容转换为Hex型
//    Serial->write(Read);



}

// zigbee 写入配置
void MainWindow::zigbee_write_config()
{

}

// 串口数据处理
void MainWindow::zigbee_data_handle()
{

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




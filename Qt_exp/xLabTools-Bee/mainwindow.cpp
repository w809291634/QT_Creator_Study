#include "mainwindow.h"
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

    // 设置图标
    this->setWindowIcon(QIcon(":/icon/xLabTools.ico"));
    this->setWindowTitle("xLabTools-Bee");

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
    on_pushButton_OpenRec_clicked();

    // 输入限制
    Regexp_Na_Hex.setPattern("^([0-9a-fA-F][0-9a-fA-F])(\\ [0-9a-fA-F][0-9a-fA-F])$");
    Regexp_Data_Ascii.setPattern("[a-zA-Z0-9?=_,.{} ]{1,250}");     //当输入模式为Ascii时限制数据长度最大为FF
    Regexp_Data_Hex.setPattern("^([0-9a-fA-F][0-9a-fA-F])(\\ [0-9a-fA-F][0-9a-fA-F])*");              //当输入模式为Hex时限制数据长度最大为FF
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
        ui->label_State->setText("关闭串口");
        ui->comboBox_Com->setEnabled(true);
        ui->comboBox_Baud->setEnabled(true);
    }
    else
    {
        /* 进行打开串口后的显示 */
        ui->pushButton_OpenCom->setText("关闭串口");                   //按键文字变化
        ui->label_State->setText("打开串口");
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

// 按键事件处理
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget *focusWidget = QApplication::focusWidget();
    if(focusWidget->inherits("QLineEdit")){
        if(focusWidget->objectName().isEmpty())return;
        // 网络框获取历史
        if(qobject_cast<QLineEdit *>(focusWidget)==ui->lineEdit_SendNa){
            if(event->key() == Qt::Key_Up){
                QString text = zigbee_na_list_get(0);
                if(!text.isEmpty())
                    ui->lineEdit_SendNa->setText(text);
            }
            if(event->key() == Qt::Key_Down){
                QString text = zigbee_na_list_get(1);
                if(!text.isEmpty())
                    ui->lineEdit_SendNa->setText(text);
            }
        }
        // 发送框获取历史
        else if(qobject_cast<QLineEdit *>(focusWidget)==ui->SendData_ledit){
            if(event->key() == Qt::Key_Up){
                QString text = zigbee_send_list_get(0);
                if(!text.isEmpty()){
                    if(ui->comboBox_Set_Send->currentIndex()==0)
                        text=AsciiString2HexQString(text);
                    ui->SendData_ledit->setText(text);
                }
            }
            if(event->key() == Qt::Key_Down){
                QString text = zigbee_send_list_get(1);
                if(!text.isEmpty()){
                    if(ui->comboBox_Set_Send->currentIndex()==0)
                        text=AsciiString2HexQString(text);
                    ui->SendData_ledit->setText(text);
                }
            }
        }
        zigbee_calculate_len_fcs();
    }
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
            ui->comboBox_Com->setCurrentText("COM13");
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

    zigbee_cmd_reset(true);
}

// 串口 写
// data: 串口写的数据，去掉空格的字节数据  经过StringToHex函数转换而来
// display_string: 显示字符
// notes: 数据的备注【可选】
void MainWindow::serial_write(QByteArray &data,QString display_string,QString notes)
{
    if(Serial->isOpen()){
        Serial->write(data);
        current_cmd=display_string;
        m_Send_Num += Serial->bytesToWrite();

        // 显示 信息
        if(!display_string.isEmpty()){
            ui->label_SendNum->setText(QString::number(m_Send_Num));
            display_string.remove("\r");display_string.remove("\n");
            QString info=QString("[%1 %2]<--%3").arg(m_Time_str).arg(m_rf_type).arg(display_string);
            ui->info_listWidget->addItem(info+notes);
            if(ui->info_listWidget->count()>INFO_LISTWIDGET_MAX_HISTORY_NUM)
                ui->info_listWidget->takeItem(0);
            ui->info_listWidget->scrollToBottom();
        }
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

// 串口错误状态信号
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
// 要求字符串数据正确
int MainWindow::Get_DelSpaceString_Length(QString str)
{
    str.remove(" ");
    if(str.length()%2!=0){
        qdebug << "长度错误" ;
        return 0;
    }
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
uint8_t MainWindow::xor_count_u8(QByteArray array,uint8_t s1,uint8_t s2)
{
  uint8_t i,check_temp;
  check_temp = static_cast<uint8_t>(array[s1]);
  for(i = s1+1;i<(s2+1);i++)
  {
    check_temp ^= array[i];									                    //异或校验
  }
  return check_temp;
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
    // 去掉首尾空格
    if(str.indexOf(" ")==0) str=str.mid(1);
    if(str.lastIndexOf(" ")==str.length()-1) str=str.mid(0,str.length()-1);

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

// HexQString 带有空格16进制字符串 例如："5f 47 87" 转换为 QString 显示字符串
QString MainWindow::HexQString2AsciiString(const QString &HexQString)
{
    QByteArray APP_DATA_ARR;
    QString qstring;
    StringToHex(HexQString,APP_DATA_ARR);
    for(int i=0;i<APP_DATA_ARR.length();i++){
        uint8_t _uchar=static_cast<uint8_t>(APP_DATA_ARR.at(i));
        ushort _u16=_uchar;
        qstring+=QString(_u16);
    }
    return qstring;
}

// QString 显示字符串 例如："0x005f,0x0047,0x0087" 转换为 HexQString 带有空格16进制字符串
QString MainWindow::AsciiString2HexQString(const QString &qString)
{
    QByteArray data=qString.toLatin1().toHex().toUpper();
    QString HexQString=Add_Space(0,data);
    return HexQString;
}

/*********************** zigbee 相关功能函数 ***********************/
/** zigbee ui状态 **/
// 初始化
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

    zigbee_send_timer = new QTimer(this);
    connect(zigbee_send_timer,SIGNAL(timeout()),
            this,SLOT(on_send_data_btn_clicked()));

    // 数据发送的ui初始化
    ui->lineEdit_SendSop->setText("FE");
    ui->lineEdit_SendCmd->setText("29 00 02");
    QStringList AppCmdlist;
    AppCmdlist<< "00 00--指示协调器此数据需要转发给节点的应用程序数据"
//              << "00 02--给节点发送数据"     //
              << "01 01--指示协调器根据给定mac地址查找网络地址"
              << "01 02--指示协调器根据给定的网络地址查找mac地址" ;
    ui->comboBox_SendAppCmd->addItems(AppCmdlist);
    ui->comboBox_SendAppCmd->setToolTip("00 00 指示协调器此数据需要转发给节点的应用程序数据\r\n"
//                                        "00 02 给节点发送数据\r\n"
                                        "01 01 指示协调器根据给定mac地址查找网络地址\r\n"
                                        "01 02 指示协调器根据给定的网络地址查找mac地址");

    // 工具提示
    ui->lineEdit_SendNa->setToolTip("1.固定hex格式,两个数组中间空格\r\n2.使用方向键‘up’或者‘down’获取历史\r\n3.数据正确回车保存");
    ui->SendData_ledit->setToolTip("1.输入hex格式时,两个数组中间空格\r\n2.使用方向键‘up’或者‘down’获取历史\r\n3.数据正确发送保存");
    ui->SendTime_ledit->setToolTip("最小周期200ms");

    // 进行一次复位
    zigbee_cmd_reset();
}

// zigbee 更新 ui 相关
void MainWindow::zigbee_ui_state_update()
{
    /** 串口打开时 **/
    if(Serial->isOpen()){
        if((zigbee_flag & BIT_0) || (zigbee_flag & BIT_1)){
            /* 正在进行读写 */
            // 相关按钮
            ui->pushButton_Read->setEnabled(false);
            ui->pushButton_Write->setEnabled(false);
            ui->send_data_btn->setEnabled(false);
            ui->send_cycle_cBox->setEnabled(false);

            // 配置选择
            ui->comboBox_Mold->setEnabled(false);
            ui->lineEdit_Panid->setEnabled(false);
            ui->comboBox_Channel->setEnabled(false);
        }else{
            ui->pushButton_Read->setEnabled(true);
            ui->pushButton_Write->setEnabled(true);
            ui->send_data_btn->setEnabled(true);
            ui->send_cycle_cBox->setEnabled(true);
        }

        /** 读结束完毕 进行节点和协调器类型 ui 更新 **/
        if(zigbee_flag & BIT_9){
            if(zigbee_at_flag){
                /* 节点 */
                ui->comboBox_Mold->setEnabled(true);
                ui->lineEdit_Panid->setEnabled(true);
                ui->comboBox_Channel->setEnabled(true);
                zigbee_set_node_label();
            }
            else {
                /* 协调器 */
                ui->comboBox_Mold->setEnabled(false);
                // 配置选择
                ui->lineEdit_Panid->setEnabled(true);
                ui->comboBox_Channel->setEnabled(true);

                zigbee_set_coordinator_label();
            }
            zigbee_send_list_add("{CD1=255}");
            zigbee_send_list_add("{OD1=255}");
        }
    }
    /** 串口关闭时 **/
    else{
        // 配置使能
        ui->comboBox_Mold->setEnabled(false);
        ui->lineEdit_Panid->setEnabled(false);
        ui->comboBox_Channel->setEnabled(false);
        // 按钮使能
        ui->pushButton_Read->setEnabled(false);
        ui->pushButton_Write->setEnabled(false);
        ui->send_data_btn->setEnabled(false);
        ui->send_cycle_cBox->setChecked(false);
        on_send_cycle_cBox_clicked(false);
        ui->send_cycle_cBox->setEnabled(false);

        // zigbee 信息清除
        ui->lineEdit_Add->clear();
        ui->lineEdit_Panid->clear();
        ui->comboBox_Mold->setCurrentIndex(0);
        ui->comboBox_Channel->setCurrentIndex(0);
    }
}

// 更新为node标签
void MainWindow::zigbee_set_node_label()
{
    /* 数据解析 */
    // 其他label进行隐藏
    QList<QLabel *> label_list =  ui->groupBox_data_analysis->findChildren<QLabel *>();
    foreach(auto label, label_list)
    {
        bool in=false;
        for(auto name:{"label_RecLen","label_RecAppData"}){
            if(label->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) label->hide();
    }

    // 其他编辑器进行隐藏
    QList<QLineEdit *> lEdit_list =  ui->groupBox_data_analysis->findChildren<QLineEdit *>();
    foreach(auto lEdit, lEdit_list)
    {
        bool in=false;
        for(auto name:{"lineEdit_RecLen","lineEdit_RecAppData"}){
            if(lEdit->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) lEdit->hide();
    }

    ui->label_RecLen->setText("数据长度");
    ui->label_RecAppData->setText("应用数据");

    /* 数据模拟 */
    // 其他label进行隐藏
    QList<QLabel *> label_list2 =  ui->groupBox_data_imitate->findChildren<QLabel *>();
    foreach(auto label, label_list2)
    {
        bool in=false;
        for(auto name:{"label_SendLen","label_SendData","SendTime_unit_label"}){
            if(label->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) label->hide();
    }

    // 其他编辑器进行隐藏
    QList<QLineEdit *> lEdit_list2 =  ui->groupBox_data_imitate->findChildren<QLineEdit *>();
    foreach(auto lEdit, lEdit_list2)
    {
        bool in=false;
        for(auto name:{"lineEdit_SendLen","SendData_ledit","SendTime_ledit"}){
            if(lEdit->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) lEdit->hide();
    }

    ui->comboBox_SendAppCmd->hide();
    ui->label_SendLen->setText("数据长度");
    ui->label_SendData->setText("应用数据");
}

// 更新为协调器标签
void MainWindow::zigbee_set_coordinator_label()
{
    /* 数据解析 */
    // 其他label进行显示
    QList<QLabel *> label_list =  ui->groupBox_data_analysis->findChildren<QLabel *>();
    foreach(auto label, label_list)
    {
        bool in=false;
        for(auto name:{"label_RecLen","label_RecAppData"}){
            if(label->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) label->show();
    }

    // 其他编辑器进行显示
    QList<QLineEdit *> lEdit_list =  ui->groupBox_data_analysis->findChildren<QLineEdit *>();
    foreach(auto lEdit, lEdit_list)
    {
        bool in=false;
        for(auto name:{"lineEdit_RecLen","lineEdit_RecAppData"}){
            if(lEdit->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) lEdit->show();
    }

    ui->label_RecLen->setText("LEN");
    ui->label_RecAppData->setText("APP_DATA");

    /* 数据模拟 */
    // 其他label进行显示
    QList<QLabel *> label_list2 =  ui->groupBox_data_imitate->findChildren<QLabel *>();
    foreach(auto label, label_list2)
    {
        bool in=false;
        for(auto name:{"label_SendLen","label_SendData","SendTime_unit_label"}){
            if(label->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) label->show();
    }

    // 其他编辑器进行显示
    QList<QLineEdit *> lEdit_list2 =  ui->groupBox_data_imitate->findChildren<QLineEdit *>();
    foreach(auto lEdit, lEdit_list2)
    {
        bool in=false;
        for(auto name:{"lineEdit_SendLen","SendData_ledit","SendTime_ledit"}){
            if(lEdit->objectName()==name){
                in=true;
                break;
            }
        }
        if(!in) lEdit->show();
    }

    ui->comboBox_SendAppCmd->show();
    ui->label_SendLen->setText("LEN");
    ui->label_SendData->setText("APP_DATA");
}

// 从网络地址列表中获取项目
QString MainWindow::zigbee_na_list_get(const int& flag)
{
    if(zigbee_na_list.empty())return "";
    switch (flag) {
        case 0:{
            // 向上取
            if(--zigbee_na_index<0)zigbee_na_index=0;
            return zigbee_na_list[zigbee_na_index];
        }
        case 1:{
            // 向下取
            if(++zigbee_na_index>=zigbee_na_list.length())zigbee_na_index=zigbee_na_list.length()-1;
            return zigbee_na_list[zigbee_na_index];
        }
        default: return "";
    }
}

// 在网络地址列表中添加项目
// item 为HexQString
void MainWindow::zigbee_na_list_add(const QString& item){
    if(item.length()!=5)return;
    if(!zigbee_na_list.contains(item)){
        zigbee_na_list.append(item);
        if(zigbee_na_list.length()>MAX_STORAGE_NUM)
            zigbee_na_list.removeAt(0);

        zigbee_na_index=zigbee_na_list.length()-1;
        ui->lineEdit_SendNa->setText(zigbee_na_list[zigbee_na_index]);
        zigbee_calculate_len_fcs();
    }
}

// 复位索引
void MainWindow::zigbee_na_list_index_reset(void){
    zigbee_na_index=zigbee_na_list.length()-1;
}

// 从发送数据列表中获取项目
QString MainWindow::zigbee_send_list_get(const int& flag)
{
    if(zigbee_send_list.empty())return "";
    switch (flag) {
        case 0:{
            // 向上取
            if(--zigbee_send_index<0)zigbee_send_index=0;
            return zigbee_send_list[zigbee_send_index];
        }
        case 1:{
            // 向下取
            if(++zigbee_send_index>=zigbee_send_list.length())zigbee_send_index=zigbee_send_list.length()-1;
            return zigbee_send_list[zigbee_send_index];
        }
        default: return "";
    }
}

// 在发送数据列表中添加项目
// item 为AsciiString
void MainWindow::zigbee_send_list_add(const QString& item){
    if(item.isEmpty())return;
    if(!zigbee_send_list.contains(item)){
        zigbee_send_list.append(item);
        // 超出删除 队首
        if(zigbee_send_list.length()>MAX_STORAGE_NUM)
            zigbee_send_list.removeAt(0);
        zigbee_send_index=zigbee_send_list.length()-1;

        QString text=zigbee_send_list[zigbee_send_index];
        if(ui->comboBox_Set_Send->currentIndex()==0)
            text=AsciiString2HexQString(text);
        ui->SendData_ledit->setText(text);
        zigbee_calculate_len_fcs();
    }
}

// 复位索引
void MainWindow::zigbee_send_list_index_reset(void){
    zigbee_send_index=zigbee_send_list.length()-1;
}

/** zigbee 读写和数据处理 **/

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
// at_test 是否进行at测试 1进行 0不进行
void MainWindow::zigbee_read_config(bool at_test)
{
    if(Serial->isOpen()){
        // 停止 自动发送
        ui->send_cycle_cBox->setChecked(false);
        on_send_cycle_cBox_clicked(false);

        zigbee_cmd_reset();
        zigbee_flag |= BIT_0;
        if(!at_test)
            zigbee_flag |= BIT_2;
        zigbee_ui_state_update();
        ZIGBEE_RES_TIMER_START;
    }
}

// 不需要进行AT测试
void MainWindow::zigbee_read_config_NoAtTest()
{
    zigbee_read_config(false);
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
            case 4: zigbee_flag|=BIT_9;zigbee_ui_state_update(); ZIGBEE_RECV_CMD_OK;break;
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
            case 2: zigbee_flag|=BIT_9;zigbee_ui_state_update(); ZIGBEE_RECV_CMD_OK;break;
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
    bool ok;
    recv_data_whole+=QString(data);
    if(recv_show_whole.isEmpty())
        recv_show_whole+=QString(display_data);
    else
        recv_show_whole+=" "+QString(display_data);
//    qdebug<< "recv_data_whole:" << recv_data_whole;
//    qdebug<< "recv_show_whole:" << recv_show_whole;

    /* 长度检查 */
    if(recv_data_whole.length()>RECV_CMD_MAX_LEN){
        qdebug<< "长度超出" ;
        zigbee_cmd_reset();
    }

    /** 测试 是否 支持AT指令 **/
    if(!(zigbee_flag & BIT_2) && current_cmd.contains("ATE0")){
        // 正常指令的回复
        if(recv_data_whole.contains("OK\r\n")){
            /* 表示支持AT指令 */
            zigbee_flag |= BIT_2;       // 必须激活标记
            zigbee_flag |= BIT_3;
            zigbee_at_flag=true;

            // 更新信息
            QString info=QString("[%1 %2]-->%3")
                    .arg(m_Time_str).arg(m_rf_type).arg("OK");
            INFO_LISTWIDGET_UPDATE(info);
            CLEAN_RECV_DATA;
            ZIGBEE_RECV_CMD_OK;
        }
        else if(recv_data_whole.contains("ERR: Bad command")){
            QString cmd_flag="ERR: Bad command";
            QString info1=QString("[%1 %2]-->%3").
                    arg(m_Time_str).arg(m_rf_type).arg(cmd_flag);
            INFO_LISTWIDGET_UPDATE(info1); \
            zigbee_cmd_reset();
        }
        return;
    }

    /** 这里表示AT测试已经完成 **/
    int count=0;
    if(zigbee_at_flag){
        /** 支持AT **/
#define ZIGBEE_ATCMD_PROCESSED      {int _index=recv_data_whole.indexOf("\r\n"); \
                                    if(_index==-1){ \
                                        qdebug << "recv_data_whole" << recv_data_whole; \
                                        qdebug << __LINE__ << "zigbee AT删除回车错误";\
                                        continue; \
                                    } \
                                    recv_data_whole=recv_data_whole.mid(_index+2); \
                                    continue;}                              // 一条指令帧处理完成
        /* 长度检查 */
        recv_show_whole.clear();    //这里不使用
        if(recv_data_whole.length()>RECV_CMD_MAX_LEN){
            qdebug<< "长度超出" ;
            zigbee_cmd_reset();
        }

        const QList<QString> cmd_res_with_ok
                = {"AT+MAC?\r\n","AT+LOGICALTYPE?\r\n","AT+CHANNEL?\r\n","AT+PANID?\r\n"};

        while(recv_data_whole.contains("\r\n") || recv_data_whole.contains(">"))
        {
            count++;if(count>5)
            {
                qdebug<< "zigbee AT命令 多次循环:" << recv_data_whole;
                ZIGBEE_ATCMD_PROCESSED;
                //return;
            }
            /** 处理发送 **/
            if(recv_data_whole.contains(">") && !node_send_data.isEmpty()){
                QByteArray send_cmd_arr;
                send_cmd_arr = node_send_data.toLatin1();
                serial_write(send_cmd_arr,node_send_data);

                node_send_data.clear();
                recv_data_whole.remove(">");            // 不关注有没有接收正常
            }

            /** 处理接收 **/
            int index=recv_data_whole.indexOf("\r\n");
            if(index==-1)return;
            QString vaild_cmd=recv_data_whole.mid(0,index);     // 去掉回车，取指令
            // qdebug << "vaild_cmd" << vaild_cmd;

            if(vaild_cmd.contains("OK")){
                // 更新信息
                QString info=QString("[%1 %2]-->%3")
                        .arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info);
                // 这个命令是否有数据帧
                if(!(zigbee_flag & BIT_6)){
                    ZIGBEE_RECV_CMD_OK;
                }
                // 处理特殊OK，表示命令结束
                if(cmd_res_with_ok.contains(current_cmd)){
                    ZIGBEE_RECV_CMD_OK;
                }
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "AT+MAC?\r\n" 回复 查询节点的mac地址（路由和终端）
            else if(vaild_cmd.contains("+MAC")){
                QString cmd_flag="+MAC:";
                // 获取信息
                QString mac=vaild_cmd.mid(cmd_flag.length());
                ui->lineEdit_Add->setText(mac);

                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "AT+LOGICALTYPE?\r\n" 回复
            else if(vaild_cmd.contains("+LOGICALTYPE")){
                // "+LOGICALTYPE:2\r\nOK\r\n"
                QString cmd_flag="+LOGICALTYPE:";

                // 获取信息
                QString type=vaild_cmd.mid(cmd_flag.length());
                int type_num=type.toInt();
                ui->comboBox_Mold->setCurrentIndex(type_num);

                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "AT+CHANNEL?\r\n" 回复
            else if(vaild_cmd.contains("+CHANNEL")){
                // "+CHANNEL:20\r\nOK\r\n"
                QString cmd_flag="+CHANNEL:";

                // 获取信息
                QString channel=vaild_cmd.mid(cmd_flag.length());
                ui->comboBox_Channel->setCurrentText(channel);

                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "AT+PANID?\r\n" 回复
            else if(vaild_cmd.contains("+PANID")){
                // "+PANID:8199\r\nOK\r\n"
                QString cmd_flag="+PANID:";

                // 获取信息
                QString pandid=vaild_cmd.mid(cmd_flag.length());
                ui->lineEdit_Panid->setText(pandid);

                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "AT+RESET?\r\n" 回复
            else if(vaild_cmd.contains("+HW:CC2530")){
                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            else if(vaild_cmd.contains("+RDY")){
                // 更新信息
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_RECV_CMD_OK;
                ZIGBEE_ATCMD_PROCESSED;
            }
            // ERR:Bad command
            else if(vaild_cmd.contains("ERR: Bad command")){
                qdebug << "ERR: Bad command";
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            else if(vaild_cmd.contains("+RECV:")){
                QString cmd_flag="+RECV:";
                int index=vaild_cmd.indexOf(cmd_flag);
                int len = vaild_cmd.mid(index+cmd_flag.length()).toInt();    // 数据长度

                index = recv_data_whole.indexOf(vaild_cmd);
                QString data_frame=recv_data_whole.mid(index+vaild_cmd.length()+2); // 加上回车,去掉+RECV:9\r\n 后的其他
                if(len > data_frame.length()) return;                       // 数据没有接收完整，下次进行

                QString data=data_frame.mid(0,len);
                if(data.at(0)!="{" || data.right(1)!="}"){
                    qdebug << "接收数据错误";
                    qdebug << "recv_data_whole:" << recv_data_whole;
                    qdebug << "data_frame:" << data_frame;
                    qdebug << "data:" << data;
                }
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(data);
                INFO_LISTWIDGET_UPDATE(info1);
                recv_data_whole=recv_data_whole.remove(data);               // 处理过数据，删除
                ZIGBEE_ATCMD_PROCESSED;
            }
            // "+SEND:<len>" 发送数据的回复
            else if(vaild_cmd.contains("+SEND:"))
            {
                QString info1=QString("[%1 %2]-->%3").
                        arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                INFO_LISTWIDGET_UPDATE(info1);
                ZIGBEE_ATCMD_PROCESSED;
            }
            // else if(){ ZIGBEE_ATCMD_PROCESSED;}
            else {
                qdebug<< "zigbee AT命令 没有处理指令："<< vaild_cmd;
                ZIGBEE_ATCMD_PROCESSED;        // 没有找到记录的类型
            }
        }
    }
    else{
        /** 不支持AT **/
#define ZIGBEE_CMD_PROCESSED   {int _index=recv_show_whole.indexOf("FE"); \
                                if(_index==-1){ \
                                    qdebug << "recv_show_whole:" << recv_show_whole; \
                                    qdebug << __LINE__ <<"zigbee HEX命令 删除FE错误";\
                                    continue; \
                                } \
                                recv_show_whole=recv_show_whole.mid(_index+2); \
                                continue;}                              // 一条指令帧处理完成

#define ZIGBEE_EXTRA_CMD       else{ \
                                    qdebug << "zigbee HEX命令 没有处理："<< vaild_cmd; \
                                    ZIGBEE_CMD_PROCESSED; \
                                }

        /* 长度检查 */
        recv_data_whole.clear();        // 到这里不再使用
        if(recv_show_whole.length()>RECV_CMD_MAX_LEN){
            qdebug<< "长度超出" ;
            zigbee_cmd_reset();
        }

        while(recv_show_whole.contains("FE")){
            count++;if(count>5){
                qdebug<< "zigbee HEX命令多次循环:" << recv_show_whole;
                ZIGBEE_CMD_PROCESSED;
                // return;
            }

            /* 进行指令完整校验 */
            int index=recv_show_whole.indexOf("FE");                    // 获取第一个出现 "FE" 帧头的位置
            // 仅保留FE开头的回复帧
            recv_show_whole=recv_show_whole.mid(index);
            // 等待通信指针长度完整
            int data_len=recv_show_whole.mid(3,2).toInt(&ok,16);        // 数据帧长度（LEN）,去除 SOP、LEN、CMD（2）、FCS
            int vaild_len=Get_DelSpaceString_Length(recv_show_whole);   // 当前接收指令去空格长度
            if(data_len > vaild_len-5) return;                          // 指令没有接收完整

            // 计算校验 满足要求
            QString recv_cmd=recv_show_whole.mid(0,(data_len+5)*3-1);   // 没有校验的完整指令
            QString xor_data_frame = recv_cmd.mid(3*1,recv_cmd.length()-6);
            // qdebug << "recv_cmd" << recv_cmd;
            // qdebug << "xor_data_frame" << xor_data_frame;
            QByteArray xor_frame_arr;
            StringToHex(xor_data_frame,xor_frame_arr);
            uint8_t _xor= xor_count_u8(xor_frame_arr,0,static_cast<uint8_t>(xor_frame_arr.length()-1));
            // 获取校验
            QString xor_frame=recv_cmd.mid(recv_cmd.length()-2);
            uint8_t r_xor=static_cast<uint8_t>(xor_frame.toInt(&ok,16));
            if(!ok){
                qdebug<< "zigbee HEX命令 进制转换错误";
                qdebug<< "xor_frame:" << xor_frame; qdebug<< "r_xor:" << r_xor;
                ZIGBEE_CMD_PROCESSED;
            }
            // 判断校验
            if(_xor==r_xor){
                // 接收的一条有效的完整指令
                QString vaild_cmd=recv_cmd;
                // qdebug << "vaild_cmd" << vaild_cmd;

                /* 进行指令信息判断 */
                QString CMD=vaild_cmd.mid(6,5);         // 命令帧
                // 29 00 上位机发送数据到协调器
                // 69 00 协调器接收到正确指令后的响应帧
                // 69 80 协调器发送数据到上位机

                /* 响应帧 */
                if( CMD=="69 00" ){
                    // 正确的指令响应帧
                    if(vaild_cmd.contains("FE 01 69 00 00 68")){
                        // "FE 01 69 00 00 68" 正确的指令响应帧
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);
                        // 这个命令是否有数据帧
                        if(!(zigbee_flag & BIT_6)){
                            ZIGBEE_RECV_CMD_OK;
                        }
                        ZIGBEE_CMD_PROCESSED;
                    }
                    // 错误的指令响应帧
                    else if(vaild_cmd.contains("FE 01 69 00 01 68")){
                        qdebug<< "ZIGBEE:指令出现错误";
                        ZIGBEE_CMD_PROCESSED;
                    }
                    ZIGBEE_EXTRA_CMD;
                }
                /* 协调器发送数据到上位机 */
                else if( CMD=="69 80" ){
                    QString NA=vaild_cmd.mid(12,5);         // 网络地址
                    QString APP_CMD=vaild_cmd.mid(18,5);    // 数据命令
                    zigbee_na_list_add(NA);
                    /* 协调器根据给定 MAC 地址，查询网络地址 */
                    if(APP_CMD=="01 01"){
                        // 类似格式："FE 0E 69 80 00 00 01 01 00 12 4B 00 15 D3 D3 21 8B 32 33"
                        // 获取数据帧
                        QString data_frame=vaild_cmd.mid(4*3,data_len*3-1);
                        QString na=data_frame.mid(12*3);

                        // 更新信息
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);
                        ZIGBEE_RECV_CMD_OK;
                        ZIGBEE_CMD_PROCESSED;
                    }
                    /* 协调器根据给定的网络地址，查询 MAC 地址 */
                    else if(APP_CMD=="01 02"){
                        // 类似格式："FE 0E 69 80 00 00 01 02 00 00 00 12 4B 00 1B D8 8D E6 15"
                        // 获取数据帧
                        QString data_frame=vaild_cmd.mid(4*3,data_len*3-1);
                        QString mac=data_frame.mid(6*3);
                        mac.replace(" ",":");
                        ui->lineEdit_Add->setText(mac);

                        // 更新信息
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);
                        ZIGBEE_RECV_CMD_OK;
                        ZIGBEE_CMD_PROCESSED;
                    }
                    /* 协调器与上位机之间数据的通信命令 */
                    else if(APP_CMD=="00 00"){
                        QString vaild_ascii_cmd=HexQString2AsciiString(vaild_cmd);
                         // qdebug << vaild_ascii_cmd;
                         // qdebug << vaild_cmd;
                        /* 返回 TYPE PANID CHANNEL */
                        if(vaild_ascii_cmd.contains("{TYPE=") &&
                                vaild_ascii_cmd.contains("}") &&
                                vaild_ascii_cmd.contains("PANID=") &&
                                vaild_ascii_cmd.contains("CHANNEL=")){
                            // 类似格式：
                            // recv_show_whole:"FE 26 69 80 00 00 00 00 7B 54 59 50 45 3D 31 30 30 30 30 2C 50 41 4E 49 44 3D 38 31 39 39 2C 43 48 41 4E 4E 45 4C 3D 32 30 7D C7"
                            /* 使用 QByteArray 进行数据获取 */
                            int index1=vaild_ascii_cmd.indexOf("{");
                            int index2=vaild_ascii_cmd.indexOf("}");
                            if(index1==-1 && index2==-1 && index1 > index2) {
                                qdebug<< "error index1"<< index1;
                                qdebug<< "error index2"<< index2;
                                return;
                            }
                            QString data_frame=vaild_ascii_cmd.mid(index1,index2-index1+1);
                            // 删除“{}”
                            data_frame.remove(0,1);
                            data_frame.remove(data_frame.length()-1,1);
                            QList<QString> data_list=data_frame.split(',');
                            if(data_list.length()<3) return;
                            QString panid_pair=data_list[1];
                            QString channel_pair=data_list[2];
                            QString panid = panid_pair.remove("PANID=");
                            QString channel = channel_pair.remove("CHANNEL=");

                            ui->lineEdit_Panid->setText(panid);
                            ui->comboBox_Channel->setCurrentText(channel);
                            ui->comboBox_Mold->setCurrentIndex(0);

                            // 更新信息
                            QString info1=QString("[%1 %2]-->%3").
                                    arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                            INFO_LISTWIDGET_UPDATE(info1);
                            ZIGBEE_RECV_CMD_OK;
                            ZIGBEE_CMD_PROCESSED;
                        }
                        /* 其他的信息 不进行处理 仅仅显示 */
                        else{
                            // 类似格式：FE 0A 69 80 4B 3D 00 00 7B 44 31 3D 30 7D EB
                            // 更新信息
                            QString info1=QString("[%1 %2]-->%3").
                                    arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                            INFO_LISTWIDGET_UPDATE(info1);
                            ZIGBEE_CMD_PROCESSED;
                        }
                    }
                    else if(APP_CMD=="00 02"){
                        // 更新信息
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);
                        ZIGBEE_CMD_PROCESSED;
                    }
                    else if(APP_CMD=="80 02"){
                        // 更新信息
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);
                        ZIGBEE_CMD_PROCESSED;
                    }
                    else{
                        qdebug << "APP_CMD" << APP_CMD;
                        qdebug << "zigbee HEX命令 没有处理："<< vaild_cmd;
                        ZIGBEE_CMD_PROCESSED;
                    }
                }
                // 特殊命令
                else if( CMD=="41 80" ){
                    QString APP_CMD=vaild_cmd.mid(12,5);    // 数据命令
                    if( APP_CMD=="02 02" ){
                        // 类似格式："FE 06 41 80 02 02 00 02 04 00 C1"
                        // 协调器接收手动复位命令成功的回复
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);

                        ZIGBEE_RECV_CMD_OK;
                        ZIGBEE_CMD_PROCESSED;
                    }
                    else if( APP_CMD=="01 02" ){
                        // 类似格式："FE 06 41 80 01 02 00 02 04 00 C2"
                        // 正常上电
                        QString info1=QString("[%1 %2]-->%3").
                                arg(m_Time_str).arg(m_rf_type).arg(vaild_cmd);
                        INFO_LISTWIDGET_UPDATE(info1);

                        ZIGBEE_RECV_CMD_OK;
                        ZIGBEE_CMD_PROCESSED;
                    }
                    ZIGBEE_EXTRA_CMD;
                }
//                else if(){ZIGBEE_CMD_PROCESSED;}          // 处理其他指令
                ZIGBEE_EXTRA_CMD;
            }else {
                qdebug<< "zigbee HEX命令校验错误";
                qdebug<< "recv_cmd:" << recv_cmd; qdebug<< "xor_data_frame:" << xor_data_frame;
                qdebug<< "xor_frame_arr:" << xor_frame_arr; qdebug<< "_xor:" << _xor;
                qdebug<< "xor_frame:" << xor_frame; qdebug<< "r_xor:" << r_xor;
                ZIGBEE_CMD_PROCESSED;
            }
        }
        //qdebug<< "recv_show_whole:" << recv_show_whole;
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
// 写入之前要求先读
void MainWindow::zigbee_write_config()
{
    if(Serial->isOpen()){
        // 停止 自动发送
        ui->send_cycle_cBox->setChecked(false);
        on_send_cycle_cBox_clicked(false);

        zigbee_cmd_reset();
        zigbee_flag |= BIT_1;
        zigbee_flag |= BIT_2;       // 默认表示不再关心AT测试，连接串口后已经进行测试
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
                ZIGBEE_RES_TIMER_START
                break;
            }
            case 1: {
                QString channel=ui->comboBox_Channel->currentText();
                cmd = QString("AT+CHANNEL=%1\r\n").arg(channel);
                ZIGBEE_CMD_NO_DATA;
                ZIGBEE_RES_TIMER_START
                break;
            }
            case 2: {
                QString Panid=ui->lineEdit_Panid->text();
                cmd = QString("AT+PANID=%1\r\n").arg(Panid);
                ZIGBEE_CMD_NO_DATA;
                ZIGBEE_RES_TIMER_START
                break;
            }
            case 3: {
                cmd = "AT+RESET\r\n";
                ZIGBEE_CMD_HAVE_DATA;
                ZIGBEE_RES_TIMER_START_2
                break;
            }
            case 4:{
                zigbee_cmd_reset(false);
                // 需要一定时间延时，否则指令错误
                if(Serial->isOpen())
                    QTimer::singleShot(1000,this,SLOT(zigbee_read_config_NoAtTest()));
                break;
            }
            default: zigbee_cmd_reset(); return;
        }
        QByteArray byte_arr= cmd.toLatin1();
        serial_write(byte_arr,cmd);
    }else{
        /* 不支持 AT 指令 */
        // FE 28 29 00 02 00 00 00 00
        // 7B 50 41 4E 49 44 3D 38 31 39 30 2C 4E 4F 44 45 5F 54 59 50 45 3D 30 2C 43 48 41 4E 4E 45 4C 3D 32 30 7D 5c
        QString cmd;
        switch(zigbee_count++)
        {
            case 0: {
                QString cmd_buf1="FE";
                QString cmd_buf2;                           // 一个字节的长度
                QString cmd_buf3="29 00";                   // 固定的命令
                QString cmd_buf4="02 00 00 00 00";          // 固定的数据头部
                QString cmd_buf5;                           // 校验

                // 处理数据
                QString panid=ui->lineEdit_Panid->text();
                int node=ui->comboBox_Mold->currentIndex();
                QString channel=ui->comboBox_Channel->currentText();
                QString app_data=QString("{PANID=%1,NODE_TYPE=%2,CHANNEL=%3}")
                        .arg(panid).arg(node).arg(channel);
                QByteArray app_data_arr=app_data.toLatin1().toHex().toUpper();
                QString app_data_hex=Add_Space(0,app_data_arr);
                cmd_buf4=cmd_buf4+app_data_hex;

                // 数据长度
                cmd_buf2=QString("%1").arg(app_data.length()+5,0,16);

                // 计算校验
                cmd = cmd_buf1+cmd_buf2+cmd_buf3+cmd_buf4;
                cmd.remove(" ");
                cmd=Add_Space(0,cmd);               // 差校验位
                QByteArray cmd_buf_arr;
                StringToHex(cmd,cmd_buf_arr);
                char _xor=xor_count(cmd_buf_arr,1,static_cast<unsigned char>(cmd_buf_arr.length()-1));
                char string[3]={0};
                sprintf(string,"%x",_xor);          // 按16进制转换
                cmd=cmd+' '+QString(string);
                ZIGBEE_CMD_NO_DATA;
                ZIGBEE_RES_TIMER_START;
                break;
            }
            case 1: {
                cmd="FE 01 01 00 00 00";
                ZIGBEE_CMD_NO_DATA;
                ZIGBEE_RES_TIMER_START_2
                break;
            }
            case 2:{
                zigbee_cmd_reset(false);
                // 需要一定时间延时，否则指令错误
                if(Serial->isOpen())
                    QTimer::singleShot(1000,this,SLOT(zigbee_read_config_NoAtTest()));
                ZIGBEE_RES_TIMER_START;
                break;
            }
            default: zigbee_cmd_reset(); return;
        }
        QByteArray byte_arr;
        StringToHex(cmd,byte_arr);
        serial_write(byte_arr,cmd);
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
#define CONVERSION_TEST
#ifdef CONVERSION_TEST
    static QString last_text;
#endif
// 分析列表信息
void MainWindow::on_info_listWidget_itemClicked(QListWidgetItem *item)
{
#define REC_APP_DATA_DISPLAY        if(ui->comboBox_Set_Rec->currentIndex()==0){  \
                                        ui->lineEdit_RecAppData->setText(APP_DATA_HEX);  \
                                    } \
                                    else {  \
                                        APP_DATA_ACSII=HexQString2AsciiString(APP_DATA_HEX);  \
                                        ui->lineEdit_RecAppData->setText(APP_DATA_ACSII);  \
                                    }
    bool ok=false;
    QList<QLineEdit *> QLineEditlist =
            ui->groupBox_data_analysis->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
    foreach(auto lineEdit, QLineEditlist) lineEdit->clear();

    QString item_str=item->text();
    int index=item_str.indexOf("FE");                    // 获取第一个出现 "FE" 帧头的位置
    if(index!=-1){
        QString APP_DATA_HEX;
        QString APP_DATA_ACSII;
        /** hex数据格式处理 **/
        // 仅保留FE开头的回复帧
        QString vaild_cmd=item_str.mid(index);
        // qdebug<< vaild_cmd;
        int i=0;

        QString SOP=vaild_cmd.mid(0,2);                 // 网络地址
        QString LEN=vaild_cmd.mid(3*(++i),2);           // 数据帧长度
        int data_len=LEN.toInt(&ok,16);                 // 数据帧长度（LEN）,去除 SOP、LEN、CMD（2）、FCS
        if(!ok){
            qdebug<< "进制转换错误" ;
            return;
        }
        QString CMD=vaild_cmd.mid(3*(++i),5);           // 命令帧
        i+=2;

        QString DATA_FRAME=vaild_cmd.mid(3*4,data_len*3-1); // 数据帧
        QString FCS=vaild_cmd.mid(vaild_cmd.length()-2);    // 校验

        ui->lineEdit_RecSop->setText(SOP);
        ui->lineEdit_RecLen->setText(LEN);
        ui->lineEdit_RecCmd->setText(CMD);
        ui->lineEdit_RecFcs->setText(FCS);

        // 29 00 上位机发送数据到协调器
        // 69 00 协调器接收到正确指令后的响应
        // 69 80 协调器发送数据到上位机
        if(CMD=="29 00"){
            /** 上位机发送数据到协调器 **/
            QString FLAG=vaild_cmd.mid(3*i,2);
            if(FLAG!="02")return;
            QString NA=vaild_cmd.mid(3*(++i),5);        // 网络地址 上位机发送数据时 多一个 02
            i+=2;
            QString APP_CMD=vaild_cmd.mid(3*i,5);       // 应用命令
            APP_DATA_HEX=DATA_FRAME.mid(3*4+3);         // 应用数据

            ui->lineEdit_RecNa->setText(NA);
            ui->lineEdit_RecAppCmd->setText(APP_CMD);
            REC_APP_DATA_DISPLAY;
        }
        else if(CMD=="69 00"){
            /** 协调器接收到正确指令后的响应 **/
            // "FE 01 69 00 00 68"
            APP_DATA_HEX=DATA_FRAME;
            REC_APP_DATA_DISPLAY;
        }
        else if(CMD=="69 80"){
            /** 协调器发送数据到上位机 **/
            // "FE 0E 69 80 00 00 01 02 00 00 00 12 4B 00 2A 5C 89 68 2A"
            QString NA=vaild_cmd.mid(3*i,5);            // 网络地址
            i+=2;
            QString APP_CMD=vaild_cmd.mid(3*i,5);       // 应用命令
            APP_DATA_HEX=DATA_FRAME.mid(3*4);           // 应用数据

            ui->lineEdit_RecNa->setText(NA);
            ui->lineEdit_RecAppCmd->setText(APP_CMD);
            REC_APP_DATA_DISPLAY;
        }
        else if(CMD=="41 80"){
            /** 手动复位 **/
            // FE 06 41 80 02 02 00 02 04 00 C1
            QString APP_CMD=vaild_cmd.mid(3*i,5);       // 应用命令
            APP_DATA_HEX=DATA_FRAME.mid(3*2);           // 应用数据
            ui->lineEdit_RecAppCmd->setText(APP_CMD);
            REC_APP_DATA_DISPLAY;
        }
        else{
            ui->lineEdit_RecNa->clear();
            ui->lineEdit_RecAppCmd->clear();
            ui->lineEdit_RecAppData->clear();
        }
#ifdef CONVERSION_TEST
        last_text=ui->lineEdit_RecAppData->text();
#endif
    }else{
        /** ascii数据格式处理 **/
        QString APP_DATA_HEX;
        QString APP_DATA_ACSII;

        int index=item_str.indexOf("{");
        if(index==-1)return;
        QString vaild_data=item_str.mid(index);
        if(vaild_data[0]=="{" && vaild_data.right(1)=="}"){
            int data_len=vaild_data.length();
            ui->lineEdit_RecLen->setText(QString::number(data_len));
            APP_DATA_HEX=AsciiString2HexQString(vaild_data);
            REC_APP_DATA_DISPLAY;
        }
    }
}

// 接收数据格式转换
void MainWindow::on_comboBox_Set_Rec_currentIndexChanged(int index)
{
    QString current_text;
    QString conversion_text;

#ifdef CONVERSION_TEST
#define CONVERSION_ERROR    if(current_text!=last_text && !last_text.isEmpty() && !current_text.isEmpty()){ \
                                qdebug << "转换错误"; \
                            }
#endif
    if(!index){
        // hex显示
        current_text=ui->lineEdit_RecAppData->text();
        conversion_text=AsciiString2HexQString(current_text);
        ui->lineEdit_RecAppData->setText(conversion_text);
#ifdef CONVERSION_TEST
        CONVERSION_ERROR
        last_text=ui->lineEdit_RecAppData->text();
#endif
    }else{
        // ascii显示
        current_text=ui->lineEdit_RecAppData->text();
        conversion_text=HexQString2AsciiString(current_text);
        ui->lineEdit_RecAppData->setText(conversion_text);
#ifdef CONVERSION_TEST
        CONVERSION_ERROR
        last_text=ui->lineEdit_RecAppData->text();
#endif
    }
}

// 发送数据格式转换
void MainWindow::on_comboBox_Set_Send_currentIndexChanged(int index)
{
    QString current_text;
    QString conversion_text;
    if(index){
        // ascii
        ui->SendData_ledit->setValidator(new QRegExpValidator(Regexp_Data_Ascii,ui->SendData_ledit));
        current_text=ui->SendData_ledit->text();
        conversion_text=HexQString2AsciiString(current_text);
        ui->SendData_ledit->setText(conversion_text);
    }else{
        // hex
        ui->SendData_ledit->setValidator(new QRegExpValidator(Regexp_Data_Hex,ui->SendData_ledit));
        current_text=ui->SendData_ledit->text();
        conversion_text=AsciiString2HexQString(current_text);
        ui->SendData_ledit->setText(conversion_text);
    }
}

/** 数据清除 组box **/
// 清除数据记录 按钮
void MainWindow::on_pushButton_DelRec_clicked()
{
    ui->info_listWidget->clear();
    QList<QLineEdit *> QLineEditlist =
            ui->groupBox_data_analysis->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
    foreach(auto lineEdit, QLineEditlist) lineEdit->clear();
}

// 清除数据模拟 按钮
void MainWindow::on_pushButton_DelSend_clicked()
{
    ui->SendData_ledit->clear();
    zigbee_send_list_index_reset();
}

/** 数据模拟 组box **/
// 计算长度和校验
void MainWindow::zigbee_calculate_len_fcs()
{
    QString LEN;    //hex
    QString CMD= ui->lineEdit_SendCmd->text();
    QString NA = "00 00";
    QString APP_CMD=ui->comboBox_SendAppCmd->currentText().mid(0,5);
    QString APP_DATA ="00";   //hex
    QString FCS;

    // APP_DATA 始终转换为hex格式
    if(ui->comboBox_Set_Send->currentIndex()==1){
        QString ascii_data=ui->SendData_ledit->displayText();
        APP_DATA=AsciiString2HexQString(ascii_data);
    }else {
        // 补足 APP_DATA , 不足使用0代替
        QString app_data=ui->SendData_ledit->displayText();
        int num;                    //hex 个数
        int space_num=app_data.count(' ');
        switch (app_data.length()) {
            case 0: num=0;break;
            case 1: num=1;break;
            case 2: num=1;break;
            default:{
                int last_index=app_data.lastIndexOf(' ');
                if(last_index!=-1 && app_data.lastIndexOf(' ')!=app_data.length()-1)
                    num=space_num+1;
                else num=space_num;
            }break;
        }
        // 根据 行编辑器中 hex个数进行补足
        for(int i=1;i<num;i++){
           APP_DATA+=" 00";
        }
        APP_DATA.replace(0,app_data.length(),app_data);
    }

    // 计算长度
    uint8_t int_len=static_cast<uint8_t>(Get_DelSpaceString_Length(APP_DATA)+5);
    char len[3]={0};
    sprintf(len,"%02X",int_len);
    LEN=QString(len);
    if(zigbee_at_flag){
        ui->lineEdit_SendLen->setText(QString::number(int_len-5)); //ascii
    }
    else
        ui->lineEdit_SendLen->setText(LEN);

    // 网络地址
    QString na = ui->lineEdit_SendNa->text();
    NA.replace(0,na.length(),na);

    // FCS
    QString xor_frame=LEN+CMD+NA+APP_CMD+APP_DATA;
    xor_frame.remove(' ');
    xor_frame=Add_Space(0,xor_frame);
    QByteArray xor_frame_arr;
    StringToHex(xor_frame,xor_frame_arr);
    uint8_t _xor= xor_count_u8(xor_frame_arr,0,static_cast<uint8_t>(xor_frame.length()-1));
    char temp[3]={0};
    sprintf(temp,"%02X",_xor);
    FCS=QString(temp);

    ui->lineEdit_SendFcs->setText(FCS);
}

// 网络地址 行编辑器 自动添加空格
// 网络地址 行编辑器 只显示 hex格式，没有ascii格式
void MainWindow::on_lineEdit_SendNa_textEdited(const QString &arg1)
{
    /** 补全空格 **/
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

    /** 计算长度和校验 **/
    zigbee_calculate_len_fcs();
    zigbee_na_list_index_reset();
}

// 网络地址 行编辑器 输入错误提示
// 网络地址 行编辑器 只显示 hex格式，没有ascii格式
void MainWindow::on_lineEdit_SendNa_inputRejected()
{
    /* hex格式 */
    int cursorPos=ui->lineEdit_SendNa->cursorPosition();    // 获取光标位置
    int h=ui->lineEdit_SendNa->height()/2;
    QPoint point=ui->lineEdit_SendNa->mapToGlobal(QPoint(cursorPos*10,h));  // 相对位置转换为全局位置
    QToolTip::showText(point,"1.固定hex格式,两个数组中间空格\r\n2.使用方向键‘up’或者‘down’获取历史\r\n3.数据正确回车保存",this);

    // 添加几种情况正确补全
    if(ui->lineEdit_SendNa->displayText().length()==2){
        QString new_text=ui->lineEdit_SendNa->displayText();
        new_text=new_text+" ";
        ui->lineEdit_SendNa->setText(new_text);
    }
}

// 回车储存 na信息
void MainWindow::on_lineEdit_SendNa_returnPressed()
{
    QString text=ui->lineEdit_SendNa->displayText();
    if(text.length()==5){
        zigbee_na_list_add(text);
    }
}

// 发送数据 行编辑器 自动添加空格
// 计算校验
void MainWindow::on_SendData_ledit_textEdited(const QString &arg1)
{
    /** 补全空格 **/
    if(ui->comboBox_Set_Send->currentIndex()==0){
        // 记录的长度
        static int last_len=0;
        // 允许删除
        QString displayText=ui->SendData_ledit->displayText();
        QChar last_char;
        if(displayText.length()>0)
             last_char=displayText.at(displayText.length()-1);
        else last_char=0;
        if(displayText.remove(' ').length()%2==0
                && last_char!=' ' && last_char!=QChar(0)
                && arg1.length()>last_len){
            QString new_text=arg1+" ";
            ui->SendData_ledit->setText(new_text);
        }
        // 记录此次长度，作为下一次的上一次
        last_len=ui->SendData_ledit->displayText().length();    // 重新获取长度
    }else{
    }
    /** 计算长度和校验 **/
    zigbee_calculate_len_fcs();
    zigbee_send_list_index_reset();
}

// 发送数据 行编辑器 拒绝输入
void MainWindow::on_SendData_ledit_inputRejected()
{
    if(ui->comboBox_Set_Send->currentIndex()==0){
        /* hex格式 */
        int h=ui->SendData_ledit->height()/2;
        QPoint point=ui->SendData_ledit->mapToGlobal(QPoint(100,h));  // 相对位置转换为全局位置
        QToolTip::showText(point,"1.hex格式,两个数组中间空格\r\n2.使用方向键‘up’或者‘down’获取历史\r\n3.数据正确发送保存",this);

        // 添加补空格的情况
        QString displayText=ui->SendData_ledit->displayText();
        QChar last_char;
        if(displayText.length()>0)
             last_char=displayText.at(displayText.length()-1);
        else last_char=QChar(0);
        if(displayText.remove(' ').length()%2==0 && last_char!=' ' && last_char!=QChar(0) ){
            QString new_text=ui->SendData_ledit->displayText();
            new_text=new_text+" ";
            ui->SendData_ledit->setText(new_text);
        }
    }else{
        /* ascii格式 */
    }
}

// 发送数据
void MainWindow::on_send_data_btn_clicked()
{
    if(zigbee_at_flag){
        /* at指令 */

            QString send_cmd = "AT+SEND=";
            if(ui->comboBox_Set_Send->currentIndex()==0){
                QString hex_data=ui->SendData_ledit->displayText();
                node_send_data=HexQString2AsciiString(hex_data);
            }else node_send_data=ui->SendData_ledit->displayText();
            send_cmd += ui->lineEdit_SendLen->text()+"\r\n";

            QByteArray send_cmd_arr;
            send_cmd_arr = send_cmd.toLatin1();
            serial_write(send_cmd_arr,send_cmd);

    }else{
        /* hex指令 */
        QString sop="FE";
        QString len= ui->lineEdit_SendLen->text();
        QString cmd= ui->lineEdit_SendCmd->text();
        QString na = ui->lineEdit_SendNa->text();
        QString app_cmd =  ui->comboBox_SendAppCmd->currentText().mid(0,5);
        QString app_data;
        QString fcs=ui->lineEdit_SendFcs->text();

        if(ui->comboBox_Set_Send->currentIndex()==1){
            QString ascii_data=ui->SendData_ledit->displayText();
            app_data=AsciiString2HexQString(ascii_data);
        }else app_data=ui->SendData_ledit->displayText();
        QString ascii_text;
        ascii_text = HexQString2AsciiString(app_data);
        zigbee_send_list_add(ascii_text);

        QString send_cmd=sop+len+cmd+na+app_cmd+app_data+fcs;
        send_cmd.remove(' ');
        send_cmd=Add_Space(0,send_cmd);

        QByteArray send_cmd_arr;
        StringToHex(send_cmd,send_cmd_arr);
        serial_write(send_cmd_arr,send_cmd);
    }
}

// 设置自动发送
void MainWindow::on_send_cycle_cBox_clicked(bool checked)
{
    bool ok;
    int cycle = ui->SendTime_ledit->displayText().toInt(&ok,10);
    if(cycle<ZIGBEE_MIN_SEND_CYCLE)cycle=ZIGBEE_MIN_SEND_CYCLE;
    if(checked){
        zigbee_send_timer->start(cycle);
        ui->SendTime_ledit->show();
        ui->SendTime_unit_label->show();
    }
    else {
        zigbee_send_timer->stop();
        ui->SendTime_ledit->hide();
        ui->SendTime_unit_label->hide();
    }
}

// 设置自动发送周期
void MainWindow::on_SendTime_ledit_textChanged(const QString &arg1)
{
    bool ok;
    int cycle = arg1.toInt(&ok,10);
    if(cycle<ZIGBEE_MIN_SEND_CYCLE)cycle=ZIGBEE_MIN_SEND_CYCLE;
    zigbee_send_timer->setInterval(cycle);
}

// 改变APP_CMD
void MainWindow::on_comboBox_SendAppCmd_currentIndexChanged(int index)
{
    if(index==0){
        ui->comboBox_Set_Rec->setCurrentIndex(1);
        ui->comboBox_Set_Send->setCurrentIndex(1);
    }else{
        ui->comboBox_Set_Rec->setCurrentIndex(0);
        ui->comboBox_Set_Send->setCurrentIndex(0);
    }
}

/** 其他 **/
// 清除发送计数
void MainWindow::on_count_num_clear_btn_clicked()
{
    m_Rec_Num=0;m_Send_Num=0;
    ui->label_RecNum->setText(QString::number(m_Rec_Num));
    ui->label_SendNum->setText(QString::number(m_Send_Num));
}



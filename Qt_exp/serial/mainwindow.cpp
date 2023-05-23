#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SHOW_SEND_DATA          (ui->show_mode_cbx->currentIndex()!=2)
#define SHOW_RECV_DATA          (ui->show_mode_cbx->currentIndex()!=1)
#define LABEL_CLEAR_DELAY       2000

// 只创建一个主窗口，所有初始化在构造函数内
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/RFIDDemo.ico"));
    Serial.reset(new QSerialPort());
    // 设置 串口 读数据 的处理函数
    connect(Serial.data(),SIGNAL(readyRead()),SLOT(serial_read()));
    init_ui();

    // 首次刷新
    get_com();
    get_time();
    serial_ui_update();

    // 输入限制
    QRegExp Regexp_Pass,Regexp_Sys,Regexp_ID_Num;
    Regexp_Pass.setPattern("[0-9a-fA-F]{1,12}");
    Regexp_Sys.setPattern("[0-9a-fA-F ]{1,47}");
    Regexp_ID_Num.setPattern("[0-9a-fA-F]{1,10}");                  // 10位的字符ID
    ui->ID_Write_lineEdit->setValidator(new QRegExpValidator(Regexp_ID_Num,ui->ID_Write_lineEdit));
    ui->old_pass_lineEdit->setValidator(new QRegExpValidator(Regexp_Pass,ui->old_pass_lineEdit));
    ui->new_pass_lineEdit->setValidator(new QRegExpValidator(Regexp_Pass,ui->new_pass_lineEdit));
    ui->pass_lineEdit->setValidator(new QRegExpValidator(Regexp_Pass,ui->pass_lineEdit));
    ui->sysblock_data_lineEdit->setValidator(new QRegExpValidator(Regexp_Sys,ui->sysblock_data_lineEdit));

    /* 定时器 */
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_com()));          // 每100ms读取一次串口
    timer->start(100);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_time()));         // 每1s读取一次当前时间
    connect(timer,SIGNAL(timeout()),this,SLOT(IC_Card_continuous_search()));
    timer->start(1000);

    clear_timer = new QTimer(this);
    clear_timer->setSingleShot(true);
    connect(clear_timer,SIGNAL(timeout()),this,SLOT(clear_text_label()));
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
    QStringList Stringlist;
    Stringlist<< "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->baud_cbbox->addItems(Stringlist);
    Stringlist.clear();
    ui->baud_cbbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    /* 初始化 调试界面 UI */
    m_show_debug_page = true;
    m_show_debug_data = true;
    ui->show_debug_btn->setText("》\n》\n》");
    ui->debug_data->show();

    Stringlist<< "显示所有数据" << "仅显示发送数据" << "仅显示接收数据";
    ui->show_mode_cbx->addItems(Stringlist);
    Stringlist.clear();
    ui->show_mode_cbx->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ui->debug_data->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    /* 初始化 IC卡 UI */
    ui->continuous_search_card->setCheckable(true);
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

//版本提示
void MainWindow::on_version_update_btn_clicked()
{
    QMessageBox::about(this,"版本更新","当前版本:V1.0."+ ver +"\n最新版本:V1.0."+ ver +"\n当前已是最新版本！");
}

// 获取时间
void MainWindow::get_time(void)
{
    QTime time = QTime::currentTime();
    m_Time_str = time.toString("hh:mm:ss");
    ui->time_label->setFont(QFont("",35,QFont::Bold));
    ui->time_label->setText(m_Time_str);
}

// 延时清理文本的函数
void MainWindow::clear_text_label()
{
    ui->IC_label_Data_State->setText("");
    ui->pass_state_label->setText("");
}

/******************* 串口 相关函数定义*******************/

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
        ui->ID_data_clear_btn->setEnabled(false);
        ui->ID_Write_lineEdit->setEnabled(false);
        // IC 页面
        ui->one_shot_search_card->setEnabled(false);
        ui->continuous_search_card->setEnabled(false);
        ui->pass_modify_btn->setEnabled(false);
        ui->sysblock_read_btn->setEnabled(false);
        ui->sysblock_write_btn->setEnabled(false);

    }
    else
    {
        /* 进行打开串口后的显示 */
        ui->serial_open_btn->setText("关闭串口");                   //按键文字变化
        ui->com_cbbox->setEnabled(false);
        ui->baud_cbbox->setEnabled(false);
        // ID 页面
        ui->ID_read_btn->setEnabled(true);
        if(ui->ID_T5557_rbtn->isChecked())
            ui->ID_write_btn->setEnabled(true);
        ui->ID_data_clear_btn->setEnabled(true);
        ui->ID_Write_lineEdit->setEnabled(true);
        // IC 页面
        ui->one_shot_search_card->setEnabled(true);
        ui->continuous_search_card->setEnabled(true);
        ui->pass_modify_btn->setEnabled(true);
        ui->sysblock_read_btn->setEnabled(true);
        ui->sysblock_write_btn->setEnabled(true);
        if(ui->block_cbox->currentIndex()==3 ||
            (ui->sector_cbox->currentIndex()==0 &&
             ui->block_cbox->currentIndex()==0))
                ui->sysblock_write_btn->setEnabled(false);




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
// str_data: 带有空格 的字符串 数据
// data: 去掉空格的字节数据  str_data经过StringToHex函数转换而来
// notes: 数据的备注
void MainWindow::serial_write(QString &str_data,QByteArray &data,QString notes)
{
    Serial->write(data);
    m_Send_Num += Serial->bytesToWrite();
    ui->label_Send_Num->setText(QString::number(m_Send_Num));
    if(m_show_debug_data && SHOW_SEND_DATA)
       ui->Debug_Data_textEdit->append(m_Time_str+" <-- "+ str_data + notes);
}

// 串口 读
int MainWindow::serial_read()
{
    QByteArray RecData_HEX;
    if(m_Com_Open)
        m_Rec_Num += Serial->bytesAvailable();
    this->thread()->msleep(30);
    RecData_HEX = Serial->readAll();                              // 读取串口的数据,单字节数组
    m_RecData = RecData_HEX.toHex(' ').toUpper();
    uart_data_handle(m_RecData,RecData_HEX);
    ui->label_Rec_Num->setText(QString::number(m_Rec_Num));
    return 0;
}

// 功能:添加空格
// 参数:x--字符串开始位置/z--字符串
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
        if(hstr == ' ') continue;
        memcpy(_str,c_string+i,2);          // 取 需要转换的 两个字节
        send_data[convert]=static_cast<char>(strtol(_str, nullptr, 16));    // 将两个字节字符串 用 16进制单字节数 储存
        i++;                                // 跳过 低位
        convert++;                          // 成功次数
    }
    send_data.resize(convert);
}

// 串口数据处理函数
// uart_data 串口读取的原始数据转换，空格分开
// uart_data_hex 串口读取的原始数据
void MainWindow::uart_data_handle(QString &uart_data,QByteArray &uart_data_hex)
{
    (void)uart_data_hex;
    static unsigned char IC_Sum = 0;
    switch(ui->stackedWidget->currentIndex()){
    /* ID 页 */
    case 0:
    {
        if(m_ID_flag & BIT_0){
            m_ID_flag &= ~BIT_0;
            /* 读ID命令 */
            if(uart_data == "CD DC 00 80 00 80")
            {
                // 失败
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(读ID卡号失败)");
            }
            else if(uart_data.mid(0, 14) == "CD DC 00 81 05")
            {
                // 成功
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(读ID卡号成功)");
                ui->ID_data_listWidget->addItem(m_Time_str+" <-- 读取到卡号为 "+uart_data.mid(14, 15).remove(" "));
            }
        }
        else if(m_ID_flag & BIT_1){
            m_ID_flag &= ~BIT_1;
            /* 写ID命令 */
            if(uart_data == "CD DC 00 80 00 80")
            {
                // 失败
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(写ID卡号失败)");
            }
            else if(uart_data.mid(0, 14) == "CD DC 00 81 05"){
                // 成功
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(写ID卡号成功)");
                ui->ID_data_listWidget->addItem(m_Time_str+" <-- 读取到卡号为 "+uart_data.mid(14, 15).remove(" "));
            }
        }
        ui->ID_data_listWidget->scrollToBottom();
    }break;

    /* IC 页 */
    case 1:
    {
        /* 寻卡指令 */
        if(m_IC_flag & BIT_0){
            m_IC_flag &= ~BIT_0;
            if(uart_data=="CD DC 00 80 00 80"){
                // 失败
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(13.56M寻卡失败)");
            }
            else if(uart_data.mid(0, 14)=="CD DC 00 81 04"){
                // 成功
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(13.56M寻卡成功)");

                // 储存 查找 到的 IC卡号码
                QString IC_Num = uart_data.mid(15,11);

                for(int i=0; i<static_cast<int>(ARRAY(IC_Card)); i++)
                {
                    if(IC_Num == IC_Card[i])return;
                }
                if(IC_Sum >= 30)
                {
                    QMessageBox::warning(this,"提示","超出最大卡号储存范围!");
                }
                else
                {
                    IC_Card[IC_Sum] = IC_Num;
                    ui->search_card_listWidget->addItem(IC_Num.remove(" "));
                    ui->search_card_listWidget->scrollToBottom();
                    IC_Sum++;
                }
            }
        }
        /* 读扇区指令 */
        else if(m_IC_flag & BIT_1){
            m_IC_flag &= ~BIT_1;
            if(uart_data=="CD DC 00 80 00 80"){
                // 失败
                ui->IC_label_Data_State->setText("读取失败");
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(读取数据失败)");
            }
            else if(uart_data.mid(0, 11)=="CD DC 00 81"){
                // 成功
                ui->IC_label_Data_State->setText("读取成功");
                ui->sysblock_listWidget->addItem("("+QString::number(uart_data.mid(16,2).toInt(nullptr,16),10)+
                                                    ","+QString::number(uart_data.mid(19,2).toInt(nullptr,16),10)+
                                                    ") "+uart_data.mid(21,47));
                ui->sysblock_listWidget->scrollToBottom();
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(读取数据成功)");
            }
            clear_timer->start(LABEL_CLEAR_DELAY);
        }
        /* 写扇区指令 */
        else if(m_IC_flag & BIT_2){
            m_IC_flag &= ~BIT_2;
            if(uart_data=="CD DC 00 80 00 80"){
                // 失败
                ui->IC_label_Data_State->setText("写入失败");
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(写入数据失败)");
            }
            else if(uart_data.mid(0, 11)=="CD DC 00 81"){
                // 成功
                ui->IC_label_Data_State->setText("写入成功");
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(写入数据成功)");
            }
            clear_timer->start(LABEL_CLEAR_DELAY);
        }
        /* 修改密码指令 */
        else if(m_IC_flag & BIT_3){
            m_IC_flag &= ~BIT_3;
            if(uart_data=="CD DC 00 80 00 80"){
                // 失败
                ui->pass_state_label->setText("修改失败");
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(修改密码失败)");
            }
            else if(uart_data.mid(0, 11)=="CD DC 00 81"){
                // 成功
                ui->pass_state_label->setText("修改成功");
                if(m_show_debug_data && SHOW_RECV_DATA)
                    ui->Debug_Data_textEdit->append(m_Time_str+" --> "+uart_data+"(修改密码成功)");

                if(ui->passA_rbtn->isChecked()){
                    QMessageBox::information(this,"提示","A密码修改成功,如果遗忘,\n可以通过修改B密码来重置A密码为0");
                }else{
                    QMessageBox::information(this,"提示","A密码重置为0");
                }
            }
            clear_timer->start(LABEL_CLEAR_DELAY);
        }
    }break;
    }
}

/******************** 调试 界面 函数 ********************/

// 显示 调试 界面
void MainWindow::on_show_debug_btn_clicked()
{
    m_show_debug_page=!m_show_debug_page;
    if(m_show_debug_page)
    {
        /* 显示 调试窗口 */
        ui->show_debug_btn->setText("》\n》\n》");
        ui->debug_data->show();
    }
    else
    {
        /* 隐藏 调试窗口 */
        ui->show_debug_btn->setText("《\n《\n《");
        ui->debug_data->hide();
    }
}

// 调试数据清除按钮
void MainWindow::on_clear_debug_btn_clicked()
{
    ui->Debug_Data_textEdit->clear();
}

// 调试数据 停止接收
void MainWindow::on_rec_stop_btn_clicked()
{
    m_show_debug_data=!m_show_debug_data;
    if(m_show_debug_data)ui->rec_stop_btn->setText("停止接收");
    else ui->rec_stop_btn->setText("开始接收");
}

// 调试数据 清除计数
void MainWindow::on_del_cnt_btn_clicked()
{
    m_Rec_Num=0;
    m_Send_Num=0;
    ui->label_Rec_Num->setText(QString::number(m_Rec_Num));
    ui->label_Send_Num->setText(QString::number(m_Send_Num));
}

/******************** ID卡 界面 函数 ********************/

// 发送 读 ID 指令
void MainWindow::on_ID_read_btn_clicked()
{
    if(m_Com_Open){
        QByteArray Send_Data;
        QString Mold_125 = "AB BA 00 15 00 15";                 //读取125k卡的指令
        StringToHex(Mold_125,Send_Data);                        //先将发送框的内容转换为Hex型
        m_ID_flag|=BIT_0;
        serial_write(Mold_125,Send_Data,"(发送读卡号指令)");
    }
}

// 发送 写 ID 指令
void MainWindow::on_ID_write_btn_clicked()
{
    if(m_Com_Open){
        QByteArray Send_Data;
        QString Send_Data_string;
        QString Write_Card[2];
        Write_Card[1] = ui->ID_Write_lineEdit->text();
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
            StringToHex(QString(Write_Card[0]+Write_Card[1]),Send_Data);    // 转换为QByteArray类型
            Send_Data[10]=xor_count(Send_Data,2,static_cast<unsigned char>(Send_Data.length()-1));
            Send_Data_string = Send_Data.toHex(' ').toUpper();              // 用于调试显示
            m_ID_flag|=BIT_1;
            serial_write(Send_Data_string,Send_Data,"(发送写卡号指令)");
        }
    }
}

void MainWindow::on_ID_data_clear_btn_clicked()
{
    ui->ID_data_listWidget->clear();
}

void MainWindow::on_ID_EM4100_rbtn_clicked()
{
    if(m_Com_Open)
        ui->ID_write_btn->setEnabled(false);
}

void MainWindow::on_ID_T5557_rbtn_clicked()
{
    if(m_Com_Open)
        ui->ID_write_btn->setEnabled(true);
}

/******************** IC卡 界面 函数 ********************/

void MainWindow::on_passA_rbtn_clicked()
{
    ui->old_pass_lineEdit->setEnabled(true);
    ui->new_pass_lineEdit->setEnabled(true);
    if(m_Com_Open)
        ui->pass_modify_btn->setEnabled(true);
}

void MainWindow::on_passB_rbtn_clicked()
{
    ui->old_pass_lineEdit->setEnabled(false);
    ui->old_pass_lineEdit->setText("FFFFFFFFFFFF");
    ui->new_pass_lineEdit->setEnabled(false);
    ui->new_pass_lineEdit->setText("FFFFFFFFFFFF");
}

void MainWindow::on_sector_cbox_currentIndexChanged(int index)
{
    // index 表示那个扇区
    switch(index){
    case 0:
        if(ui->block_cbox->currentIndex()==0 ||
           ui->block_cbox->currentIndex()==3)
            ui->sysblock_write_btn->setEnabled(false);
        else  ui->sysblock_write_btn->setEnabled(true);
        break;
    default:
        if(ui->block_cbox->currentIndex()==3)
            ui->sysblock_write_btn->setEnabled(false);
        else {
            ui->sysblock_write_btn->setEnabled(true);
        }
        break;
    }
}

void MainWindow::on_block_cbox_currentIndexChanged(int index)
{
    // index 表示那个块
    switch(index){
    case 0:
        if(ui->sector_cbox->currentIndex()!=0)       // 不是0扇区0块就使能
            ui->sysblock_write_btn->setEnabled(true);
        else  ui->sysblock_write_btn->setEnabled(false);
        break;
    case 3:
        ui->sysblock_write_btn->setEnabled(false);
        break;
    default:
        ui->sysblock_write_btn->setEnabled(true);
        break;
    }
}

/***** 寻卡 相关指令 ****/

// 单次 寻卡 指令
void MainWindow::on_one_shot_search_card_clicked()
{
    if(m_Com_Open){
        QByteArray Send_Data;
        QString Mold_1356 = "AB BA 00 10 00 10";                    //读取13.56M卡的指令
        StringToHex(Mold_1356,Send_Data);
        m_IC_flag|=BIT_0;
        serial_write(Mold_1356,Send_Data,"(13.56M寻卡指令)");
    }
}

// 连续 寻卡时 定时器槽函数
void MainWindow::IC_Card_continuous_search()
{
    if(m_IC_continuous)on_one_shot_search_card_clicked();
}

// 连续 寻卡
void MainWindow::on_continuous_search_card_clicked(bool checked)
{
    if(checked)m_IC_continuous=1;
    else m_IC_continuous=0;
}

// 清除 寻卡 信息
void MainWindow::on_search_card_clear_clicked()
{
    ui->search_card_listWidget->clear();
    for(auto &i:IC_Card)i.clear();
}

/***** 读写扇区 相关指令 ****/

// 读扇区 指令
void MainWindow::on_sysblock_read_btn_clicked()
{
    if(m_Com_Open){
        QByteArray Send_Data;
        QString Send_Data_string;
        QString Sys_Data[4];
        QString Pass_Mode;

        Sys_Data[0] = "ABBA";               // 协议头
        Sys_Data[1] = "0012090"+QString::number(ui->sector_cbox->currentIndex(),16);    // 哪个扇区
        if(ui->pass_cbox->currentIndex() == 0) Pass_Mode = "0A";
        else Pass_Mode = "0B";
        Sys_Data[2] = "0"+QString::number(ui->block_cbox->currentIndex(),16) + Pass_Mode; // 哪个块 和 密钥
        Sys_Data[3] = ui->pass_lineEdit->text();
        QString Sys = Sys_Data[0]+Sys_Data[1]+Sys_Data[2]+Sys_Data[3];
        StringToHex(Sys,Send_Data);
        Send_Data[Send_Data.length()]=xor_count(Send_Data,2,static_cast<unsigned char>(Send_Data.length()-1));
        Send_Data_string = Send_Data.toHex(' ').toUpper();              // 用于调试显示
        m_IC_flag|=BIT_1;
        serial_write(Send_Data_string,Send_Data,"(读取指定扇区数据)");
    }
}

// 写扇区 指令
void MainWindow::on_sysblock_write_btn_clicked()
{
    if(m_Com_Open){
        QByteArray Send_Data;
        QString Send_Data_string;
        QString Sys_Data[5];
        QString Pass_Mode;

        Sys_Data[0] = "ABBA";
        Sys_Data[1] = "0013190"+QString::number(ui->sector_cbox->currentIndex(),16);
        if(ui->pass_cbox->currentIndex() == 0) Pass_Mode = "0A";
        else Pass_Mode = "0B";
        Sys_Data[2] = "0"+QString::number(ui->block_cbox->currentIndex(),16) + Pass_Mode;
        Sys_Data[3] = ui->pass_lineEdit->text();                        //密码
        Sys_Data[4] = ui->sysblock_data_lineEdit->text().remove(' ');   //要写入的数据
        QString Sys = Sys_Data[0]+Sys_Data[1]+Sys_Data[2]+Sys_Data[3]+Sys_Data[4];
        StringToHex(Sys,Send_Data);
        Send_Data[Send_Data.length()]=xor_count(Send_Data,2,static_cast<unsigned char>(Send_Data.length()-1));
        Send_Data_string = Send_Data.toHex(' ').toUpper();              // 用于调试显示
        m_IC_flag|=BIT_2;
        serial_write(Send_Data_string,Send_Data,"(写入指定扇区数据)");
    }
}

// 数据区 清空
void MainWindow::on_sysblock_clear_btn_clicked()
{
    ui->sysblock_listWidget->clear();
}

// 修改密码 指令
void MainWindow::on_pass_modify_btn_clicked()
{
    QByteArray Send_Data;
    QString Send_Data_string;
    QString Write_Pass[5];

    Write_Pass[0] = "ABBA";
    int regic = ui->pass_sector_cbox->currentIndex();
    if(ui->passA_rbtn->isChecked())
    {
        Write_Pass[1] = "00140E0"+QString::number(regic,16)+"0A";
    }
    else
    {
        Write_Pass[1] = "00140E0"+QString::number(regic,16)+"0B";
    }
    Write_Pass[2] = ui->old_pass_lineEdit->text();
    Write_Pass[3] = ui->new_pass_lineEdit->text();
    if(Write_Pass[2].isEmpty() || Write_Pass[3].isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空！");
    }
    else if(Write_Pass[2].length()<12 || Write_Pass[3].length()<12)
    {
        QMessageBox::warning(this,"警告","密码不能低于12位！");
    }
    else
    {
        // 可以正常修改密码
        if(QMessageBox::Yes!=QMessageBox::critical(this, "注意","确认修改密码？",
                                 QMessageBox::Yes, QMessageBox::No))
            return;
    }
    QString Pass = Write_Pass[0]+Write_Pass[1]+Write_Pass[2]+Write_Pass[3];
    StringToHex(Pass,Send_Data);
    Send_Data[Send_Data.length()]=xor_count(Send_Data,2,static_cast<unsigned char>(Send_Data.length()-1));
    Send_Data_string = Send_Data.toHex(' ').toUpper();              // 用于调试显示
    m_IC_flag|=BIT_3;
    serial_write(Send_Data_string,Send_Data,"(修改密码指令)");
}

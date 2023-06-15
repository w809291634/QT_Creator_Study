/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "code.h"

/*********************************************************************************************
* 全局变量
*********************************************************************************************/
QString MainWindow::Code_Str;
QString MainWindow::ChmData_Str;

static long SendData_Count = 0;
static long RecData_Count = 0;
static int Last_Com = 0;
static unsigned char Read_Flag = 0;
static unsigned char Write_Flag = 0;
static bool Send_Flag = false;
static bool Save_Flag = false;
static bool ZigBee_AT_Test_OK = true;      //ZigBee AT测试指令
static bool LoRa_AT_Test_OK = false;        //LoRa AT测试标志位
static bool WiFi_AT_Test_OK = false;
static bool NB_AT_Test_OK = false;
static bool Test_OK = true;
static QByteArray uartData;
static int at_error_cnt = 0;
static bool listWidgetRecFouns = true;

static QStringList lines;
static QString line;
static int row = 0;

static QString isCom = "";
static QString com_str[30];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    up = new upData(this);
    IsOpen = false;                                             //串口是否打开标志位
    IsRecv = false;                                             //是否开始接收标志位
    Rec_Mode = true;                                           //接收模式标志位
    Send_Mode = true;                                          //发送模式标志位
    autoSendFlag = false;                                      // 自动发送模式标志位

    ui->actionZigBee->setChecked(true);

    ui->comboBox_Baud->setCurrentIndex(5);                      //默认选择波特率为38400
    ui->comboBox_Set_Rec->setCurrentIndex(1);
    ui->comboBox_Set_Send->setCurrentIndex(1);
    ui->lineEdit_SendSop->setText("FE");                        //发送数据头默认“FE”
    ui->lineEdit_Send_Cmd->setText("29 00 02");                 //发送数据的CMD默认

    RWControlChange(false);

    ui->le_sendTime->hide();

    ui->stackedWidget->setMinimumHeight(110);

    ui->lbl_version->setText(up->backVersion());

    //设置输入限制
    //[]内为可以输入的,{}表示输入位数限制
    Regexp_Panid.setPattern("[0-9]{1,5}");
    Regexp_LoRa_ID.setPattern("[0-9]{1,3}");
    Regexp_Na.setPattern("[0-9a-fA-F ]{1,5}");
    Regexp_Data_Ascii.setPattern("[a-zA-Z0-9?=_,.{} ]{1,250}");   //当输入模式为Ascii时限制数据长度最大为FF
    Regexp_Data_Hex.setPattern("[0-9a-fA-F ]{1,749}");          //当输入模式为Hex时限制数据长度最大为FF
    Regexp_G_ID.setPattern("[ -~]{1,100}");
    Regexp_G_KEY.setPattern("[ -~]{1,100}");
    Regexp_G_IP.setPattern("[0-9a-zA-Z.]{1,100}");
    Regexp_LoRa_PS.setPattern("[0-9]{1,2}");
    Regexp_LoRa_FP.setPattern("[0-9]{1,3}");
    Regexp_LoRa_HOP.setPattern("[0-9,]{1,39}");
    Regexp_LoRa_Release.setPattern("[0-9a-fA-F]{1,2}");
    Regexp_WiFi_SSID.setPattern("[0-9a-zA-Z_- ]{1,63}");
    Regexp_WiFi_PSK.setPattern("[0-9a-zA-Z]{1,31}");
    Regexp_WiFi_IP.setPattern("[0-9.]{1,50}");
    Regexp_LoRaWAN_JOINEUI.setPattern("[0-9a-fA-F]{1,16}");
    Regexp_LoRaWAN_KEY.setPattern("[0-9a-fA-F]{1,32}");
    ui->lineEdit_Panid->setValidator(new QRegExpValidator(Regexp_Panid,ui->lineEdit_Panid));
    ui->lineEdit_SendNa->setValidator(new QRegExpValidator(Regexp_Na,ui->lineEdit_SendNa));
    ui->lineEdit_SendData->setValidator(new QRegExpValidator(Regexp_Data_Ascii,ui->lineEdit_SendData));
    ui->lineEdit_Send_Cmd->setValidator(new QRegExpValidator(Regexp_Na,ui->lineEdit_Send_Cmd));

    Get_Com();                                                  //获取当前的可用串口号

    Set_Label(2);

    //串口读取到数据就调用读取串口函数
    connect(&Serial,SIGNAL(readyRead()),this,SLOT(ReadUart()));

    connect(ui->actionZigBee,SIGNAL(triggered(bool)),this,SLOT(Change_ZigBee()));
    connect(ui->actionabout,SIGNAL(triggered(bool)),this,SLOT(About_Up()));
    connect(ui->action_save,SIGNAL(triggered(bool)),this,SLOT(saveFileSlot()));
    connect(autoTime, SIGNAL(timeout()), this, SLOT(WriteUart()));
    //创建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(Get_Com()));      //每两秒读取一次串口
    timer->start(2000);
    QTimer::singleShot(1000, this, SLOT(firstVerUp()));
}

void MainWindow::firstVerUp()
{
    emit check_upFirst();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
         listWidgetRecFouns = true;
    }
}

void MainWindow::RWControlChange(bool enable)
{
    ui->pushButton_Read->setEnabled(enable);
    ui->pushButton_Write->setEnabled(enable);
    ui->comboBox_Com->setEnabled(!enable);
    ui->comboBox_Baud->setEnabled(!enable);
    ui->pushButton_Send->setEnabled(enable);
    ui->pushButton_OpenRec->setEnabled(enable);
}

//保存数据到文件
void MainWindow::saveFileSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this,"保存数据记录",QDir::currentPath(),"纯文本(.txt)");
    if(fileName.isEmpty())
    {
        return ;
    }
    if(fileName.indexOf(".txt") < 0)
        fileName += ".txt";
    file->setFileName(fileName);
    bool ok = file->open(QIODevice::ReadWrite); //读写
    if(ok)  //是否保存成功
    {
        row = 0;
        while(row<(ui->listWidget_Rec->count()))            //从wistlidget里一行一行的读取文本并存入lines里边
        {
            line=ui->listWidget_Rec->item(row)->text();
            row++;
            lines<<line;
        }
        QTextStream stream( file );//开始写入文本
        for ( QStringList::Iterator it=lines.begin(); it!=lines.end(); ++it )
        stream<<*it<<"\r\n";
        file->close();
    }
    else
    {
        QMessageBox::information(this,"出错","保存出错\r\n"+file->errorString());
        return;
    }
}

/*********************************************************************************************
* 名称:Get_Com()
* 功能:获取当前电脑可用串口
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Get_Com()
{
    //智能识别当前系统的有效串口号
    QList<QSerialPortInfo>serialPortInfo = QSerialPortInfo::availablePorts();
    int count = serialPortInfo.count();                     //有效串口的个数
    static int sum = 0;

    if(IsOpen == false)
    { 
        if(count != Last_Com)                                   //避免出现串口刷新后自动改变
        {
            ui->comboBox_Com->clear();                          //清除下拉选择框中的内容
            for(int a=0; a<sum; a++)
            {
                com_str[a] = "";
            }
            for(int i=0;i<count;i++)
            {
                //把有效的串口号添加到comboBox_Com中
                ui->comboBox_Com->addItem(serialPortInfo.at(i).portName());
                com_str[i] = serialPortInfo.at(i).portName();
            }
            sum = count;
            Last_Com = count;
        }
        else
        {
            return;
        }
    }
    else
    {
        bool off = false;
        for(int i=0; i<count; i++)
        {
            if(isCom == serialPortInfo.at(i).portName())
                off = true;
        }
        if(off == false)
        {
            OpenUart_Clicked();
        }
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
    time = QTime::currentTime();
    Time_str = time.toString("hh:mm:ss");
}


//版本更新
void MainWindow::About_Up()
{
    emit check_upData();
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
    bool ok;
    for(int a=x; a<z.length(); a+=y)
    {
        int Data_Int = z.mid(a,2).toInt(&ok,16);               //将组合数据分解
        Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
    }
    QString Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
    int Data_Fcs_Int = Data_Fcs.toInt(&ok,16);
    if(Data_Fcs_Int <= 15)
        Data_Fcs = '0'+Data_Fcs;
    return Data_Fcs;
}

//Ascii转换为Hex
QString MainWindow::Get_Hex(QString str)
{
    QString Data_Hex;
    for(int i=0; i<str.length(); i++)          //Ascii转Hex
    {
        QString Ascii = str.at(i);
        QByteArray SendApp_Data_Ascii = Ascii.toLatin1();
        QString SendApp_Data_Hex = QByteArray(SendApp_Data_Ascii.toHex());
        Data_Hex += " "+SendApp_Data_Hex;
    }
    return Data_Hex;
}

//设置
void MainWindow::Set_Label(unsigned char mode)
{
    if(mode > 0)
    {
        if(ZigBee_AT_Test_OK)
        {
            ui->label_RecSop->show();
            ui->lineEdit_RecSop->show();
            ui->label_RecNa->show();
            ui->label_RecAppCmd->show();
            ui->label_RecLen->show();
            ui->lineEdit_RecLen->show();
            ui->label_RecCmd->show();
            ui->label_SendCmd->show();
            ui->label_SendNa->show();
            ui->label_Send_Cmd->show();
            ui->lineEdit_RecAppCmd->show();
            ui->lineEdit_RecCmd->show();
            ui->lineEdit_RecNa->show();
            ui->lineEdit_SendNa->show();
            ui->lineEdit_Send_Cmd->show();
            ui->comboBox_Send_AppCmd->show();
            ui->label_RecSop->setText("  SOP");
            ui->lineEdit_RecSop->setPlaceholderText(" 数据头");
            ui->lineEdit_SendLen->setPlaceholderText(" 数据长度(自动计算)");
            ui->lineEdit_SendData->setPlaceholderText(" 数据");
            ui->label_RecNa->setText("   NA");
            ui->lineEdit_RecNa->setPlaceholderText(" 网络地址");
            ui->label_RecCmd->setText("  CMD");
            ui->lineEdit_RecCmd->setPlaceholderText(" 标示码");
            ui->lineEdit_SendSop->setText("FE");
            ui->label_RecFcs->setText("  FCS");
            ui->lineEdit_RecFcs->setPlaceholderText(" 校验位");
            ui->label_RecLen->setText("  LEN");
            ui->lineEdit_RecLen->setPlaceholderText(" 数据长度");
            ui->label_Send_Cmd->setText("  CMD");
            ui->lineEdit_Send_Cmd->setText("29 00 02");
            ui->lineEdit_Send_Cmd->setReadOnly(true);
            ui->lineEdit_Send_Cmd->setPlaceholderText("");
            ui->label_RecAppData->setText("APP_DATA");
            ui->lineEdit_RecAppData->setPlaceholderText(" 数据");

            ui->label_RecFcs->show();
            ui->lineEdit_RecFcs->show();
            ui->label_SendLen->show();
            ui->lineEdit_SendLen->show();

            ui->label_Recrelease->hide();
            ui->lineEdit_release->hide();
            ui->LoRa_label_release->hide();
            ui->LoRa_lineEdit_release->hide();

            ui->label_SendFcs->show();
            ui->lineEdit_SendFcs->show();
            ui->label_SendSop->show();
            ui->lineEdit_SendSop->show();
            ui->label_SendSop->setText("  SOP");
            ui->label_SendLen->setText("  LEN");
            ui->label_SendNa->setText("   NA");
            ui->lineEdit_SendNa->setPlaceholderText(" 网络地址");
            ui->label_SendData->setText("APP_DATA");
        }
        else
        {
            ui->label_RecAppCmd->hide();
            ui->lineEdit_RecAppCmd->hide();
            ui->label_RecNa->hide();
            ui->lineEdit_RecNa->hide();
            ui->label_RecCmd->hide();
            ui->lineEdit_RecCmd->hide();

            ui->label_SendCmd->hide();
            ui->comboBox_Send_AppCmd->hide();
            ui->label_SendNa->hide();
            ui->lineEdit_SendNa->hide();
            ui->label_Send_Cmd->hide();
            ui->lineEdit_Send_Cmd->hide();

            ui->label_RecFcs->hide();
            ui->lineEdit_RecFcs->hide();
            ui->label_SendFcs->hide();
            ui->lineEdit_SendFcs->hide();
            ui->label_RecSop->hide();
            ui->lineEdit_RecSop->hide();
            ui->label_SendSop->hide();
            ui->lineEdit_SendSop->hide();
            ui->label_RecLen->setText("数据长度");
            ui->lineEdit_RecLen->setPlaceholderText("");
            ui->label_RecAppData->setText("应用数据");
            ui->lineEdit_RecAppData->setPlaceholderText("");

            ui->label_SendLen->setText("数据长度");
            ui->lineEdit_SendLen->setPlaceholderText(" 数据长度(自动计算)");
            ui->label_SendData->setText("应用数据");
            ui->lineEdit_SendData->setPlaceholderText(" 应用数据");
        }
        if(mode == 1)
        {
            if(IsOpen)
                OpenUart_Clicked();
        }
    }
    else
    {
        ui->label_RecNa->hide();
        ui->label_RecAppCmd->hide();
        ui->label_RecCmd->hide();
        ui->label_SendCmd->hide();
        ui->label_SendNa->hide();
        ui->label_Send_Cmd->hide();
        ui->lineEdit_RecAppCmd->hide();
        ui->lineEdit_RecCmd->hide();
        ui->lineEdit_RecNa->hide();
        ui->lineEdit_SendNa->hide();
        ui->lineEdit_Send_Cmd->hide();
        ui->comboBox_Send_AppCmd->hide();
        if(Mode == 2)
        {
            ui->label_Recrelease->show();
            ui->lineEdit_release->show();
            ui->LoRa_label_release->show();
            ui->LoRa_lineEdit_release->show();
            ui->LoRa_lineEdit_release->setText("81");
            ui->lineEdit_SendLen->setValidator(new QRegExpValidator(Regexp_LoRa_ID,ui->lineEdit_SendLen));
            ui->label_RecSop->show();
            ui->lineEdit_RecSop->show();
            ui->lineEdit_SendData->setPlaceholderText(" 应用数据");
            ui->label_RecSop->setText("数据长度");
            ui->lineEdit_RecSop->setPlaceholderText("");
            ui->label_RecLen->setText(" 网络ID");
            ui->lineEdit_RecLen->setPlaceholderText("");

            ui->label_RecFcs->hide();
            ui->lineEdit_RecFcs->hide();

            ui->label_RecNa->show();
            ui->lineEdit_RecNa->show();
            ui->label_RecNa->setText(" 源地址");
            ui->lineEdit_RecNa->setPlaceholderText("");
            ui->label_RecCmd->show();
            ui->lineEdit_RecCmd->show();
            ui->label_RecCmd->setText("目的地址");
            ui->lineEdit_RecCmd->setPlaceholderText("");
            ui->label_RecAppData->setText("应用数据");
            ui->lineEdit_RecAppData->setPlaceholderText("");
            ui->lineEdit_release->setPlaceholderText("");

            ui->label_SendSop->show();
            ui->lineEdit_SendSop->show();

            ui->label_SendSop->setText("数据长度");
            ui->lineEdit_SendSop->setPlaceholderText(" 数据长度(自动计算)");
            ui->lineEdit_Send_Cmd->setReadOnly(false);
            ui->label_SendLen->show();
            ui->lineEdit_SendLen->show();

            ui->label_SendLen->setText(" 网络ID");
            ui->lineEdit_SendLen->show();
            ui->lineEdit_SendLen->setReadOnly(false);
            ui->lineEdit_SendLen->setPlaceholderText(" 网络ID(十进制)");

            ui->label_Send_Cmd->show();
            ui->lineEdit_Send_Cmd->show();
            ui->label_Send_Cmd->setText("目的地址");
            ui->lineEdit_Send_Cmd->clear();
            ui->lineEdit_Send_Cmd->setPlaceholderText(" 目的地址");

            ui->label_SendNa->show();
            ui->lineEdit_SendNa->show();
            ui->label_SendNa->setText(" 源地址");
            ui->lineEdit_SendNa->setPlaceholderText(" 源地址");

            ui->label_SendData->setText("应用数据");

            ui->label_SendFcs->hide();
            ui->lineEdit_SendFcs->hide();

            ui->lineEdit_SendSop->clear();
        }
        else
        {
            ui->label_RecFcs->hide();
            ui->lineEdit_RecFcs->hide();
            ui->label_SendFcs->hide();
            ui->lineEdit_SendFcs->hide();
            ui->label_RecSop->hide();
            ui->lineEdit_RecSop->hide();
            ui->label_SendSop->hide();
            ui->lineEdit_SendSop->hide();
            ui->label_RecLen->setText("数据长度");
            ui->lineEdit_RecLen->setPlaceholderText("");
            ui->label_RecAppData->setText("应用数据");
            ui->lineEdit_RecAppData->setPlaceholderText("");

            ui->label_SendLen->setText("数据长度");
            ui->lineEdit_SendLen->setPlaceholderText(" 数据长度(自动计算)");
            ui->label_SendData->setText("应用数据");
            ui->lineEdit_SendData->setPlaceholderText(" 应用数据");
        }
        if(IsOpen)
            OpenUart_Clicked();
    }
    if(Mode != 2)
    {
        ui->label_Recrelease->hide();
        ui->lineEdit_release->hide();
        ui->LoRa_label_release->hide();
        ui->LoRa_lineEdit_release->hide();
        ui->lineEdit_SendLen->setReadOnly(true);
    }
    ui->lineEdit_RecAppCmd->clear();
    ui->lineEdit_RecAppData->clear();
    ui->lineEdit_RecCmd->clear();
    ui->lineEdit_RecFcs->clear();
    ui->lineEdit_RecNa->clear();
    ui->lineEdit_RecSop->clear();
    ui->lineEdit_RecLen->clear();
    ui->lineEdit_SendLen->clear();
    ui->lineEdit_SendData->clear();
    ui->lineEdit_SendFcs->clear();
    ui->lineEdit_SendNa->clear();
    if(!IsOpen)
        ui->comboBox_Baud->setCurrentIndex(5);
    ui->lineEdit_SendLen->clear();
    ui->lineEdit_SendNa->setReadOnly(false);
    ui->comboBox_Send_AppCmd->setEnabled(true);
}

//切换界面
void MainWindow::Change_ZigBee()
{
    ui->lineEdit_Add->clear();
    ui->lineEdit_Panid->clear();
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionZigBee->setChecked(true);
    Mode = 1;
    Set_Label(1);
    ui->stackedWidget->setMinimumHeight(110);
}


/*********************************************************************************************
* 名称:Set_RecMode()
* 功能:设置当前接收的显示模式
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Set_RecMode()
{
    bool ok;
    QString Data;
    QString RecApp_Data = ui->lineEdit_RecAppData->text();
    QString Rec_SOP = ui->lineEdit_RecSop->text();
    QString Rec_LEN = ui->lineEdit_RecLen->text();
    QString Rec_RSSI = ui->lineEdit_RecFcs->text();
    int RecApp_Data_Len = (RecApp_Data.length()-RecApp_Data.count(' '))/2;
    if(ui->comboBox_Set_Rec->currentIndex() == 1)
    {
        Rec_Mode = true;
        if(Mode == 2)  //LoRa
        {
            for(int i=0; i<RecApp_Data_Len; i++)                    //Data
            {
                int Data_Int = RecApp_Data.section(' ',i,i).toInt(&ok,16);
                Data += QString((Data_Int%256)&0xFF);
            }
            ui->lineEdit_RecAppData->setText(Data.simplified());
            Data.clear();
        }
        else
        {
            for(int i=0; i<RecApp_Data_Len; i++)                    //Hex转Ascii
            {
                int Data_Int = RecApp_Data.section(' ',i,i).toInt(&ok,16);
                Data += QString((Data_Int%256)&0xFF);
            }
            ui->lineEdit_RecAppData->setText(Data.simplified());
            Data.clear();
        }
    }
    else
    {
        Rec_Mode = false;
        QString RecApp_Data_Str;
        if(Mode == 2)
        {
            for(int i=0; i<RecApp_Data.length(); i++)               //Ascii转Hex
            {
                QString Ascii = RecApp_Data.at(i);
                QByteArray RecApp_Data_Ascii = Ascii.toLatin1();
                QString RecApp_Data_Hex = QByteArray(RecApp_Data_Ascii.toHex());
                RecApp_Data_Str += " "+RecApp_Data_Hex;
            }
            ui->lineEdit_RecAppData->setText(RecApp_Data_Str.toUpper().simplified());
            Data.clear();
        }
        else
        {
            for(int i=0; i<RecApp_Data.length(); i++)               //Ascii转Hex
            {
                QString Ascii = RecApp_Data.at(i);
                QByteArray RecApp_Data_Ascii = Ascii.toLatin1();
                QString RecApp_Data_Hex = QByteArray(RecApp_Data_Ascii.toHex());
                RecApp_Data_Str += " "+RecApp_Data_Hex;
            }
            ui->lineEdit_RecAppData->setText(RecApp_Data_Str.toUpper().simplified());
            Data.clear();
        }
    }
}

/*********************************************************************************************
* 名称:Set_SendMode()
* 功能:设置发送数据显示模式
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Set_SendMode()
{
    QString Data;
    bool ok;
    QString SendApp_Data = ui->lineEdit_SendData->text().simplified();
    int SendApp_Data_Len = (SendApp_Data.length()-SendApp_Data.count(' '))/2;
    if(ui->comboBox_Set_Send->currentIndex() == 1)
    {
        Send_Mode = true;
        //设置输入Ascii时的输入限制
        ui->lineEdit_SendData->setValidator(new QRegExpValidator(Regexp_Data_Ascii,ui->lineEdit_SendData));
        for(int i=0; i<SendApp_Data_Len; i++)                   //Hex转Ascii
        {
            int Data_Int = SendApp_Data.section(' ',i,i).toInt(&ok,16);
            Data += QString((Data_Int%256)&0xFF);
        }
        ui->lineEdit_SendData->setText(Data.simplified());
    }
    else
    {
        Send_Mode = false;
        QString SendApp_Data_Str;
        //设置输入Hex时的输入限制
        ui->lineEdit_SendData->setValidator(new QRegExpValidator(Regexp_Data_Hex,ui->lineEdit_SendData));
        for(int i=0; i<SendApp_Data.length(); i++)              //Ascii转Hex
        {
            QString Ascii = SendApp_Data.at(i);
            QByteArray SendApp_Data_Ascii = Ascii.toLatin1();
            QString SendApp_Data_Hex = QByteArray(SendApp_Data_Ascii.toHex());
            SendApp_Data_Str += " "+SendApp_Data_Hex;
        }
        ui->lineEdit_SendData->setText(SendApp_Data_Str.toUpper().simplified());
    }
}

/*********************************************************************************************
* 名称:Del_Num()
* 功能:复位计数值
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Del_Num()
{
    ui->label_CountRecvNum->setText("0");
    ui->label_CountSendNum->setText("0");
    SendData_Count = 0;
    RecData_Count = 0;
}

MainWindow::~MainWindow()
{
    Serial.close();                                             //软件退出后串口断开连接

    delete file;
    delete ui;
}


/*********************************************************************************************
* 名称:OpenRecv_Clicked()
* 功能:接收按钮功能的实现
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::OpenRecv_Clicked()
{
    if(IsRecv == true)
    {
        IsRecv = false;
        ui->pushButton_OpenRec->setText("开始接收");                //按钮上文字变化
        ui->label_State->setText("接收停止");
    }
    else
    {
        IsRecv = true;
        ui->pushButton_OpenRec->setText("停止接收");
        ui->label_State->setText("正在接收");
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
        timer->start(2000);
        IsOpen = false;
        IsRecv = false;
        ZigBee_AT_Test_OK = true;
        Test_OK = true;
        uartData.clear();
        ui->label_State->setText("连接断开");
        ui->pushButton_OpenCom->setText("打开串口");             //按键字变化
        ui->pushButton_OpenRec->setText("开始接收");             //接收按键文字变化

        ui->comboBox_Mold->setEnabled(true);

        //打开或者关闭一些按键的可用性
        RWControlChange(false);

        RecData.clear();
        //关闭串口网络信息清除
        if(Mode == 1)
        {
            ui->lineEdit_Panid->clear();
            ui->lineEdit_Add->clear();
            ui->lineEdit_Add->clear();
        }
        if(autoSendFlag)
        {
            autoSendFlag = false;
            ui->pushButton_Send->setText("开始发送");
            ui->le_sendTime->setReadOnly(false);
            autoTime->stop();
        }
    }
    else                                                        //串口当前处于未打开状态
    {
        if(SetUart() == true)                                   //串口打开成功
        {
            isCom = ui->comboBox_Com->currentText();
            IsOpen = true;
            IsRecv = true;
            Read_Flag = 0;
            Test_OK = true;
            uartData.clear();
            ui->label_State->setText("连接成功");
            ui->pushButton_OpenCom->setText("关闭串口");         //按键文字变化
            ui->pushButton_OpenRec->setText("停止接收");
//            timer->stop();

            //打开或者关闭一些按键的可用性
            RWControlChange(true);

            Read_Parameter();

        }
        else
        {
            IsOpen = false;
            ui->label_State->setText(Serial.errorString());
            //串口打开失败,弹出打开失败原因信息
            QMessageBox::warning(this,"Connection Error",Serial.errorString());
            return;
        }
    }
}

//获取节点参数
void MainWindow::Read_Parameter()
{
    if(Mode == 1)
        ZigBee_AT_Test();                                     //读取当前网络信息
}

//读取写入标志位清除
void MainWindow::Del_Read_Write_flag()
{
    Read_Flag = 0;
    Write_Flag = 0;
    LoRa_AT_Test_OK = false;
    WiFi_AT_Test_OK = false;
    NB_AT_Test_OK = false;
    Test_OK = false;
}

// 功能:添加空格
// 参数:x--字符串开始位置/z--字符串
QString MainWindow::Add_Space(int x, QString z)
{
    QString ss;
    for(int i=x; i<z.length(); i+=2)
    {
        QString aa = z.mid(i,2);
        ss += aa+" ";
    }
    return ss.toUpper();
}

//ZigBee串口数据解析
void MainWindow::ZigBee_Data_Analysis()
{
    bool ok;
    QString Type_Mold;
    QString Panid,Channel;
    QString RecData_s;
    QByteArray RecData_Uart;
    RecData_Uart = Serial.readAll();                   //读取串口的数据
    int idx = -1;
    QString atc = "";
    uartData += RecData_Uart;
    static QString data = "";
    static int data_length = 0;
    QString Time_Data;
    do {
        if (data_length > 0)
        {
            if (uartData.length()<= data_length)
            {
                data += uartData;
                data_length -= uartData.length();
                uartData = "";
            }
            else
            {
                data += uartData.left(data_length);
                uartData = uartData.right(uartData.length()-data_length);
                data_length  = 0;
            }
            if (data_length == 0)
            {
                QString shex="";
                for(int i=0; i<data.length(); i++)          //Ascii转Hex
                {
                    QString Ascii = data.at(i);
                    QByteArray Clicked_Data_Ascii = Ascii.toLatin1();
                    QString Clicked_Data_Hex = QByteArray(Clicked_Data_Ascii.toHex());
                    shex += " "+Clicked_Data_Hex;
                }
                /**/
                Get_Time();
                Time_Data = "["+Time_str+"  ZigBee] --> [Hex]"+shex.simplified().toUpper();
                ui->listWidget_Rec->addItem(Time_Data);
                data = "";
            }
        }
        else
        {
            idx = uartData.indexOf("\r\n");
            if (idx >= 0)
            {
                atc = uartData.left(idx);
                uartData = uartData.right(uartData.length()-(idx+2));
                if(atc.mid(0,3) == "ERR")
                {
                    if(Test_OK)
                    {
                        ZigBee_AT_Test_OK = false;
                        Set_Label(2);
                        Test_OK = false;
                    }
                }
                if (atc == "OK")
                {
                    Get_Time();
                    Time_Data = "["+Time_str+"  ZigBee] --> "+atc;
                    ui->listWidget_Rec->addItem(Time_Data);
                    if(Write_Flag==3)
                    {
                        Write_Flag = 2;
                        Write_NetWord();
                    }
                    else if(Write_Flag ==2)
                    {
                        Write_Flag = 1;
                        Write_NetWord();
                    }
                    else if(Write_Flag==1)
                    {
                        WiFi_Save_Reset(1);     //重启
                        Write_Flag = 0;
                    }
                    if(Test_OK)
                    {
                        ZigBee_AT_Test_OK = false;
                        Set_Label(2);
                        Test_OK = false;
                    }
                    if (Read_Flag != 0)
                    {
                        Read_NetWord(1);
                    }
                }
                else if(Read_Flag && atc.mid(0,7) == "+PANID:")
                {
                    Read_Flag = 3;
                    ui->lineEdit_Panid->setText(atc.mid(7,atc.length()-7));
                }
                else if(Read_Flag && atc.mid(0,9) == "+CHANNEL:")
                {
                    Read_Flag = 2;
                    ui->comboBox_Channel->setCurrentText(atc.mid(9,2));
                }
                else if(Read_Flag && atc.mid(0,13) == "+LOGICALTYPE:")
                {
                    Read_Flag = 1;
                    ui->comboBox_Mold->setCurrentIndex(atc.mid(13,1).toInt(&ok,10));
                }
                else if(Read_Flag && atc.mid(0,5) == "+MAC:")
                {
                    Read_Flag = 0;
                    ui->lineEdit_Add->setText(atc.mid(5,atc.length()-5));
                }
                else if(atc.mid(0,6) == "+RECV:")
                {
                    int len = 0;
                    int pos = atc.indexOf(',');
                    if(pos == -1) {
                        pos = atc.length();
                    }
                    if(pos > 6) {
                        len = atc.mid(6,pos-6).toInt();
                    }
                    if (len > 0)
                    {
                        data_length = len;
                    }
                }
                if(atc != "OK")
                {
                    Get_Time();
                    Time_Data = "["+Time_str+"  ZigBee] --> "+atc;
                    ui->listWidget_Rec->addItem(Time_Data);
                }
            }
            else
                if (uartData.right(1) == ">")
                {
                    Get_Time();
                    Time_Data = "["+Time_str+"  ZigBee] --> " + uartData;
                    ui->listWidget_Rec->addItem(Time_Data);
                    if(Send_Flag)
                    {
                        WiFi_Send_Data();           //ZigBee发送数据
                        Send_Flag = false;
                    }
                    uartData = "";
                }
        }
    }while (idx>0 && uartData.length()>0);

    if (!ZigBee_AT_Test_OK ){}
    else
    {
        if(RecData.isEmpty())
        {
            QString hex_data=RecData_Uart.toHex().toUpper();
            hex_data=Add_Space(0,hex_data);
            RecData = RecData.append(hex_data);
        }
        else
        {
            QString hex_data=RecData_Uart.toHex().toUpper();
            hex_data=Add_Space(0,hex_data);
            RecData = RecData.append(" "+hex_data);
        }
        for(int x=0; x<RecData.length(); x+=3)              //解析储存的数据
        {
            int RecData_Sum = 0;
            if(RecData.mid(x,2) == "FE")
            {
                RecData_Sum = RecData.mid(x+3,2).toInt(&ok,16);//获取数据长度
                if(RecData.length()>=(RecData_Sum+5)*3-1)
                {
                    RecData_s = RecData.mid(x,(RecData_Sum+5)*3);//截取数据长度的数据
                    Get_Time();
                    Time_Data = "["+Time_str+"  ZigBee] --> " + RecData_s;
                    ui->listWidget_Rec->addItem(Time_Data); //将串口的数据显示在接收文本框中
                    RecData.replace(0,(RecData_Sum+5)*3+x,"");//删除已经解析完的数据
                    x=-3;
                }
                else
                {
                    break;
                }
            }
        }
        //解析网络信息
        unsigned char y = 0;
        if(RecData_s.mid(3,2) == "27")          //判断PANID位数
            y = 5;
        else if(RecData_s.mid(3,2) == "26")
            y = 4;
        else if(RecData_s.mid(3,2) == "25")
            y = 3;
        else if(RecData_s.mid(3,2) == "24")
            y = 2;
        else
            y = 1;
        if(Read_Flag && RecData_s.mid(27,11) == "54 59 50 45"
                && RecData_s.mid(60,14) == "50 41 4E 49 44"
                && RecData_s.mid(81+y*3,20) == "43 48 41 4E 4E 45 4C")
        {

            if(RecData_s.mid(27,11) == "54 59 50 45")        //解析Type
            {
                Type_Mold = RecData_s.mid(45,2);
                if(Type_Mold == "30")
                {
                    ui->comboBox_Mold->setCurrentIndex(0);
                    ui->lineEdit_SendNa->setReadOnly(false);
                    ui->comboBox_Send_AppCmd->setEnabled(true);
                    ui->comboBox_Mold->setEnabled(false);
                }
                else if(Type_Mold == "31")
                {
                    ui->comboBox_Mold->setCurrentIndex(1);
                    ui->lineEdit_SendNa->setText("00 00");
                    ui->lineEdit_SendNa->setReadOnly(true);
                    ui->comboBox_Send_AppCmd->setEnabled(false);
                    ui->comboBox_Mold->setEnabled(true);
                }
                else if(Type_Mold == "32")
                {
                    ui->comboBox_Mold->setCurrentIndex(2);
                    ui->lineEdit_SendNa->setText("00 00");
                    ui->lineEdit_SendNa->setReadOnly(true);
                    ui->comboBox_Send_AppCmd->setEnabled(false);
                    ui->comboBox_Mold->setEnabled(true);
                }
            }
            if(RecData_s.mid(60,14) == "50 41 4E 49 44")       //解析PANID
            {
                for(int j=0; j<y; j++)
                {
                    int ss = RecData_s.mid(78+j*3,2).toInt(&ok,16);
                    Panid += QString(ss);
                }
                ui->lineEdit_Panid->setText(Panid);
            }
            if(RecData_s.mid(81+y*3,20) == "43 48 41 4E 4E 45 4C") //解析Channel
            {
                for(int j=0; j<2; j++)
                {
                    int ss = RecData_s.mid(105+y*3+j*3,2).toInt(&ok,16);
                    Channel += QString(ss);
                }
                ui->comboBox_Channel->setCurrentText(Channel);
                ZigBee_Read_MAC();   //查询MAC
            }
        }
        else if(Read_Flag && RecData_s.mid(18,11) == "01 02 00 00")
        {
            ui->lineEdit_Add->setText(RecData_s.mid(30,23).replace(' ',':'));
            Read_Flag = 0;
        }
    }
}

/*********************************************************************************************
* 名称:ReadUart()
* 功能:读取串口数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::ReadUart()
{
    if(IsRecv == true)
    {
        QThread::currentThread()->msleep(30);                   //延时
        RecData_Count += Serial.bytesAvailable();
        if(Mode == 1)
        {
            ZigBee_Data_Analysis();         //ZigBee串口数据解析函数
        }
        ui->label_CountRecvNum->setText(QString::number(RecData_Count,10));
        if(ui->listWidget_Rec->count() >= 1000)
        {
            row = 0;
            while(row<(ui->listWidget_Rec->count()))            //从wistlidget里一行一行的读取文本并存入lines里边
            {
                line=ui->listWidget_Rec->item(row)->text();
                row++;
                lines<<line;
            }
            ui->listWidget_Rec->clear();
        }
        if(listWidgetRecFouns == true)                          //判断当前数据接收是否处于活动状态
            ui->listWidget_Rec->scrollToBottom();               //接收窗口下滑至最底处
    }
    else
    {
        Serial.readAll().clear();
    }
}

/*********************************************************************************************
* 名称:Analysis()
* 功能:将鼠标点击的数据进行解析
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Analysis()
{
    bool ok;
    QString Data;
    listWidgetRecFouns = false;
    QString Clicked_Data = ui->listWidget_Rec->currentItem()->text();
    Clicked_Data = Clicked_Data.replace(0,23,"");       //删除数据介绍
    if(Mode == 1)
    {
        if(!ZigBee_AT_Test_OK)
        {
            if(Clicked_Data.indexOf("[Hex]") >= 0)
            {
                Clicked_Data = Clicked_Data.replace(0,5,"");
                if(Rec_Mode)
                {
                    for(int i=0; i<Clicked_Data.length(); i+=3)                   //Hex转Ascii
                    {
                        int Data_Int = Clicked_Data.mid(i,2).toInt(&ok,16);
                        Data += QString((Data_Int%256)&0xFF);
                    }
                    ui->lineEdit_RecAppData->setText(Data);
                    ui->lineEdit_RecLen->setText(QString::number(Data.length(),10));
                }
                else
                {
                    ui->lineEdit_RecLen->setText(QString::number(Clicked_Data.length(),10));
                    ui->lineEdit_RecAppData->setText(Clicked_Data);
                }
            }
        }
        else
        {
            if(Clicked_Data.mid(0,2) == "FE" && (Clicked_Data.mid(6,5) == "69 80" || Clicked_Data.mid(6,5) == "29 00"))
            {
                int a = 0;
                int Len = Clicked_Data.section(' ',1,1).toInt(&ok,16);      //把数据长度转换为十进制
                if(Clicked_Data.mid(6,5) == "29 00")
                    a = 1;
                //将选中的数据分割处理显示
                ui->lineEdit_RecSop->setText(Clicked_Data.section(' ',0,0));
                ui->lineEdit_RecLen->setText(Clicked_Data.section(' ',1,1));
                ui->lineEdit_RecCmd->setText(Clicked_Data.section(' ',2,3+a));
                ui->lineEdit_RecAppCmd->setText(Clicked_Data.section(' ',6+a,7+a));
                ui->lineEdit_RecNa->setText(Clicked_Data.section(' ',4+a,5+a));
                int Clicked_Data_len = (Clicked_Data.section(' ',8+a,Len+3).length()-Clicked_Data.section(' ',8+a,Len+3).count(' '))/2;
                if(Rec_Mode)                                        //将数据转化为Ascii显示
                {
                    for(int i=0; i<Clicked_Data_len; i++)
                    {
                        int Data_Int = Clicked_Data.section(' ',8+i+a,8+i+a).toInt(&ok,16);
                        Data += QString((Data_Int%256)&0xFF);
                    }
                    ui->lineEdit_RecAppData->setText(Data.simplified());
                }
                else                                                        //直接Hex模式显示
                    ui->lineEdit_RecAppData->setText(Clicked_Data.section(' ',8+a,Len+3));

                ui->lineEdit_RecFcs->setText(Clicked_Data.section(' ',Len+4,Len+4));
            }
            else
            {
                return ;
            }
        }
    }
    else if(Mode == 2)
    {
        if(Clicked_Data.indexOf("[Hex]") >= 0)
        {
            Clicked_Data = Clicked_Data.replace(0,5,"");
            ui->lineEdit_RecLen->setText(QString::number(Clicked_Data.mid(0,2).toInt(&ok, 16), 10));
            ui->lineEdit_RecCmd->setText(Clicked_Data.mid(3,5));
            ui->lineEdit_RecNa->setText(Clicked_Data.mid(9,5));
            ui->lineEdit_release->setText(Clicked_Data.mid(15,2));
            QString Data_ss = Clicked_Data.mid(18,Clicked_Data.length()-18);

            if(Rec_Mode)
            {
                for(int i=0; i<Data_ss.length(); i+=3)                   //Hex转Ascii
                {
                    int Data_Int = Data_ss.mid(i,2).toInt(&ok,16);
                    Data += QString((Data_Int%256)&0xFF);
                }
                ui->lineEdit_RecAppData->setText(Data);
                ui->lineEdit_RecSop->setText(QString::number(Data.length()+6,10));
            }
            else
            {
                ui->lineEdit_RecSop->setText(QString::number((Clicked_Data.length()-Clicked_Data.count(' '))/2,10));
                ui->lineEdit_RecAppData->setText(Data_ss);
            }
        }
    }
    else
    {
        if(Clicked_Data.indexOf("[Hex]") >= 0)
        {
            Clicked_Data = Clicked_Data.replace(0,5,"");
            if(Rec_Mode)
            {
                for(int i=0; i<Clicked_Data.length(); i+=3)                   //Hex转Ascii
                {
                    int Data_Int = Clicked_Data.mid(i,2).toInt(&ok,16);
                    Data += QString((Data_Int%256)&0xFF);
                }
                ui->lineEdit_RecAppData->setText(Data);
                ui->lineEdit_RecLen->setText(QString::number(Data.length(),10));
            }
            else
            {
                ui->lineEdit_RecLen->setText(QString::number(Clicked_Data.length(),10));
                ui->lineEdit_RecAppData->setText(Clicked_Data);
            }
        }
        else
        {
            return ;
        }
    }
}

/*********************************************************************************************
* 名称:ConvertHexChar()
* 功能:将Hex型转换为Char型
* 参数:ch-需要转换的Hex型数据
* 返回:转换完成的Char型数据
*********************************************************************************************/
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return ch-ch;                                      //不在0-f范围内的会发送成0
}

/*********************************************************************************************
* 名称:StringToHex()
* 功能:将字符串转换为十六进制
* 参数:str-需要转换的字符串
*     send_data-转换后的十六进制数
* 返回:无
*********************************************************************************************/
void MainWindow::StringToHex(QString str,QByteArray &send_data)
{
    int hex_data,lowhex_data;
    int hex_data_len = 0;
    int len = str.length();
    send_data.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr = str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hex_data = ConvertHexChar(hstr);
        lowhex_data = ConvertHexChar(lstr);
        if((hex_data == 16)||(lowhex_data == 16))
            break;
        else
            hex_data = hex_data*16 + lowhex_data;
        i++;
        send_data[hex_data_len] = (char)hex_data;
        hex_data_len++;
    }
    send_data.resize(hex_data_len);
}

/*********************************************************************************************
* 名称:WriteUart()
* 功能:串口发送数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::WriteUart()
{
    QByteArray Send_Data;
    QString str;
    if(Mode == 1)
    {
        if(!ZigBee_AT_Test_OK)
        {
            ZigBee_Send_Length();
        }
        else
        {
            str = ZigBeeData_Hex;
            StringToHex(str,Send_Data);                                 //先将发送框的内容转换为Hex型
            Serial.write(Send_Data);
            Get_Time();
            str = "["+Time_str+"  ZigBee] <-- "+str;
            ui->listWidget_Rec->addItem(str);
            SendData_Count += Serial.bytesToWrite();
            ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
        }
    }
    if(listWidgetRecFouns == true)
        ui->listWidget_Rec->scrollToBottom();               //接收窗口下滑至最底处
}

/*********************************************************************************************
* 名称:Del_SendData()
* 功能:删除发送区的数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Del_SendData()
{
    ui->lineEdit_SendNa->clear();
    ui->lineEdit_SendData->clear();
    ui->lineEdit_SendFcs->clear();
    ui->lineEdit_SendLen->clear();
}

/*********************************************************************************************
* 名称:Del_RecData()
* 功能:删除接收区的数据
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Del_RecData()
{
    ui->lineEdit_RecAppCmd->clear();
    ui->lineEdit_RecAppData->clear();
    ui->lineEdit_RecCmd->clear();
    ui->lineEdit_RecFcs->clear();
    ui->lineEdit_RecNa->clear();
    ui->lineEdit_RecSop->clear();
    ui->listWidget_Rec->clear();
    ui->lineEdit_RecLen->clear();
}

//组合运算选择
void MainWindow::Mode_Group()
{
    if(Mode == 1)
    {
        Group();
    }
    else if(Mode == 2) //其他模式组合运算
    {
        Group_LoRa();
    }
    else
    {
        Group_NoZigBee();
    }
}

//非ZigBee模式运算
void MainWindow::Group_NoZigBee()
{
    bool ok;
    Other.clear();
    QString NoZigBee_Data = ui->lineEdit_SendData->text();
    if(NoZigBee_Data == "")
    {
        ui->lineEdit_SendLen->clear();
    }
    else
    {
        if(Send_Mode)
        {
            ui->lineEdit_SendLen->setText(QString::number(NoZigBee_Data.length(),10));
        }
        else
        {
            ui->lineEdit_SendLen->setText(QString::number((ui->lineEdit_SendData->text().length()
                                                           -ui->lineEdit_SendData->text().count(' '))/2,10));
        }
    }
    if(Send_Mode)
    {
        Other = NoZigBee_Data;
    }
    else
    {
        QString Data;
        QString a = ui->lineEdit_SendData->text();
        for(int i=0; i<a.length(); i++)                     //Hex转换为Ascii
        {
            int Data_Int = a.section(' ',i,i).toInt(&ok,16);
            Data += QString((Data_Int%256)&0xFF);
        }
        Other = Data.simplified();
    }
}

//LoRa数据组合
void MainWindow::Group_LoRa()
{
    bool ok;
    QString LoRa_Data[3];
    LoRa_Data[0] = "AT+SEND=";
    QString Id = ui->lineEdit_SendLen->text();
    QString Na = ui->lineEdit_SendNa->text();
    QString Send_Data = ui->lineEdit_SendData->text();
    QString Add = ui->lineEdit_Send_Cmd->text();
    QString Realse = ui->LoRa_lineEdit_release->text();
    int Send_sum = 0;
    if(Id.toInt(&ok,10) <= 15)
        Id = "0"+QString::number(Id.toInt(&ok,10),16).toUpper();
    else
        Id = QString::number(Id.toInt(&ok,10),16).toUpper();
    if(!Send_Mode)
    {
        Send_sum = (Send_Data.length()-Send_Data.count(' '))/2;
    }
    else
    {
        Send_sum = Send_Data.length();
    }
    int num1 = 0;
    if(ui->lineEdit_SendLen->text().isEmpty())
        num1 = 0;
    else
        num1 = 1;
    int num2 = 0;
    if(ui->LoRa_lineEdit_release->text().isEmpty())
        num2 = 0;
    else
        num2 = 1;
    int LoRa_Data_sum = Send_sum + (Add.length()-Add.count(' '))/2
                                + (Na.length()-Na.count(' '))/2
                                + num1 + num2;
    LoRa_Data[1] = QString::number(LoRa_Data_sum,10);
    ui->lineEdit_SendSop->setText(LoRa_Data[1].toUpper());
    LoRa.clear();
    LoRa_Data[2] = Id+" "+Add+" "+Na+" "+Realse+Get_Hex(Send_Data).toUpper();
    if(ui->lineEdit_SendSop->text().toInt() == 0)
        ui->lineEdit_SendSop->clear();
    if(Send_Mode)
    {
        LoRa = (Id+" "+Add+" "+Na+" "+Realse+Get_Hex(Send_Data).toUpper()).simplified();
    }
    else
    {
        LoRa = (Id+" "+Add+" "+Na+" "+Realse+" "+Send_Data.toUpper()).simplified();
    }
}

/*********************************************************************************************
* 名称:Group()
* 功能:将发送编辑框中的数据组合运算
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Group()
{
    bool ok;
    QString Data,Sum,Combine_Num,Data_Fcs;
    int Data_Int;
    char Data_Hex=0;
    QString SendApp_Data = ui->lineEdit_SendData->text();
    if(!ZigBee_AT_Test_OK)
    {
        ZigBeeData_Str.clear();
        QString ZigBee_Data = ui->lineEdit_SendData->text();
        if(Send_Mode)
        {
            if(ZigBee_Data.length() == 0)
            {
                ui->lineEdit_SendLen->clear();
                return ;
            }
            else
            {
               ui->lineEdit_SendLen->setText(QString::number(ZigBee_Data.length(),10));
            }
        }
        else
        {
            ui->lineEdit_SendLen->setText(QString::number((ui->lineEdit_SendData->text().length()
                                                           -ui->lineEdit_SendData->text().count(' '))/2,10));
        }
        if(!ZigBee_Data.isEmpty())
        {
            if(Send_Mode)
            {
                ZigBeeData_Str = ZigBee_Data;
            }
            else
            {
                QString Data;
                QString a = ui->lineEdit_SendData->text();
                for(int i=0; i<a.length(); i+=3)                    //Hex转Ascii
                {
                    int Data_Int = a.mid(i,2).toInt(&ok,16);
                    Data += QString((Data_Int%256)&0xFF);
                }
                ZigBeeData_Str = Data.simplified();
            }
        }
    }
    else
    {
        Group_Data[0] = ui->lineEdit_SendSop->text();
        Group_Data[2] = "29 00 02";                               //添加上位机发送的CMD及02
        Group_Data[3] = ui->lineEdit_SendNa->text();

        Group_Data[4] = ui->comboBox_Send_AppCmd->currentText().simplified();

        if(Send_Mode == true)
        {
            QString SendApp_Data_Str;
            for(int i=0; i<SendApp_Data.length(); i++)          //Ascii转Hex
            {
                QString Ascii = SendApp_Data.at(i);
                QByteArray SendApp_Data_Ascii = Ascii.toLatin1();
                QString SendApp_Data_Hex = QByteArray(SendApp_Data_Ascii.toHex());
                SendApp_Data_Str += " "+SendApp_Data_Hex;
            }
            Group_Data[5] = SendApp_Data_Str.simplified();
        }
        else
        {
            Group_Data[5] = ui->lineEdit_SendData->text().simplified();
        }

        //获取当前输入的数据长度
        int Num_Int = ((Group_Data[3]+Group_Data[4]+Group_Data[5]).length()-(Group_Data[3]+Group_Data[4]+Group_Data[5]).count(' '))/2+1;
        QString Num_Hex = QString::number(Num_Int,16);              //先将长度转换为十六进制字符串
        if(Num_Int <= 15)
            Num_Hex = '0'+Num_Hex;
        if(Num_Int > 3)
            ui->lineEdit_SendLen->setText(Num_Hex.toUpper());           //填入数据长度并大写
        Group_Data[1] = ui->lineEdit_SendLen->text();               //填入计算后的数据长度

        //计算校验数据
        for(int a=1; a<6; a++)                                      //将需要求异或的各个文本框中的内容组合
        {
            if(a==1 || a==5)
            {
                if(a==1)
                    Sum = Group_Data[a];
                else
                {
                    Sum = ' '+Group_Data[a];
                }
            }
            else
                Sum = ' '+Group_Data[a];
            Combine_Num += Sum;
        }
        Combine_Num = Combine_Num.simplified();                                   //删除数据前面或结尾的空格
        //求异或和
        for(int a=0; a<Num_Int+3; a++)
        {
            Data_Int = Combine_Num.section(' ',a,a).toInt(&ok,16);  //将组合数据分解
            Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
        }
        Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
        int Data_Fcs_Int = Data_Fcs.toInt(&ok,16);
        if(Data_Fcs_Int == 0)
            Data_Fcs = "00";
        else if(Data_Fcs_Int <= 15)
            Data_Fcs = '0'+Data_Fcs;
        ui->lineEdit_SendFcs->setText(Data_Fcs.toUpper());
        Group_Data[6] = Data_Fcs;

        //数据编译组合
        for(int a=0; a<7; a++)
        {
            if(a == 0)
                Data += Group_Data[a];
            else if(a == 5)
            {
                Data += ' '+Group_Data[a];
            }
            else
                Data += ' '+Group_Data[a];
        }
        ZigBeeData_Hex = Data.simplified().toUpper();
    }
}

//WiFi数据结合
void MainWindow::Group_WiFi()
{
    bool ok;
    Other.clear();
    QString WiFi_Data = ui->lineEdit_SendData->text();
    if(WiFi_Data == "")
    {
        ui->lineEdit_SendLen->clear();
    }
    else
    {
        if(Send_Mode)
        {
            ui->lineEdit_SendLen->setText(QString::number(WiFi_Data.length(),10));
        }
        else
        {
            ui->lineEdit_SendLen->setText(QString::number((ui->lineEdit_SendData->text().length()
                                                           -ui->lineEdit_SendData->text().count(' '))/2,10));
        }
    }
    if(Send_Mode)
    {
        Other = WiFi_Data;
    }
    else
    {
        QString Data;
        QString a = ui->lineEdit_SendData->text();
        for(int i=0; i<a.length(); i+=3)                     //Hex转换为Ascii
        {
            int Data_Int = a.mid(i,2).toInt(&ok,16);
            Data += QString((Data_Int%256)&0xFF);
        }
        Other = Data.simplified();
    }
}

/*********************************************************************************************
* 名称:SendNa_Add_Space()
* 功能:将SendNa中的数据自动大写并两个字符自动添加空格
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::SendNa_Add_Space()
{
    static int SendNa_num=0,space_number=0;            //当前发送字符数，当前空格数
    QString SendNa_text = ui->lineEdit_SendNa->text().toUpper();
    ui->lineEdit_SendNa->setText(SendNa_text);                  //将SendNa中的改为大写
    SendNa_num = SendNa_text.length();
    SendNa_num -= space_number;
    //自动添加空格操作
    if((SendNa_num%2 == 0) && (SendNa_num < 4) && (space_number < SendNa_num/2))
    {
        ui->lineEdit_SendNa->insert(" ");
    }
    space_number = ui->lineEdit_SendNa->text().count(' ');
}

/*********************************************************************************************
* 名称:SendNa_Add_Space()
* 功能:将SendNa中的数据自动大写并两个字符自动添加空格
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::SendCmd_Add_Space()
{
    static int SendCmd_num=0,space_number=0;            //当前发送字符数，当前空格数
    QString SendCmd_text = ui->lineEdit_Send_Cmd->text().toUpper();
    ui->lineEdit_Send_Cmd->setText(SendCmd_text);                  //将SendNa中的改为大写
    SendCmd_num = SendCmd_text.length();
    SendCmd_num -= space_number;
    //自动添加空格操作
    if((SendCmd_num%2 == 0) && (SendCmd_num < 4) && (space_number < SendCmd_num/2))
    {
        ui->lineEdit_Send_Cmd->insert(" ");
    }
    space_number = ui->lineEdit_Send_Cmd->text().count(' ');
}

/*********************************************************************************************
* 名称:SendData_Add_Space()
* 功能:将SendData中的数据自动大写并两个字符自动添加空格
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::SendData_Add_Space()
{
    static int Senddata_num=0,space_number=0;
    int Next_Hex_Cursor;
    int Last_Cursor;
    Last_Cursor = ui->lineEdit_SendData->cursorPosition();      //获取当前光标位置
    QString Senddata_text;
    if(Send_Mode == false)                                      //当当前发送数据显示模式为Hex时，每输入两个字符自动加空格
    {
        Senddata_text = ui->lineEdit_SendData->text();
        Senddata_num = Senddata_text.length();
        Senddata_num -= space_number;
        Next_Hex_Cursor = ui->lineEdit_SendData->cursorPosition();
        if(Last_Cursor != Next_Hex_Cursor)
        {
            ui->lineEdit_SendData->setCursorPosition(Last_Cursor);
        }
        //自动添加空格操作
        if((Senddata_num%2 == 0) && (space_number < Senddata_num/2))
        {
            ui->lineEdit_SendData->insert(" ");
        }
        space_number = ui->lineEdit_SendData->text().count(' ');
    }
    else
    {
        int Next_Ascii_Cursor = ui->lineEdit_SendData->cursorPosition();
        if(Last_Cursor != Next_Ascii_Cursor)                    //防止光标直接跳到最后
            ui->lineEdit_SendData->setCursorPosition(Last_Cursor);
    }
}

//协调器获取TYPE,PANID,CHANNEL
void MainWindow::Read_NetWord(bool mode)
{
    if(!mode)
    {
        QByteArray Read;
        //{TYPE=?,PANID=?,CHANNEL=?}
        QString str = "FE 1F 29 00 02 00 00 00 00 7B 54 59 50 45 3D 3F 2C 50 41 4E 49 44 3D 3F 2C 43 48 41 4E 4E 45 4C 3D 3F 7D 39";
        StringToHex(str,Read);                                 //先将发送框的内容转换为Hex型
        Serial.write(Read);
        Get_Time();
        str = "["+Time_str+"  ZigBee] <-- "+str;
        ui->listWidget_Rec->addItem(str);
        SendData_Count += Serial.bytesToWrite();
        ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
        Read_Flag = 2;
    }
    else
    {
        QString Read;       //AT+指令？\r\n
        switch(Read_Flag)
        {
        case 1: Read = "AT+MAC?\r\n"; break;
        case 2: Read = "AT+LOGICALTYPE?\r\n"; break;
        case 3: Read = "AT+CHANNEL?\r\n"; break;
        case 4: Read = "AT+PANID?\r\n"; break;
           default:return;
        }
        Serial.write(Read.toStdString().c_str());
        Get_Time();
        Read = "["+Time_str+"  ZigBee] <-- "+Read.left(Read.length()-2);
        ui->listWidget_Rec->addItem(Read);
        Serial.flush();
        SendData_Count += Serial.bytesToWrite();
        ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
    }
}

//ZigBee协调器获取MAC
void MainWindow::ZigBee_Read_MAC()
{
    QByteArray Read;
    QString Send_Data = "FE 07 29 00 02 00 00 01 02 00 00 2F";
    StringToHex(Send_Data,Read);                                 //先将发送框的内容转换为Hex型
    Serial.write(Read);
    Get_Time();
    Send_Data = "["+Time_str+"  ZigBee] <-- "+Send_Data;
    ui->listWidget_Rec->addItem(Send_Data);
    SendData_Count += Serial.bytesToWrite();
    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
    Read_Flag = 1;
}

//ZigBee AT测试函数
void MainWindow::ZigBee_AT_Test()
{
    uartData.clear();
    at_error_cnt = 0;
    QString Send_Data = "ATE0\r\n";
    Serial.write(Send_Data.toStdString().c_str());
    Get_Time();
    Send_Data = "["+Time_str+"  ZigBee] <-- "+Send_Data.left(Send_Data.length()-2);
    ui->listWidget_Rec->addItem(Send_Data);
    SendData_Count += Serial.bytesToWrite();
    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
    ZigBee_AT_Test_OK = true;
    Test_OK = true;
    Read_Flag = 0;
    QTimer::singleShot(1000,this,SLOT(ZigBee_Make_Type()));
    QTimer::singleShot(3000,this,SLOT(Del_Read_Write_flag()));
}

//ZigBee确定类型函数
void MainWindow::ZigBee_Make_Type()
{
    if(ZigBee_AT_Test_OK)
    {
        Read_NetWord(0);     //协调器
        Set_Label(2);
    }
    else
    {
        uartData.clear();
        Read_Flag = 4;
        Read_NetWord(1);
        ui->comboBox_Mold->setEnabled(true);
    }
}

/*********************************************************************************************
* 名称:Write_NetWord()
* 功能:修改PANID,CHANNEL信息
* 参数:无
* 返回:无
*********************************************************************************************/
void MainWindow::Write_NetWord()
{
    bool ok;
    QString Panid,Panid_Hex;
    QString Channel,Channel_Hex;
    QString table,Data_Fcs,str,buff[6];
    QByteArray Write;
    char Data_Hex = 0;
    Panid = ui->lineEdit_Panid->text();
    int Panid_Int = Panid.toInt(&ok,10);
    Channel = ui->comboBox_Channel->currentText();
    if((Panid.isEmpty() || Channel.isEmpty()) == true)
    {
        QMessageBox::warning(this,"提示","PANID或CHANNEL不能为空!");
    }
    else if(((Panid_Int>16383 || Panid_Int<1) == true) /*&& (Panid_Int != 65535)*/)
    {
        QMessageBox::warning(this,"提示","PANID允许范围为1-16383之间");
    }
    else
    {
        if(!ZigBee_AT_Test_OK)
        {
            QString PANID_str = ui->lineEdit_Panid->text();
            QString CHANNEL_str = ui->comboBox_Channel->currentText();
            int Type_sum = ui->comboBox_Mold->currentIndex();
            QString Write;
            if(Type_sum == 0)
                QMessageBox::warning(this,"提示","路由和终端节点不支持修改为协调器！");
            else
            {
                if(Write_Flag == 0)
                    Write_Flag = 3;
                switch(Write_Flag)
                {
                case 1: Write = "AT+LOGICALTYPE="+QString::number(Type_sum,10)+"\r\n"; break;
                case 2: Write = "AT+CHANNEL="+CHANNEL_str+"\r\n"; break;
                case 3: Write = "AT+PANID="+PANID_str+"\r\n"; break;
                }
                Serial.write(Write.toStdString().c_str());
                Get_Time();
                Write = "["+Time_str+"  ZigBee] <-- "+Write.left(Write.length()-2);
                ui->listWidget_Rec->addItem(Write);
                SendData_Count += Serial.bytesToWrite();
                ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
            }
        }
        else
        {
            for(int i=0; i<Panid.length(); i++)
            {
                QString Ascii = Panid.at(i);
                QByteArray SendApp_Data_Ascii = Ascii.toLatin1();
                QString SendApp_Data_Hex = QByteArray(SendApp_Data_Ascii.toHex());
                Panid_Hex += " "+SendApp_Data_Hex;
            }
            for(int i=0; i<Channel.length(); i++)
            {
                QString Ascii = Channel.at(i);
                QByteArray SendApp_Data_Ascii = Ascii.toLatin1();
                QString SendApp_Data_Hex = QByteArray(SendApp_Data_Ascii.toHex());
                Channel_Hex += " "+SendApp_Data_Hex;
            }
            buff[0] = "FE "; //{PANID=
            buff[1] = " 29 00 02 00 00 00 00 7B 50 41 4E 49 44 3D"+Panid_Hex+" 2C ";                             //PANID,中的数据
            buff[2] = "4E 4F 44 45 5F 54 59 50 45 3D ";
            if(ui->comboBox_Mold->currentIndex() == 0)
                buff[2] += "30";
            else if(ui->comboBox_Mold->currentIndex() == 1)
                buff[2] += "31";
            else
                buff[2] += "32";
            buff[3] = " 2C 43 48 41 4E 4E 45 4C 3D";                    //,CHANNEL=
            buff[4] = Channel_Hex+" 7D ";                               //}
            QString buf = buff[1]+buff[2]+buff[3]+buff[4];
            int data = (buf.length()-buf.count(' ')-4)/2;
            table = buff[0]+QString::number(data,16)+buf;

            //求异或和
            for(int a=0; a<(QString::number(data,16)+buf).length(); a+=3)
            {
                int Data_Int = (QString::number(data,16)+buf).mid(a,2).toInt(&ok,16);      //将组合数据分解
                Data_Hex ^= (Data_Int%256)&0xFF;                        //将十进制转换为十六进制异或
            }
            Data_Fcs = QString::number(Data_Hex,16).right(2);           //提取异或后的结果最后两位
            int Data_Fcs_Int = Data_Fcs.toInt(&ok,16);
            if(Data_Fcs_Int == 0)
                Data_Fcs = "00";
            else if(Data_Fcs_Int <= 15)
                Data_Fcs = '0'+Data_Fcs;
            buff[5] = Data_Fcs;                                         //校验和
            str = table+buff[5];
            StringToHex(str,Write);
            Serial.write(Write);
            Get_Time();
            str = "["+Time_str+"  ZigBee] <-- "+str;
            ui->listWidget_Rec->addItem(str);
            SendData_Count += Serial.bytesToWrite();
            ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
            QTimer::singleShot(1000,this,SLOT(Read_Netslot()));
        }
    }
}

//写入ZigBee配置一秒后调用读取函数
void MainWindow::Read_Netslot()
{
    Read_NetWord(0);
}

QString MainWindow::addHandleLog(QString SendStr)
{
    QString str = "";
    if(Mode == 1)
        str = "["+Time_str+"  ZigBee] <-- ";
    else if(Mode == 2)
        str = "["+Time_str+"   LoRa ] <-- ";
    else if(Mode == 3)
        str = "["+Time_str+"   LTE  ] <-- ";
    else if(Mode == 4)
        str = "["+Time_str+"  NB_IoT] <-- ";
    else if(Mode == 5 || Mode == 9)
        str = "["+Time_str+"   WiFi ] <-- ";
    else if(Mode == 6)
        str = "["+Time_str+"   BLE  ] <-- ";
    else if(Mode == 7)
        str = "["+Time_str+" LoRaWAN] <-- ";
    else if(Mode == 8)
        str = "["+Time_str+"  CAT1  ] <-- ";
    SendStr = str + SendStr;
    return SendStr;
}

//ZigBee发送数据长度
void MainWindow::ZigBee_Send_Length()
{
    QString Send_Data = "AT+SEND=";
    Send_Flag = true;
    Send_Data += ui->lineEdit_SendLen->text()+"\r\n";
    Serial.write(Send_Data.toStdString().c_str());
    Get_Time();
    Send_Data = addHandleLog(Send_Data);
    ui->listWidget_Rec->addItem(Send_Data.left(Send_Data.length()-2));
    SendData_Count += Serial.bytesToWrite();
    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
}

/****************LoRa******************/
//二维码
void MainWindow::Out_Code()
{
//    if(Mode == 3)
//        MainWindow::Code_Str = "{\"id\":\""+ui->G_lineEdit_ID->text()+
//                "\",\"key\":\""+ui->G_lineEdit_Key->text()+
//                "\",\"server\":\""+ui->G_lineEdit_Sever->text()+"\"}";
//    else if(Mode == 8)
//        MainWindow::Code_Str = "{\"id\":\""+ui->CAT1_lineEdit_ID->text()+
//                "\",\"key\":\""+ui->CAT1_lineEdit_Key->text()+
//                "\",\"server\":\""+ui->CAT1_lineEdit_Sever->text()+"\"}";
//    else if(Mode == 9)
//        MainWindow::Code_Str = "{\"id\":\""+ui->ESP8266_lineEdit_AID->text()+
//                "\",\"key\":\""+ui->ESP8266_lineEdit_AKey->text()+
//                "\",\"server\":\""+ui->ESP8266_lineEdit_SIP->text()+"\"}";
//    else
//        MainWindow::Code_Str = "{\"id\":\""+ui->NB_lineEdit_ID->text()+
//                "\",\"key\":\""+ui->NB_lineEdit_Key->text()+
//                "\",\"server\":\""+ui->NB_lineEdit_Sever->text()+"\"}";
//    emit Display_Code();
}


void MainWindow::on_chk_sendEnd_clicked(bool checked)
{
    if(checked)
    {
        ui->le_sendTime->show();
        ui->pushButton_Send->setText("开始发送");
        if(ui->le_sendTime->text().isEmpty())
        {
            ui->le_sendTime->setText("1000");
        }
    }
    else
    {
        ui->le_sendTime->hide();
        autoTime->stop();
        autoSendFlag = false;
        ui->le_sendTime->setReadOnly(false);
        ui->pushButton_Send->setText("发送");
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    bool ok;
    bool flag = ui->chk_sendEnd->isChecked();
    if(flag)
    {
        if(autoSendFlag)
        {
            autoSendFlag = false;
            ui->pushButton_Send->setText("开始发送");
            ui->le_sendTime->setReadOnly(false);
            autoTime->stop();
        }
        else
        {
            autoSendTime = ui->le_sendTime->text().toInt(&ok, 10);
            if(autoSendTime < 100 || autoSendTime > 1200000)
            {
                QMessageBox::warning(this, "严重警告", "自动发送时间允许范围100-1200000ms(20分钟)之间！");
            }
            else
            {
                autoSendFlag = true;
                ui->pushButton_Send->setText("停止发送");
                ui->le_sendTime->setReadOnly(true);
                autoTime->start(autoSendTime);
            }
        }
    }
    else
    {
        WriteUart();
    }
}


//WiFi发送数据
void MainWindow::WiFi_Send_Data()
{
    QString Send_Data;
    QString Send_Data_Hex;
    if(Mode == 1)
        Send_Data = ZigBeeData_Str;
    else
        Send_Data = Other;
    for(int i=0; i<Send_Data.length(); i++)               //Ascii转Hex
    {
        QString Ascii = Send_Data.at(i);
        QByteArray RecApp_Data_Ascii = Ascii.toLatin1();
        QString RecApp_Data_Hex = QByteArray(RecApp_Data_Ascii.toHex());
        Send_Data_Hex += " "+RecApp_Data_Hex;
    }
    Send_Data_Hex = Send_Data_Hex.simplified().toUpper();
    Serial.write(Send_Data.toStdString().c_str());
    Get_Time();
    Send_Data_Hex = addHandleLog("[Hex]" + Send_Data_Hex);
    ui->listWidget_Rec->addItem(Send_Data_Hex);
    SendData_Count += Serial.bytesToWrite();
    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
}

//WiFI配置保存及重启
void MainWindow::WiFi_Save_Reset(bool mode)
{
    QString Save = "AT+ENVSAVE\r\n";
    QString Reset = "AT+RESET\r\n";
    if(mode)
    {
        Serial.write(Reset.toStdString().c_str());
        Get_Time();
        Reset = addHandleLog(Reset);
        ui->listWidget_Rec->addItem(Reset.left(Reset.length()-2));
    }
    else
    {
        Save_Flag = 1;
        Serial.write(Save.toStdString().c_str());
        Get_Time();
        Save = addHandleLog(Save);
        ui->listWidget_Rec->addItem(Save.left(Save.length()-2));
    }
    SendData_Count += Serial.bytesToWrite();
    ui->label_CountSendNum->setText(QString::number(SendData_Count,10));
}

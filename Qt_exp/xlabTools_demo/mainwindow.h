#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QtCore>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QToolTip>

#define BIT_0                   (1<<0)
#define BIT_1                   (1<<1)
#define BIT_2                   (1<<2)
#define BIT_3                   (1<<3)
#define BIT_4                   (1<<4)
#define BIT_5                   (1<<5)
#define BIT_6                   (1<<6)
#define BIT_7                   (1<<7)
#define ARRAY(x)                (sizeof(x)/sizeof(x[0]))
#define INFO_LISTWIDGET_UPDATE(info)        {ui->info_listWidget->addItem(info); \
                                            ui->info_listWidget->scrollToBottom();}
#define CLEAN_RECV_DATA             {recv_raw_whole.clear();recv_data_whole.clear();recv_show_whole.clear();}
#define RECV_CMD_MAX_LEN            256

#define ZIGBEE_RES_TIMER_START      {zigbee_res_timer->start(1000);}
#define ZIGBEE_RECV_CMD_OK          {zigbee_Sem.release();}
#define ZIGBEE_CMD_HAVE_DATA        {zigbee_flag|=BIT_6;}
#define ZIGBEE_CMD_NO_DATA          {zigbee_flag&=~BIT_6;}

/* zigbee */
// 网络短地址范围 0xFFFF
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /** 串口管理 **/
    QSharedPointer<QSerialPort> Serial;
    QByteArray recv_raw_whole="";
    QString recv_data_whole="";
    QString recv_show_whole="";

    bool m_display_recv=0;              // 显示接收数据的显示
    unsigned long m_Rec_Num = 0;        // 接收数
    unsigned long m_Send_Num = 0;       // 发送数

    /** zigbee **/
    QSemaphore zigbee_Sem{1};           // 指令信号量
    unsigned short zigbee_flag;         // zigbee标志位
    // 位0：是否需要读配置
    // 位1：保留
    // 位2：是否测试过AT指令
    // 为3：是否支持AT指令
    // 位4：保留
    // 位5：保留
    // 位6：指令有无数据帧，读写标志位
    unsigned short zigbee_count = 0;         // zigbee标志位

    QTimer* zigbee_cycle_timer;         // 周期定时器
    QTimer* zigbee_res_timer;           // 指令回复超时 定时器.start会复位

    // 其他指令就是

    /** 公用部分 **/
    QTime m_time;
    QString m_Time_str;
    QString m_rf_type;

    /** 输入限制 **/
    QRegExp Regexp_Na_Ascii,Regexp_Na_Hex;
    QRegExp Regexp_Data_Ascii,Regexp_Data_Hex;

private slots:
    /** 公共状态更新 **/
    void init_app_ui();
    void init_app_timer();
    void get_time();

    /** 串口管理接口函数 **/
    void serial_get_availablePorts();  // 获取有效的串口
    bool serial_open();             // 打开串口
    void serial_close();            // 关闭串口
    void serial_ui_update();        // 封装如上函数

    void serial_write(QByteArray &hex,QString display_string="",QString notes="");
    int serial_read();

    /** 字符处理功能 **/
    void StringToHex(QString str,QByteArray &send_data);
    char xor_count(QByteArray array,unsigned char s1,unsigned char s2);
    QString Add_Space(int x, QString z);
    int Get_DelSpaceString_Length(QString str);

    /** zigbee 相关功能函数 **/
    void zigbee_app_init();
    void zigbee_read_config();
    void zigbee_read_config_handle();
    void zigbee_read_config_timeout();
    void zigbee_data_handle(QByteArray& data,QString& display_data);
    void zigbee_write_config();
    void zigbee_read_config_reset();

    void zigbee_ui_state_update();
    /** ui槽函数 **/
    // 软件设置 组box
    void on_pushButton_OpenCom_clicked();
    void on_pushButton_OpenRec_clicked();
    // 配置 组box
    void on_pushButton_Read_clicked();
    void on_pushButton_Write_clicked();
    // 数据显示设置 组box

    // 数据清除 组box
    void on_pushButton_DelRec_clicked();
    void on_pushButton_DelSend_clicked();

    // 数据模拟 组box
    void on_lineEdit_SendNa_textEdited(const QString &arg1);
    void on_lineEdit_SendNa_inputRejected();
};

#endif // MAINWINDOW_H

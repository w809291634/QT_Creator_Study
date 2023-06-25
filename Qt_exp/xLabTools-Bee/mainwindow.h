﻿#ifndef MAINWINDOW_H
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
#include <iostream>
#include <QListWidgetItem>
#include <QKeyEvent>

#define qdebug                  qDebug() << __LINE__
#define BIT_0                   (1<<0)
#define BIT_1                   (1<<1)
#define BIT_2                   (1<<2)
#define BIT_3                   (1<<3)
#define BIT_4                   (1<<4)
#define BIT_5                   (1<<5)
#define BIT_6                   (1<<6)
#define BIT_7                   (1<<7)
#define BIT_8                   (1<<8)
#define BIT_9                   (1<<9)

#define INFO_LISTWIDGET_MAX_HISTORY_NUM  100
#define ARRAY(x)                (sizeof(x)/sizeof(x[0]))
#define INFO_LISTWIDGET_UPDATE(info)        do{ \
                                                if(m_display_recv){ \
                                                    ui->info_listWidget->addItem(info); \
                                                    if(ui->info_listWidget->count()>INFO_LISTWIDGET_MAX_HISTORY_NUM) \
                                                        ui->info_listWidget->takeItem(0); \
                                                    ui->info_listWidget->scrollToBottom(); \
                                                } \
                                            }while(0);
#define CLEAN_RECV_DATA             {recv_data_whole.clear();recv_show_whole.clear();}
#define RECV_CMD_MAX_LEN            256
#define MAX_STORAGE_NUM             10

#define ZIGBEE_RES_TIMER_START      {zigbee_res_timer->start(1000);}
#define ZIGBEE_RES_TIMER_START_2    {zigbee_res_timer->start(2000);}
#define ZIGBEE_RECV_CMD_OK          {zigbee_Sem.release();}
#define ZIGBEE_CMD_HAVE_DATA        {zigbee_flag|=BIT_6;}
#define ZIGBEE_CMD_NO_DATA          {zigbee_flag&=~BIT_6;}
#define ZIGBEE_MIN_SEND_CYCLE       200

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
    QString current_cmd="";             // 储存当前的发送命令
    QString recv_data_whole="";
    QString recv_show_whole="";

    bool m_display_recv=0;              // 显示接收数据的显示
    unsigned long m_Rec_Num = 0;        // 接收数
    unsigned long m_Send_Num = 0;       // 发送数

    /** zigbee **/
    QSemaphore zigbee_Sem{1};           // 指令信号量
    bool zigbee_at_flag=false;          // zigbee是否支持AT指令，每次读刷新
    unsigned short zigbee_flag;         // zigbee标志位
    // 位0：是否需要读取配置
    // 位1：是否进行写入配置
    // 位2：是否测试过AT指令
    // 为3：是否支持AT指令(废弃)
    // 位4：保留
    // 位5：保留
    // 位6：指令有无数据帧，读写标志位，1表示读 0表示写
    // 位7：保留 位8：保留
    // 位9：读配置完成
    unsigned short zigbee_count = 0;    // 发送指令的计数

    QString node_send_data="";          // AT+SEND 发送的ascii数据

    QTimer* zigbee_cycle_timer;         // 周期定时器
    QTimer* zigbee_res_timer;           // 指令回复超时 定时器.start会复位
    QTimer* zigbee_send_timer;

    // 信息储存
    QList<QString> zigbee_na_list;
    int zigbee_na_index= 0;
    QList<QString> zigbee_send_list;
    int zigbee_send_index= 0;

    /** 公用部分 **/
    QTime m_time;
    QString m_Time_str;
    QString m_rf_type;

    /** 输入限制 **/
    QRegExp Regexp_Na_Ascii,Regexp_Na_Hex;
    QRegExp Regexp_Data_Ascii,Regexp_Data_Hex;
    QRegExp Panid_RegExp;

private slots:
    /** 公共状态更新 **/
    void init_app_ui();
    void init_app_timer();
    void get_time();
    virtual void keyPressEvent(QKeyEvent *event);

    /** 串口管理接口函数 **/
    void serial_get_availablePorts();  // 获取有效的串口
    bool serial_open();             // 打开串口
    void serial_close();            // 关闭串口
    void serial_ui_update();        // 封装如上函数

    void serial_write(QByteArray &hex,QString display_string="",QString notes="");
    int serial_read();
    void serial_error(QSerialPort::SerialPortError serialPortError);

    /** 字符处理功能 **/
    void StringToHex(QString str,QByteArray &send_data);
    char xor_count(QByteArray array,unsigned char s1,unsigned char s2);
    uint8_t xor_count_u8(QByteArray array,uint8_t s1,uint8_t s2);
    QString Add_Space(int x, QString z);
    int Get_DelSpaceString_Length(QString str);
    QString HexQString2AsciiString(const QString &HexQString);
    QString AsciiString2HexQString(const QString &qString);

    /** zigbee 相关功能函数 **/
    void zigbee_app_init();
    void zigbee_ui_state_update();
    void zigbee_calculate_len_fcs();
    void zigbee_set_node_label();
    void zigbee_set_coordinator_label();

    void zigbee_cmd_reset(bool update_ui=true);
    void zigbee_read_config(bool at_test=true);
    void zigbee_read_config_NoAtTest();
    void zigbee_read_config_handle();
    void zigbee_data_handle(QByteArray& data,QString& display_data);
    void zigbee_cmd_timeout();
    void zigbee_write_config();
    void zigbee_write_config_handle();

    // 储存信息
    void zigbee_na_list_add(const QString& item);
    void zigbee_na_list_index_reset();
    QString zigbee_na_list_get(const int& flag);

    void zigbee_send_list_add(const QString& item);
    void zigbee_send_list_index_reset();
    QString zigbee_send_list_get(const int& flag);

    /** ui槽函数 **/
    // 软件设置 组box
    void on_pushButton_OpenCom_clicked();
    void on_pushButton_OpenRec_clicked();
    // 配置 组box
    void on_pushButton_Read_clicked();
    void on_pushButton_Write_clicked();
    // 数据显示设置 组box
    void on_info_listWidget_itemClicked(QListWidgetItem *item);
    void on_comboBox_Set_Rec_currentIndexChanged(int index);
    void on_comboBox_Set_Send_currentIndexChanged(int index);

    // 数据清除 组box
    void on_pushButton_DelRec_clicked();
    void on_pushButton_DelSend_clicked();
    void on_comboBox_SendAppCmd_currentIndexChanged(int index);

    // 数据模拟 组box
    void on_lineEdit_SendNa_textEdited(const QString &arg1);
    void on_lineEdit_SendNa_inputRejected();
    void on_lineEdit_SendNa_returnPressed();

    void on_send_data_btn_clicked();
    void on_SendData_ledit_textEdited(const QString &arg1);
    void on_SendData_ledit_inputRejected();

    void on_send_cycle_cBox_clicked(bool checked);
    void on_SendTime_ledit_textChanged(const QString &arg1);
    // 其他
    void on_count_num_clear_btn_clicked();
};

#endif // MAINWINDOW_H
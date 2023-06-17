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
#include <QTime>
#include <QToolTip>
#include <QMouseEvent>

#define BIT_0               (1<<0)
#define BIT_1               (1<<1)
#define BIT_2               (1<<2)
#define BIT_3               (1<<3)
#define ARRAY(x)            (sizeof(x)/sizeof(x[0]))

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
    QString m_hexdata_space;    // 串口接收数据（带空格hex格式字符串形式）
    QByteArray m_rawdata;       // 串口接收数据

    bool m_display_recv=0;          // 显示接收数据的显示
    unsigned long m_Rec_Num = 0;    // 接收数
    unsigned long m_Send_Num = 0;   // 发送数

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
    void get_time();

    /** 串口管理接口函数 **/
    void serial_get_availablePorts();  // 获取有效的串口
    bool serial_open();             // 打开串口
    void serial_close();            // 关闭串口
    void serial_ui_update();        // 封装如上函数

    void serial_write(QByteArray &hex,QString display_string="");
    int serial_read();

    /** 字符处理功能 **/
    void StringToHex(QString str,QByteArray &send_data);
    char xor_count(QByteArray array,unsigned char s1,unsigned char s2);
    QString Add_Space(int x, QString z);

    /** zigbee 相关功能函数 **/
    void zigbee_get_type();
    void zigbee_read_config();
    void zigbee_write_config();
    void zigbee_data_handle();


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

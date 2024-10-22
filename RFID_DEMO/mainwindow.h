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

namespace Ui {
class MainWindow;
}

#define BIT_0               (1<<0)
#define BIT_1               (1<<1)
#define BIT_2               (1<<2)
#define BIT_3               (1<<3)
#define MAX_IC_NUM          30
#define ARRAY(x)            (sizeof(x)/sizeof(x[0]))

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init_ui(void);
    QString Add_Space(int x, QString z);
    char xor_count(QByteArray array,unsigned char s1,unsigned char s2);
    void StringToHex(QString str,QByteArray &send_data);

    /* 时间管理 */
    QString m_Time_str;
    QTimer* clear_timer;
    QString ver = "180929";

    /* 串口管理 */
    QSharedPointer<QSerialPort> Serial;
    QString m_Com;              // 已打开的串口号
    QString m_Baud;             // 已打开的串口波特率
    QString m_RecData;          // 串口接收的数据，字符串形式
    bool m_Com_Open=false;      // 串口打开标志位

    bool init_serial();
    void serial_ui_update();
    bool serial_close();
    bool serial_open();
    void uart_data_handle(QString &uart_data,QByteArray &uart_data_hex);

    /* ID卡 基础界面 */
    QString m_ID_Card_Num;
    char m_ID_flag=0;             // 位0:读ID命令 位1:写ID命令

    /* IC卡 基础界面 */
    QString m_IC_Card_Num;
    char m_IC_flag=0;               // 位0:寻卡指令
                                    // 位1:读扇区指令
                                    // 位2:写扇区指令
                                    // 位3:修改密码指令
    char m_IC_continuous=0;
    QString IC_Card[MAX_IC_NUM];     //IC寻卡卡号缓存区

    /* 调试管理 */
    bool m_show_debug_page;
    bool m_show_debug_data;
    unsigned long m_Rec_Num = 0;   //接收数
    unsigned long m_Send_Num = 0;  //发送数


private slots:
    /* 时间管理 */
    void get_time();
    void clear_text_label();
    void on_version_update_btn_clicked();

    /* 串口管理 */
    void get_com();
    void serial_write(QString &str_data,QByteArray &data,QString notes);
    int serial_read();
    void on_serial_open_btn_clicked();

    /* 调试管理 */
    void on_show_debug_btn_clicked();
    void on_clear_debug_btn_clicked();
    void on_rec_stop_btn_clicked();
    void on_del_cnt_btn_clicked();

    /* 页面切换 */
    void on_IDcard_page_btn_clicked();
    void on_ICcard_page_btn_clicked();

    /* ID卡 基础界面 */
    void on_ID_read_btn_clicked();
    void on_ID_write_btn_clicked();
    void on_ID_data_clear_btn_clicked();

    void on_ID_EM4100_rbtn_clicked();
    void on_ID_T5557_rbtn_clicked();

    /* IC卡 基础界面 */
    void on_passA_rbtn_clicked();
    void on_passB_rbtn_clicked();
    void on_sector_cbox_currentIndexChanged(int index);
    void on_block_cbox_currentIndexChanged(int index);
    // 寻卡指令
    void on_one_shot_search_card_clicked();
    void on_search_card_clear_clicked();
    void on_continuous_search_card_clicked(bool checked);
    void IC_Card_continuous_search();
    // 读写块指令
    void on_sysblock_read_btn_clicked();
    void on_sysblock_write_btn_clicked();
    void on_sysblock_clear_btn_clicked();
    // 修改命令命令
    void on_pass_modify_btn_clicked();
};


#endif // MAINWINDOW_H

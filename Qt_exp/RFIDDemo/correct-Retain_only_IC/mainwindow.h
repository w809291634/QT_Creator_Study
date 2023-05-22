#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QInputDialog>         //字符串弹窗
#include <QStandardItemModel>

#include <synchapi.h>   //延时
#include <QCloseEvent>  //关闭事件
#include <QPushButton>

#include <QDebug>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *timer_read = new QTimer(this);
    QTimer *IC_timer_read = new QTimer(this); //IC寻卡定时器
    QTimer *IC_time_Mess = new QTimer(this);//IC读取用户信息

    QStandardItemModel *model_ID = new QStandardItemModel();
    QStandardItemModel *model_Data = new QStandardItemModel();

    bool SetUart();
    char ConvertHexChar(char ch);
    void StringToHex(QString str,QByteArray &send_data);
    QString Get_Xor(unsigned char x, unsigned char y, QString z);
    QString Add_Space(unsigned char x, QString z);
    void closeEvent(QCloseEvent *event);  //关闭事件函数
    //ID考勤系统
    void ID_Analysis();
    //IC
    void Change_Money(unsigned long x);
    void Money_Down();

    unsigned char Mode = 1;
    char Name[10][50];      //储存卡号对应的员工信息
    QString RecData_ID;         //卡号
    QString RecData;
    QString ID[10];             //储存卡号
    unsigned int Sign_Num[10] = {0,0,0,0,0,0,0,0,0,0};//打卡次数
    QString ID_Name;        //员工姓名
    int ID_Num = 0;         //员工数量
    QString Time_str;
    QTime time;
    bool IsMorn;
    bool IsOpen;
    bool IsShow;
    bool IsStop;
    unsigned char Show_Mode = 0;
    void ID_CheckMode(bool mode);
    void G_Write_AppInfo(unsigned char h);

    QString get_Data(QString lastData, unsigned char lastLen, unsigned char addLen);
    void Uart_DataHandle(unsigned char lastLen, unsigned char addLen);

private:
    Ui::MainWindow *ui;
    QString ver = "180929";
    QRegExp Regexp_Pass,Regexp_Sys,Regexp_Num,Regexp_Num_Card,Regexp_ETC_TxL,Regexp_ETC_App;
    QRegExp G_Regexp_Panid, G_Regexp_Write;
    QRegExp ETC_Regexp_Write_RFU, ETC_Regexp_Write_EPC, ETC_Regexp_Write_TID, ETC_Regexp_Write_USER;
    QSerialPort Serial;
    QString mCom;
    QString mBaud;
    //保存设置的时间
    QTime Time_Morn_Up;
    QTime Time_Morn_Down;
    QTime Time_Afte_Up;
    QTime Time_Afte_Down;
    //初始化的时间
    QTime Time_Morn_Up_Last;
    QTime Time_Morn_Down_Last;
    QTime Time_Afte_Up_Last;
    QTime Time_Afte_Down_Last;

    //ETC应用界面停车时间
    QTime Time_Stop_Start;
    QTime Time_Stop_Over;

private slots:
    void Get_Com();
    void OpenUart_Clicked();
    void Get_Time();
    void ReadUart();
    void Show_Data();
    void Set_Show_Mode();
    void Stop_Rec();
    void Del_Num();
    void Change_ID();
    void Change_IC();
    void About_up();

    //ID考勤系统
    bool Message();
    void Time_Save();
    void ID_Change_Mode();
    void ID_Open_Door();
    void ID_EM4100();
    void ID_Read_Card();
    void ID_Write_Card();

    //公交非接触IC卡
    void IC_Del_Card();
    void IC_Send_Look();
    void IC_Begin_Look();
    void IC_Data_Look();
    void IC_Pass_Change();
    void IC_Write_Region();
    void IC_Read_Region();

    void IC_Read_Mess();
    void Mode_Change();
    void IC_Read_Mess_flag();
    void Money_Up();
    void Down_Money_Save();

    void IC_Ticket_Reset();
    void IC_App_Ticket_Save();
    void IC_App_Up_Money();
    void IC_App_Send_Balance();
    void IC_App_Down_Money();

    void IC_Change_Pass();
    void IC_Switch_Mode();

    void on_IC_radioButton_Pass_B_clicked(bool checked);
    void on_IC_radioButton_Pass_A_clicked(bool checked);
    void on_IC_lineEdit_Sys_Write_textChanged(const QString &arg1);
    void on_IC_comboBox_Sys_Lump_activated(int index);
    void on_IC_comboBox_Sys_Region_activated(int index);
};

#endif // MAINWINDOW_H

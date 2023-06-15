#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>  //对话框
#include <string>
#include <QTimer>
#include <QDateTime>
#include <QStatusBar>
#include <QLabel>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QThread>
#include "updata/updata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer = new QTimer(this);
    QTimer *autoTime = new QTimer(this);
    QByteArray RecData;
    upData *up;
    bool SetUart();
    void mousePressEvent(QMouseEvent *event);
    QString Get_Xor(unsigned char x, unsigned char y, QString z);
    void Group();
    void Group_LoRa();
    void Group_WiFi();
    void Group_NoZigBee();
    char ConvertHexChar(char ch);
    QString Get_Hex(QString str);
    QString Add_Space(int x, QString z);
    void StringToHex(QString str,QByteArray &send_data);
    void Set_Label(unsigned char mode);
    QPixmap createQRCode(const QString &text);

    void ZigBee_Data_Analysis();
    void Read_NetWord(bool mode);
    void ZigBee_Read_MAC();
    static QString ChmData_Str;
    static QString Code_Str;
    unsigned char Mode = 1;  //当前的模式
    QString Time_str;

public slots:
    void firstVerUp();
private:
    Ui::MainWindow *ui;
    QFile *file = new QFile;
    QSerialPort Serial;
    QLabel mLabel;
    QTime time;
    int autoSendTime = 1000;
    bool autoSendFlag;
    bool IsOpen;
    bool IsRecv;
    bool Rec_Mode;
    bool Send_Mode;

    QString ZigBeeData_Str;
    QString ZigBeeData_Hex;
    QString LoRa;
    QString Other;

    QString mCom;
    QString mBaud;
    QString Group_Data[7];
    QRegExp Regexp_Cmd,Regexp_Na,Regexp_Data_Hex,Regexp_Data_Ascii,Regexp_Panid,Regexp_LoRa_ID;
    QRegExp Regexp_G_ID,Regexp_G_KEY,Regexp_G_IP;
    QRegExp Regexp_LoRa_PS,Regexp_LoRa_FP,Regexp_LoRa_HOP,Regexp_LoRa_Release;
    QRegExp Regexp_WiFi_SSID,Regexp_WiFi_PSK,Regexp_WiFi_IP;
    QRegExp Regexp_LoRaWAN_JOINEUI,Regexp_LoRaWAN_KEY;

    QString addHandleLog(QString SendStr);
    void RWControlChange(bool enable);
signals:
    void Display_Code();
    void check_upFirst();
    void check_upData();
    void setLoRaWANChm();
private slots:
    void WiFi_Save_Reset(bool mode);
    void WiFi_Send_Data();

    void Get_Time();
    void saveFileSlot();
    void Write_NetWord();
    void OpenUart_Clicked();
    void OpenRecv_Clicked();
    void ReadUart();
    void Analysis();
    void About_Up();

    void SendNa_Add_Space();
    void SendData_Add_Space();
    void SendCmd_Add_Space();
    void WriteUart();
    void Del_SendData();
    void Del_RecData();
    void Get_Com();
    void Set_RecMode();
    void Set_SendMode();
    void Del_Num();

    void Change_ZigBee();
    void Mode_Group();

    void ZigBee_AT_Test();
    void ZigBee_Make_Type();
    void ZigBee_Send_Length();

    void Out_Code();
    void Read_Parameter();

    void Read_Netslot();
    void Del_Read_Write_flag();

    void on_chk_sendEnd_clicked(bool checked);
    void on_pushButton_Send_clicked();
};

#endif // MAINWINDOW_H

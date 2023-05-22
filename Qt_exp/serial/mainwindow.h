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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init_ui(void);
    void StringToHex(QString str,QByteArray &send_data);

    /* 串口管理 */
    QSharedPointer<QSerialPort> Serial;
    QString m_Com;              // 已打开的串口号
    QString m_Baud;             // 已打开的串口波特率
    bool m_Com_Open=false;      // 串口打开标志位

    bool init_serial();
    void serial_ui_update();
    bool serial_close();
    bool serial_open();

    /* 调试管理 */
    bool show_debug;

private slots:
    /* 时间管理 */
    void get_time();

    /* 串口管理 */
    void get_com();
    void serial_write();
    int serial_read();
    void on_serial_open_btn_clicked();

    /* 调试管理 */
    void on_show_debug_btn_clicked();

    /* 页面切换 */
    void on_IDcard_page_btn_clicked();
    void on_ICcard_page_btn_clicked();

    /* ID卡 应用界面 */
    void on_ID_read_btn_clicked();
    void on_ID_write_btn_clicked();
    void on_ID_data_clear_btn_clicked();
};


#endif // MAINWINDOW_H

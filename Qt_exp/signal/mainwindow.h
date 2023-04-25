#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
Q_SIGNALS:
    void showchild();
    void closechild();
    void sendText(const QString str);

private Q_SLOTS:
    void on_btn_open_1_clicked();
    void on_btn_send_1_clicked();
    void on_btn_close_1_clicked();
    void on_btn_open_2_clicked();
    void on_btn_close_2_clicked();
    void on_btn_send_2_clicked();
    //  自定义槽函数
    void set_mian_label(const QString str);     // 子窗口 设置文本的槽函数
    void form_close_handle();  // 子窗口 关闭信号的处理

private:
    Ui::MainWindow *ui;
    QSharedPointer<Form> from_1=nullptr;
    QSharedPointer<Form> from_2=nullptr;
};

#endif // MAINWINDOW_H

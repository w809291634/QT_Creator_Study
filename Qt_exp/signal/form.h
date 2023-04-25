#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(const QString name,
                  const QString send_main_str,
                  const QString send_other_str,
                  QWidget *parent = nullptr);
    ~Form();


signals:
    void close();              // 通知主窗口 子窗口关闭
    void sendMainText(const QString str);
    void sendOtherText(const QString str);

private slots:
    // 自定义槽函数
    void showMyself();
    void closeMyself();
    // 按钮槽函数
    void on_btn_hide_clicked();
    void on_btn_close_clicked();
    void on_btn_send_to_main_clicked();
    void on_btn_send_to_other_clicked();

public slots:
    void setlabelText(const QString str);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Form *ui;
    QString sendstr_to_main;    // 发送主窗口的字符串
    QString sendstr_to_other;   // 发送同类窗口的字符串
};

#endif // FORM_H

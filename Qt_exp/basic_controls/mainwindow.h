#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void init_control_attribute();
    void init_control_communication();
private slots:
    /* mainwindow设置当前主页 */
    void on_setCurrentIndex();
    /* 按钮页面槽函数 */
    void pushbutton_clicked();
    void toolbutton_clicked();
    void radiobutton_clicked();
    void checkbox_stateChange(const int state);
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_spinBox_R_valueChanged(int arg1);
    void on_spinBox_G_valueChanged(int arg1);
    void on_doubleSpinBox_B_valueChanged(double arg1);
    /* 文本页面槽函数 */
    void lineedit_textEdited(const QString str);
    void lineedit_returnPressed();
    /* 时间日期页面槽函数 */
    void on_calendarWidget_clicked(const QDate &date);
    void on_dateEdit_userDateChanged(const QDate &date);
    void on_timeEdit_userTimeChanged(const QTime &time);
    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    /* 文本页面槽函数 */
    void handletimeout(void);
};

#endif // MAINWINDOW_H

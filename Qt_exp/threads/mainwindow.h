#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QtCore>
#include "mythread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    /* 信号量槽函数 */
    void on_QSemaphore_btn_clicked(bool checked);
    void update_QSemaphore_bar(const QString& msg);

    /* 互斥量槽函数 */
    void on_QMutex_cbox_clicked(bool checked);
    void on_QMutex_run_btn_clicked();
    void update_QMutex_data(const QString& act,
                            const QString& cnt1,
                            const QString& cnt2);
    /* QWaitCondition 槽函数 */
    void QWaitCondition_btn_clicked();
    void handleTimeout();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
    QTimer* m_pTimer;
};

void app_main(MainWindow* instance);
extern MainWindow* _main_win;

#endif // MAINWINDOW_H

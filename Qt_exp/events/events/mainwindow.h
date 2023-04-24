#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QTimer>
#include <QDebug>
#include "myevent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // 事件相关
    bool eventFilter(QObject *object, QEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void handletimeout();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    myevent* _myevent;
//    myevent event1{};
//    myevent event2{};
//    myevent event3{};
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QTimer>
#include <QDebug>
#include <QPushButton>
namespace Ui {
class MainWindow;
}

extern QEvent::Type _postEvent;
extern QEvent::Type _sendEvent;

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

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handletimeout();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    // 自定义事件
    QSharedPointer<QEvent> sendEvent=nullptr;
};

#endif // MAINWINDOW_H

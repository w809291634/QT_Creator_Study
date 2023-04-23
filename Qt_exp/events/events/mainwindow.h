#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Event handlers
  bool event(QEvent *event) override;
//     void mousePressEvent(QMouseEvent *event);
//     void mouseReleaseEvent(QMouseEvent *event);
//     void mouseDoubleClickEvent(QMouseEvent *event);
//     void mouseMoveEvent(QMouseEvent *event);

//     void keyPressEvent(QKeyEvent *event);
//     void keyReleaseEvent(QKeyEvent *event);
//     void focusInEvent(QFocusEvent *event);
//     void focusOutEvent(QFocusEvent *event);
//     void enterEvent(QEvent *event);
//     void leaveEvent(QEvent *event);
//     void paintEvent(QPaintEvent *event);
//     void moveEvent(QMoveEvent *event);
//     void resizeEvent(QResizeEvent *event);
//     void closeEvent(QCloseEvent *event);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

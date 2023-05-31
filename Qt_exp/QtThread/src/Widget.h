#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QThread>

class ThreadFromQThread;
class ThreadObject;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void startObjThreadWork1();
    void startObjThreadWork2();
private slots:

    void onButtonObjectMove2ThreadClicked();
    void onButtonObjectMove2Thread2Clicked();
    void onButtonObjectThreadStopClicked();
    void startObjThread();

    void progress(int val);
    void receiveMessage(const QString& str);
    void heartTimeOut();

private:
    Ui::Widget *ui;

    ThreadObject* m_obj;
    QThread* m_objThread;
    ThreadFromQThread* m_currentRunLoaclThread;
};

#endif // WIDGET_H

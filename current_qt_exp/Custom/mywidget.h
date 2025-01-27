#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

class MyWidget : public QWidget
{
public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
private:
    void InitWidget();
protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
//    QtWidgetTitleBar    *m_widgetTitle;
};

#endif // MYWIDGET_H

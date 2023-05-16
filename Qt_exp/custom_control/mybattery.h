#ifndef MYBATTERY_H
#define MYBATTERY_H

#include <QWidget>
#include <QtGui>

class MyBattery : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal m_value READ getValue WRITE setValue)
public:
    MyBattery(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(200,120);
    }


private:
    void drawFrame(QPainter* painter);
    void drawBattery(QPainter* painter);
    qreal getValue(void) const
    {
        return m_value;
    }

private:
    QTimer* updateTimer;
    bool    m_bReverse;
    qreal   m_headWidth;
    qreal   m_value;
    qreal   m_currentValue;
    QRectF  m_batteryRect;

signals:
    void valueChanged(qreal value);

private slots:
    void UpdateGraph();

public slots:
    void setValue(qreal value);
};

#endif

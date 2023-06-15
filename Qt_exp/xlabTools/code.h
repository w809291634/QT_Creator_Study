#ifndef CODE_H
#define CODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QPainter>
#include "mainwindow.h"

namespace Ui {
class Code;
}

class Code : public QWidget
{
    Q_OBJECT

public:
    explicit Code(QWidget *parent = nullptr);
    ~Code();
    QMainWindow *mainwindow;
    void GenerateQRcode(QString tempstr);
private:
    Ui::Code *ui;
private slots:
    void Display_Code();
};

#endif // CODE_H

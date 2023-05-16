#include <QtWidgets>
#include "frmmain.h"

#include <QTextCodec>
//#include <QDesktopWidget>
//#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frmMain w;
    //窗体居中显示
    QDesktopWidget *desktop=QApplication::desktop();
    int width=desktop->width();
    int height=desktop->height();

    w.move((width-w.width())/2,(height-w.height())/2);
    w.setFixedSize(w.width(),w.height());                   //设置窗口大小
    w.show();
//    //应用样式
//    QApplication::setStyle(QStyleFactory::create("GTK"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}

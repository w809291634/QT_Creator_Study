#include "QtHttpForC.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtHttpForC w;
    w.show();
    return a.exec();
}

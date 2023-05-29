#include "QtHttpForS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtHttpForS w;
    w.show();
    return a.exec();
}

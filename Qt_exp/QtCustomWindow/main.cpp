#include "CustomMainWindow.h"
#include <QApplication>
#include "BaseWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseWindow w;
    w.show();

    return a.exec();
}

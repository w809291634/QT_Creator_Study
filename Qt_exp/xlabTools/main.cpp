#include "mainwindow.h"
#include <QApplication>
#include "code.h"
#include "updata/updata.h"
#include "lorawanchm.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Code c;
    upData u;
    LoRaWANChm l;
    QObject::connect(&w,SIGNAL(Display_Code()),&c,SLOT(Display_Code()));
    QObject::connect(&w,SIGNAL(check_upFirst()),&u,SLOT(checkUp()));
    QObject::connect(&w,SIGNAL(check_upData()),&u,SLOT(on_btn_checkUp_clicked()));
    QObject::connect(&w,SIGNAL(setLoRaWANChm()),&l,SLOT(showLoRaWANChm()));
    QObject::connect(&l,SIGNAL(chmTableClose()),&w,SLOT(fillChmDataStr()));
    w.show();
    return a.exec();
}

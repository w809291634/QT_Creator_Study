#include "frmmain.h"
#include "ui_frmmain.h"


frmMain::frmMain(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::frmMain)
{
    ui->setupUi(this);

    //解决中文乱码
    codec = QTextCodec::codecForName("GBK");

    this->InitTab1();
    this->ChangeVisibleTab1(false);

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(TabChange(int)));
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::TabChange(int index)
{
    if (index==0)
    {
        this->ChangeVisibleTab1(false);
    }
    else
    {
        this->ChangeVisibleTab2(false);
    }
}

void frmMain::InitTab1()
{
    mainLayout1=new QVBoxLayout();
    mainLayout1->setSpacing(0);
    mainLayout1->setContentsMargins(10,10,10,10);

    mybattery1=new myBattery(ui->fm1);
    mybattery2=new myBattery(ui->fm1);
    mybattery3=new myBattery(ui->fm1);
    mybattery4=new myBattery(ui->fm1);

    mainLayout1->addWidget(mybattery1);
    mainLayout1->addWidget(mybattery2);
    mainLayout1->addWidget(mybattery3);
    mainLayout1->addWidget(mybattery4);

    ui->fm1->setLayout(mainLayout1);

    updateTimer8=new QTimer(this);
    updateTimer8->setInterval(3000);
    connect(updateTimer8,SIGNAL(timeout()),this,SLOT(UpdateData8()));
}

void frmMain::InitTab2()
{

}

void frmMain::ChangeVisibleTab1(bool b)
{
    mybattery1->setVisible(b);
    mybattery2->setVisible(b);
    mybattery3->setVisible(b);
    mybattery4->setVisible(b);
}

void frmMain::ChangeVisibleTab2(bool b)
{

}

void frmMain::on_btnGauge1_clicked()
{

}

void frmMain::UpdateData1()
{

}

void frmMain::on_btnGauge2_clicked()
{

}

void frmMain::UpdateData2()
{

}

void frmMain::on_btnGauge3_clicked()
{

}

void frmMain::UpdateData3(qreal value)
{

}

void frmMain::on_btnGauge4_clicked()
{

}

void frmMain::UpdateData4()
{

}

void frmMain::on_btnGauge5_clicked()
{

}

void frmMain::UpdateData5()
{

}

void frmMain::on_btnGauge6_clicked()
{

}

void frmMain::UpdateData6()
{

}

void frmMain::on_btnGauge7_clicked()
{

}

void frmMain::UpdateData7()
{

}

void frmMain::on_btnProgressBar_clicked()
{

}


void frmMain::on_btnBattery_clicked()
{
    this->ChangeVisibleTab1(false);
    mybattery1->setVisible(true);
    mybattery2->setVisible(true);
    mybattery3->setVisible(true);
    mybattery4->setVisible(true);
    updateTimer8->start();
}

void frmMain::UpdateData8()
{
    int value1=qrand()%100;
    int value2=qrand()%100;
    int value3=qrand()%100;
    int value4=qrand()%100;
    mybattery1->setValue(value1);
    mybattery2->setValue(value2);
    mybattery3->setValue(value3);
    mybattery4->setValue(value4);
    ui->labValue1->setText(codec->toUnicode("当前值:%1 %2 %3 %4").arg(value1).arg(value2).arg(value3).arg(value4));
}

void frmMain::on_btnCoolBar_clicked()
{

}

void frmMain::UpdateData9(int value)
{

}

void frmMain::on_btnIndicator_clicked()
{

}

void frmMain::UpdateData10()
{
}

void frmMain::on_btnVolume_clicked()
{

}

void frmMain::UpdateData11()
{

}

void frmMain::on_btnChart1_clicked()
{

}

void frmMain::UpdateData21()
{

}

void frmMain::on_btnFlashList_clicked()
{

}

void frmMain::UpdateData22(QString txt)
{

}

void frmMain::on_btnButtonBoard_clicked()
{

}

void frmMain::UpdateData23(QString txt)
{

}

void frmMain::on_btnSliderButton_clicked()
{

}

void frmMain::UpdateData24(QString txt)
{

}

void frmMain::on_btnPhotoView_clicked()
{

}

void frmMain::on_btnAroundCircle_clicked()
{

}

void frmMain::on_btnProgressBar1_clicked()
{

}

void frmMain::UpdateData27()
{

}

void frmMain::on_btnNewLed_clicked()
{

}

void frmMain::UpdateData28()
{

}

void frmMain::on_btnWave_clicked()
{

}

void frmMain::UpdateData29()
{

}

void frmMain::on_btnWave1_clicked()
{

}

void frmMain::UpdateData30()
{

}

void frmMain::on_btnWave2_clicked()
{

}

void frmMain::UpdateData31()
{

}

#include "code.h"
#include "ui_code.h"
#include "qrcode/qrencode.h"

extern QString Code_Str;

Code::Code(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Code)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
}

Code::~Code()
{
    delete ui;
}

/*
生成二维码图片
*/
void Code::GenerateQRcode(QString tempstr)
{
    QRcode *qrcode; //二维码数据
    //QR_ECLEVEL_Q 容错等级
    qrcode = QRcode_encodeString(tempstr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qint32 temp_width=250; //二维码图片的大小
    qint32 temp_height=250;
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y =(double) temp_height /(double) qrcode_width;
    QImage mainimg=QImage(temp_width,temp_height,QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, temp_width, temp_height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( qint32 y = 0; y < qrcode_width; y ++)
    {
        for(qint32 x = 0; x < qrcode_width; x++)
        {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    QPixmap mainmap=QPixmap::fromImage(mainimg);
    ui->label_Code->setPixmap(mainmap);
}

//弹出二维码
void Code::Display_Code()
{
    this->setWindowModality(Qt::ApplicationModal);//其他窗口不可用
    GenerateQRcode(MainWindow::Code_Str);
    this->show();
}



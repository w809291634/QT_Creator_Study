#include "lorawanchm.h"
#include "ui_lorawanchm.h"

extern QString ChmData_Str;

LoRaWANChm::LoRaWANChm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoRaWANChm)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    dataRowTitle << tr("掩 码") << tr("0") << tr("1") << tr("2") << tr("3")
                 << tr("4") << tr("5") << tr("6") << tr("7");
    dataColumnTitle << tr("00-07") << tr("08-15") << tr("16-23") << tr("24-31")
                    << tr("32-39") << tr("40-47") << tr("48-55") << tr("56-63")
                    << tr("64-71") << tr("72-79") << tr("80-87") << tr("88-95");
    ui->tableWidget->setColumnCount(dataRowTitle.count());
    ui->tableWidget->setRowCount(12);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(dataRowTitle);
    ui->tableWidget->verticalHeader()->setVisible(false);
    for(int i=0; i<12; i++)
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(dataColumnTitle.at(i)));
}

LoRaWANChm::~LoRaWANChm()
{
    delete ui;
}

void LoRaWANChm::showLoRaWANChm()
{
    fillChmTable(MainWindow::ChmData_Str);
    interface_init();
    this->show();
    this->setWindowModality(Qt::ApplicationModal);//其他窗口不可用
}

/**
* @funcname      hexToBin()
* @brief         十六进制转换为二进制
* @param         hexStr-十六进制字符串
* @return        无
* @author        Zhoucj
* @date          2019-01-02
***************************************************************/
QString LoRaWANChm::hexToBin(QString hexStr)
{
    bool ok;
    QString binStr;
    int dec = hexStr.toInt(&ok, 16);
    binStr = hexStr.setNum(dec, 2);
    int binStrLen = binStr.length();
    if(binStrLen < 8)
    {
        for(unsigned char i=0; i<8-binStrLen; i++)
            binStr = '0' + binStr;
    }
    return binStr;
}

/**
* @funcname      binToHex()
* @brief         二进制转换为十六进制
* @param         binStr-二进制字符串
* @return        无
* @author        Zhoucj
* @date          2019-01-02
***************************************************************/
QString LoRaWANChm::binToHex(QString binStr)
{
    bool ok;
    QString hexStr;
    int dec = binStr.toInt(&ok, 2);
    hexStr = binStr.setNum(dec, 16);
    int hexStrLen = hexStr.length();
    if(hexStrLen < 2)
        hexStr = '0' + hexStr;
    return hexStr.toUpper();
}

/**
* @funcname      chmHexDataHandle
* @brief         十六进制数据转换为二进制
* @param         hexDataStr-十六进制字符串
* @return        binDataStr-二进制字符串
* @author        Zhoucj
* @date          2019-01-12
***************************************************************/
QString LoRaWANChm::chmHexDataHandle(QString hexDataStr)
{
    QString hexStr = "";
    QString binDataStr = "";
    for(int i=0; i<hexDataStr.length(); i+=2)
    {
        hexStr = hexDataStr.mid(i, 2);
        binDataStr += hexToBin(hexStr);
    }
    return binDataStr;
}

/**
* @funcname      fillChmTable
* @brief         补充表格数据
* @param         chmDataStr-需要补充的数据
* @return        无
* @author        Zhoucj
* @date          2019-01-12
***************************************************************/
void LoRaWANChm::fillChmTable(QString chmDataStr)
{
    QString binStr = chmHexDataHandle(chmDataStr);
    int a = 0;
    for(int i=0; i<12; i++)
    {
        for(int j=8; j>0; j--)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(binStr.mid(a++, 1)));
            ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

/**
* @funcname      chmBinDataHandle
* @brief         二进制数据转换为十六进制
* @param         binDataStr-二进制字符串
* @return        hexDataStr-十六进制字符串
* @author        Zhoucj
* @date          2019-01-12
***************************************************************/
QString LoRaWANChm::chmBinDataHandle(QString binDataStr)
{
    QString binStr = "";
    QString hexDataStr = "";
    for(int i=0; i<binDataStr.length(); i+=8)
    {
        binStr = binDataStr.mid(i, 8);
        hexDataStr += binToHex(binStr);
    }
    return hexDataStr;
}

/**
* @funcname      getChmTable
* @brief         获取表格二进制字符串，并转换为十六进制
* @param         无
* @return        十六进制数据
* @author        Zhoucj
* @date          2019-01-12
***************************************************************/
QString LoRaWANChm::getChmTable(void)
{
    QString binStr = "";
    for(int i=0; i<12; i++)
        for(int j=8; j>0; j--)
            binStr += ui->tableWidget->item(i, j)->text();
    return chmBinDataHandle(binStr);
}

void LoRaWANChm::interface_init(void)
{
    ui->lbl_hint->setText(getChmTable());
}

void LoRaWANChm::on_buttonBox_accepted()
{
    MainWindow::ChmData_Str = ui->lbl_hint->text();
    emit chmTableClose();
    this->hide();
}

void LoRaWANChm::on_buttonBox_rejected()
{
    this->hide();
}

void LoRaWANChm::on_tableWidget_cellClicked(int row)
{
    static int lastRow = 0;
    if(lastRow != row)
    {
        dataRowTitle.clear();
        int num = row * 8;
        dataRowTitle << tr("掩 码") << QString::number(num, 10) << QString::number(num+1, 10)
                         << QString::number(num+2, 10) << QString::number(num+3, 10)
                             << QString::number(num+4, 10) << QString::number(num+5, 10)
                                 << QString::number(num+6, 10) << QString::number(num+7, 10);
        ui->tableWidget->setHorizontalHeaderLabels(dataRowTitle);
        lastRow = row;
    }
    else
    {
        return;
    }
}

void LoRaWANChm::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QString dataStr = ui->tableWidget->item(index.row(), index.column())->text();
    QTableWidgetItem *clickData = new QTableWidgetItem;
    if(dataStr == "1")
        clickData->setText("0");
    else
        clickData->setText("1");
    ui->tableWidget->setItem(index.row(), index.column(), clickData);
    ui->tableWidget->item(index.row(), index.column())->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->lbl_hint->setText(getChmTable());
}

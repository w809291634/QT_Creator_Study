#ifndef LORAWANCHM_H
#define LORAWANCHM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include "mainwindow.h"

namespace Ui {
class LoRaWANChm;
}

class LoRaWANChm : public QWidget
{
    Q_OBJECT

public:
    explicit LoRaWANChm(QWidget *parent = nullptr);
    ~LoRaWANChm();
    QString binToHex(QString binStr);
    QString hexToBin(QString hexStr);
    void fillChmTable(QString chmDataStr);
    QString chmHexDataHandle(QString hexDataStr);
    QString chmBinDataHandle(QString binDataStr);
    QString getChmTable();
    void interface_init();
public slots:
    void showLoRaWANChm();
signals:
    void chmTableClose();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_tableWidget_cellClicked(int row);
    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::LoRaWANChm *ui;
    QStringList dataRowTitle;
    QStringList dataColumnTitle;
};

#endif // LORAWANCHM_H

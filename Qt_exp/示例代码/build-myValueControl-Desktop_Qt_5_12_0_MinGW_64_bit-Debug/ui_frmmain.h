/********************************************************************************
** Form generated from reading UI file 'frmmain.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMMAIN_H
#define UI_FRMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmMain
{
public:
    QTabWidget *tabWidget;
    QWidget *tab1;
    QFrame *fm1;
    QLabel *labValue1;
    QPushButton *btnGauge4;
    QPushButton *btnGauge6;
    QPushButton *btnProgressBar;
    QPushButton *btnVolume;
    QPushButton *btnGauge5;
    QPushButton *btnGauge2;
    QPushButton *btnGauge3;
    QPushButton *btnCoolBar;
    QPushButton *btnGauge1;
    QPushButton *btnIndicator;
    QPushButton *btnGauge7;
    QPushButton *btnBattery;
    QWidget *tab2;
    QFrame *fm2;
    QPushButton *btnChart1;
    QLabel *labValue2;
    QPushButton *btnFlashList;
    QPushButton *btnButtonBoard;
    QPushButton *btnSliderButton;
    QPushButton *btnPhotoView;
    QPushButton *btnAroundCircle;
    QPushButton *btnProgressBar1;
    QPushButton *btnNewLed;
    QPushButton *btnWave;
    QPushButton *btnWave1;
    QPushButton *btnWave2;

    void setupUi(QWidget *frmMain)
    {
        if (frmMain->objectName().isEmpty())
            frmMain->setObjectName(QString::fromUtf8("frmMain"));
        frmMain->resize(642, 548);
        tabWidget = new QTabWidget(frmMain);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 625, 533));
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setTabsClosable(false);
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        fm1 = new QFrame(tab1);
        fm1->setObjectName(QString::fromUtf8("fm1"));
        fm1->setGeometry(QRect(0, 0, 481, 509));
        fm1->setFrameShape(QFrame::StyledPanel);
        fm1->setFrameShadow(QFrame::Raised);
        labValue1 = new QLabel(tab1);
        labValue1->setObjectName(QString::fromUtf8("labValue1"));
        labValue1->setGeometry(QRect(490, 480, 131, 28));
        labValue1->setStyleSheet(QString::fromUtf8(""));
        btnGauge4 = new QPushButton(tab1);
        btnGauge4->setObjectName(QString::fromUtf8("btnGauge4"));
        btnGauge4->setGeometry(QRect(490, 120, 125, 29));
        btnGauge4->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge6 = new QPushButton(tab1);
        btnGauge6->setObjectName(QString::fromUtf8("btnGauge6"));
        btnGauge6->setGeometry(QRect(490, 200, 125, 29));
        btnGauge6->setCursor(QCursor(Qt::PointingHandCursor));
        btnProgressBar = new QPushButton(tab1);
        btnProgressBar->setObjectName(QString::fromUtf8("btnProgressBar"));
        btnProgressBar->setGeometry(QRect(490, 280, 125, 29));
        btnProgressBar->setCursor(QCursor(Qt::PointingHandCursor));
        btnVolume = new QPushButton(tab1);
        btnVolume->setObjectName(QString::fromUtf8("btnVolume"));
        btnVolume->setGeometry(QRect(490, 440, 125, 29));
        btnVolume->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge5 = new QPushButton(tab1);
        btnGauge5->setObjectName(QString::fromUtf8("btnGauge5"));
        btnGauge5->setGeometry(QRect(490, 160, 125, 29));
        btnGauge5->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge2 = new QPushButton(tab1);
        btnGauge2->setObjectName(QString::fromUtf8("btnGauge2"));
        btnGauge2->setGeometry(QRect(490, 40, 125, 29));
        btnGauge2->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge3 = new QPushButton(tab1);
        btnGauge3->setObjectName(QString::fromUtf8("btnGauge3"));
        btnGauge3->setGeometry(QRect(490, 80, 125, 29));
        btnGauge3->setCursor(QCursor(Qt::PointingHandCursor));
        btnCoolBar = new QPushButton(tab1);
        btnCoolBar->setObjectName(QString::fromUtf8("btnCoolBar"));
        btnCoolBar->setGeometry(QRect(490, 360, 125, 29));
        btnCoolBar->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge1 = new QPushButton(tab1);
        btnGauge1->setObjectName(QString::fromUtf8("btnGauge1"));
        btnGauge1->setGeometry(QRect(490, 3, 125, 29));
        btnGauge1->setCursor(QCursor(Qt::PointingHandCursor));
        btnIndicator = new QPushButton(tab1);
        btnIndicator->setObjectName(QString::fromUtf8("btnIndicator"));
        btnIndicator->setGeometry(QRect(490, 400, 125, 29));
        btnIndicator->setCursor(QCursor(Qt::PointingHandCursor));
        btnGauge7 = new QPushButton(tab1);
        btnGauge7->setObjectName(QString::fromUtf8("btnGauge7"));
        btnGauge7->setGeometry(QRect(490, 240, 125, 29));
        btnGauge7->setCursor(QCursor(Qt::PointingHandCursor));
        btnBattery = new QPushButton(tab1);
        btnBattery->setObjectName(QString::fromUtf8("btnBattery"));
        btnBattery->setGeometry(QRect(490, 320, 125, 29));
        btnBattery->setCursor(QCursor(Qt::PointingHandCursor));
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        fm2 = new QFrame(tab2);
        fm2->setObjectName(QString::fromUtf8("fm2"));
        fm2->setGeometry(QRect(0, 0, 481, 509));
        fm2->setFrameShape(QFrame::StyledPanel);
        fm2->setFrameShadow(QFrame::Raised);
        btnChart1 = new QPushButton(tab2);
        btnChart1->setObjectName(QString::fromUtf8("btnChart1"));
        btnChart1->setGeometry(QRect(490, 3, 125, 29));
        btnChart1->setCursor(QCursor(Qt::PointingHandCursor));
        labValue2 = new QLabel(tab2);
        labValue2->setObjectName(QString::fromUtf8("labValue2"));
        labValue2->setGeometry(QRect(490, 480, 131, 28));
        labValue2->setStyleSheet(QString::fromUtf8(""));
        btnFlashList = new QPushButton(tab2);
        btnFlashList->setObjectName(QString::fromUtf8("btnFlashList"));
        btnFlashList->setGeometry(QRect(490, 40, 125, 29));
        btnFlashList->setCursor(QCursor(Qt::PointingHandCursor));
        btnButtonBoard = new QPushButton(tab2);
        btnButtonBoard->setObjectName(QString::fromUtf8("btnButtonBoard"));
        btnButtonBoard->setGeometry(QRect(490, 80, 125, 29));
        btnButtonBoard->setCursor(QCursor(Qt::PointingHandCursor));
        btnSliderButton = new QPushButton(tab2);
        btnSliderButton->setObjectName(QString::fromUtf8("btnSliderButton"));
        btnSliderButton->setGeometry(QRect(490, 120, 125, 29));
        btnSliderButton->setCursor(QCursor(Qt::PointingHandCursor));
        btnPhotoView = new QPushButton(tab2);
        btnPhotoView->setObjectName(QString::fromUtf8("btnPhotoView"));
        btnPhotoView->setGeometry(QRect(490, 160, 125, 29));
        btnPhotoView->setCursor(QCursor(Qt::PointingHandCursor));
        btnAroundCircle = new QPushButton(tab2);
        btnAroundCircle->setObjectName(QString::fromUtf8("btnAroundCircle"));
        btnAroundCircle->setGeometry(QRect(490, 200, 125, 29));
        btnAroundCircle->setCursor(QCursor(Qt::PointingHandCursor));
        btnProgressBar1 = new QPushButton(tab2);
        btnProgressBar1->setObjectName(QString::fromUtf8("btnProgressBar1"));
        btnProgressBar1->setGeometry(QRect(490, 240, 125, 29));
        btnProgressBar1->setCursor(QCursor(Qt::PointingHandCursor));
        btnNewLed = new QPushButton(tab2);
        btnNewLed->setObjectName(QString::fromUtf8("btnNewLed"));
        btnNewLed->setGeometry(QRect(490, 280, 125, 29));
        btnNewLed->setCursor(QCursor(Qt::PointingHandCursor));
        btnWave = new QPushButton(tab2);
        btnWave->setObjectName(QString::fromUtf8("btnWave"));
        btnWave->setGeometry(QRect(490, 320, 125, 29));
        btnWave->setCursor(QCursor(Qt::PointingHandCursor));
        btnWave1 = new QPushButton(tab2);
        btnWave1->setObjectName(QString::fromUtf8("btnWave1"));
        btnWave1->setGeometry(QRect(490, 360, 125, 29));
        btnWave1->setCursor(QCursor(Qt::PointingHandCursor));
        btnWave2 = new QPushButton(tab2);
        btnWave2->setObjectName(QString::fromUtf8("btnWave2"));
        btnWave2->setGeometry(QRect(490, 400, 125, 29));
        btnWave2->setCursor(QCursor(Qt::PointingHandCursor));
        tabWidget->addTab(tab2, QString());

        retranslateUi(frmMain);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(frmMain);
    } // setupUi

    void retranslateUi(QWidget *frmMain)
    {
        frmMain->setWindowTitle(QApplication::translate("frmMain", "\346\216\247\344\273\266\346\274\224\347\244\272", nullptr));
        labValue1->setText(QApplication::translate("frmMain", "\345\275\223\345\211\215\345\200\274:", nullptr));
        btnGauge4->setText(QApplication::translate("frmMain", "\347\272\277\346\200\247\345\210\273\345\272\246\346\214\207\347\244\272\344\273\252", nullptr));
        btnGauge6->setText(QApplication::translate("frmMain", "\345\236\202\347\233\264\345\217\263\345\210\273\345\272\246\346\214\207\347\244\272\344\273\252", nullptr));
        btnProgressBar->setText(QApplication::translate("frmMain", "\351\205\267\347\202\253\350\277\233\345\272\246\346\235\241", nullptr));
        btnVolume->setText(QApplication::translate("frmMain", "\351\242\221\350\260\261\345\210\206\346\236\220\346\216\247\344\273\266", nullptr));
        btnGauge5->setText(QApplication::translate("frmMain", "\345\234\206\345\275\242\345\210\273\345\272\246\346\214\207\347\244\272\344\273\252", nullptr));
        btnGauge2->setText(QApplication::translate("frmMain", "\345\234\206\345\275\242\346\214\207\351\222\210\346\214\207\347\244\272\344\273\252", nullptr));
        btnGauge3->setText(QApplication::translate("frmMain", "\345\267\246\345\217\263\346\273\221\345\235\227\346\214\207\347\244\272\344\273\252", nullptr));
        btnCoolBar->setText(QApplication::translate("frmMain", "\346\244\255\345\234\206\346\273\221\345\235\227", nullptr));
        btnGauge1->setText(QApplication::translate("frmMain", "\345\234\206\345\275\242\346\213\261\345\275\242\346\214\207\347\244\272\344\273\252", nullptr));
        btnIndicator->setText(QApplication::translate("frmMain", "\345\236\202\347\233\264\346\226\271\345\235\227\350\277\233\345\272\246\346\235\241", nullptr));
        btnGauge7->setText(QApplication::translate("frmMain", "\345\236\202\347\233\264\345\267\246\345\210\273\345\272\246\346\214\207\347\244\272\344\273\252", nullptr));
        btnBattery->setText(QApplication::translate("frmMain", "\347\224\265\346\272\220\346\214\207\347\244\272\344\273\252", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("frmMain", "\345\220\204\347\247\215\345\220\204\346\240\267\347\232\204\346\216\247\344\273\266", nullptr));
        btnChart1->setText(QApplication::translate("frmMain", "\345\233\276\350\241\250\346\233\262\347\272\277\346\230\276\347\244\272", nullptr));
        labValue2->setText(QApplication::translate("frmMain", "\345\275\223\345\211\215\345\200\274:", nullptr));
        btnFlashList->setText(QApplication::translate("frmMain", "\346\273\221\345\212\250\345\210\227\350\241\250\346\241\206", nullptr));
        btnButtonBoard->setText(QApplication::translate("frmMain", "\346\214\211\351\222\256\351\235\242\346\235\277", nullptr));
        btnSliderButton->setText(QApplication::translate("frmMain", "\346\273\221\345\212\250\350\203\214\346\231\257\351\235\242\346\235\277", nullptr));
        btnPhotoView->setText(QApplication::translate("frmMain", "\345\233\276\347\211\207\346\265\217\350\247\210\345\267\245\345\205\267", nullptr));
        btnAroundCircle->setText(QApplication::translate("frmMain", "\346\227\213\350\275\254\347\232\204\345\234\206", nullptr));
        btnProgressBar1->setText(QApplication::translate("frmMain", "\345\217\246\347\261\273\350\277\233\345\272\246\346\235\241", nullptr));
        btnNewLed->setText(QApplication::translate("frmMain", "LED\346\225\260\345\255\227\346\230\276\347\244\272", nullptr));
        btnWave->setText(QApplication::translate("frmMain", "\345\243\260\351\237\263\346\263\242\345\275\242\345\212\250\346\200\201\345\233\276", nullptr));
        btnWave1->setText(QApplication::translate("frmMain", "\345\243\260\351\237\263\347\233\264\346\226\271\345\212\250\346\200\201\345\233\276", nullptr));
        btnWave2->setText(QApplication::translate("frmMain", "\345\257\271\347\247\260\347\233\264\346\226\271\345\212\250\346\200\201\345\233\276", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("frmMain", "\345\220\204\345\274\217\345\220\204\346\240\267\347\232\204\346\216\247\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class frmMain: public Ui_frmMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMMAIN_H

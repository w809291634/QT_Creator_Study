/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *btn_open_1;
    QLabel *label;
    QPushButton *btn_close_1;
    QPushButton *btn_send_1;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *btn_open_2;
    QPushButton *btn_close_2;
    QPushButton *btn_send_2;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btn_open_1 = new QPushButton(groupBox);
        btn_open_1->setObjectName(QString::fromUtf8("btn_open_1"));

        gridLayout->addWidget(btn_open_1, 0, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        btn_close_1 = new QPushButton(groupBox);
        btn_close_1->setObjectName(QString::fromUtf8("btn_close_1"));

        gridLayout->addWidget(btn_close_1, 1, 0, 1, 1);

        btn_send_1 = new QPushButton(groupBox);
        btn_send_1->setObjectName(QString::fromUtf8("btn_send_1"));

        gridLayout->addWidget(btn_send_1, 3, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        btn_open_2 = new QPushButton(groupBox_2);
        btn_open_2->setObjectName(QString::fromUtf8("btn_open_2"));

        gridLayout_2->addWidget(btn_open_2, 0, 0, 1, 1);

        btn_close_2 = new QPushButton(groupBox_2);
        btn_close_2->setObjectName(QString::fromUtf8("btn_close_2"));

        gridLayout_2->addWidget(btn_close_2, 1, 0, 1, 1);

        btn_send_2 = new QPushButton(groupBox_2);
        btn_send_2->setObjectName(QString::fromUtf8("btn_send_2"));

        gridLayout_2->addWidget(btn_send_2, 2, 0, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 3, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\2431", nullptr));
        btn_open_1->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\255\220\347\252\227\345\217\243", nullptr));
        label->setText(QString());
        btn_close_1->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\255\220\347\252\227\345\217\243", nullptr));
        btn_send_1->setText(QApplication::translate("MainWindow", "\345\220\221\347\252\227\345\217\2431\345\217\221\351\200\201\344\277\241\345\217\267", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\2432", nullptr));
        btn_open_2->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\255\220\347\252\227\345\217\243", nullptr));
        btn_close_2->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\255\220\347\252\227\345\217\243", nullptr));
        btn_send_2->setText(QApplication::translate("MainWindow", "\345\220\221\347\252\227\345\217\2432\345\217\221\351\200\201\344\277\241\345\217\267", nullptr));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

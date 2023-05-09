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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_btn;
    QAction *action_text;
    QAction *action_bar;
    QAction *action_datetime;
    QAction *action_etc;
    QAction *action_exit;
    QAction *action_about;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_1_btn;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_1;
    QPushButton *btn_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_3;
    QPushButton *btn_4;
    QWidget *page_2_text;
    QWidget *page_3_bar;
    QWidget *page_4_etime;
    QWidget *page_5_etc;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(701, 410);
        action_btn = new QAction(MainWindow);
        action_btn->setObjectName(QString::fromUtf8("action_btn"));
        action_text = new QAction(MainWindow);
        action_text->setObjectName(QString::fromUtf8("action_text"));
        action_bar = new QAction(MainWindow);
        action_bar->setObjectName(QString::fromUtf8("action_bar"));
        action_datetime = new QAction(MainWindow);
        action_datetime->setObjectName(QString::fromUtf8("action_datetime"));
        action_etc = new QAction(MainWindow);
        action_etc->setObjectName(QString::fromUtf8("action_etc"));
        action_exit = new QAction(MainWindow);
        action_exit->setObjectName(QString::fromUtf8("action_exit"));
        action_about = new QAction(MainWindow);
        action_about->setObjectName(QString::fromUtf8("action_about"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_1_btn = new QWidget();
        page_1_btn->setObjectName(QString::fromUtf8("page_1_btn"));
        groupBox = new QGroupBox(page_1_btn);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 20, 219, 106));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_1 = new QPushButton(groupBox);
        btn_1->setObjectName(QString::fromUtf8("btn_1"));

        horizontalLayout->addWidget(btn_1);

        btn_2 = new QPushButton(groupBox);
        btn_2->setObjectName(QString::fromUtf8("btn_2"));

        horizontalLayout->addWidget(btn_2);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btn_3 = new QPushButton(groupBox);
        btn_3->setObjectName(QString::fromUtf8("btn_3"));

        horizontalLayout_2->addWidget(btn_3);

        btn_4 = new QPushButton(groupBox);
        btn_4->setObjectName(QString::fromUtf8("btn_4"));

        horizontalLayout_2->addWidget(btn_4);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        stackedWidget->addWidget(page_1_btn);
        page_2_text = new QWidget();
        page_2_text->setObjectName(QString::fromUtf8("page_2_text"));
        stackedWidget->addWidget(page_2_text);
        page_3_bar = new QWidget();
        page_3_bar->setObjectName(QString::fromUtf8("page_3_bar"));
        stackedWidget->addWidget(page_3_bar);
        page_4_etime = new QWidget();
        page_4_etime->setObjectName(QString::fromUtf8("page_4_etime"));
        stackedWidget->addWidget(page_4_etime);
        page_5_etc = new QWidget();
        page_5_etc->setObjectName(QString::fromUtf8("page_5_etc"));
        stackedWidget->addWidget(page_5_etc);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 701, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(action_btn);
        menu->addAction(action_text);
        menu->addAction(action_bar);
        menu->addAction(action_datetime);
        menu->addAction(action_etc);
        menu_2->addAction(action_about);
        menu_2->addAction(action_exit);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_btn->setText(QApplication::translate("MainWindow", "\346\214\211\351\222\256", nullptr));
#ifndef QT_NO_TOOLTIP
        action_btn->setToolTip(QApplication::translate("MainWindow", "\346\214\211\351\222\256", nullptr));
#endif // QT_NO_TOOLTIP
        action_text->setText(QApplication::translate("MainWindow", "\346\226\207\346\234\254", nullptr));
        action_bar->setText(QApplication::translate("MainWindow", "\350\277\233\345\272\246\346\235\241", nullptr));
        action_datetime->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264\346\227\245\346\234\237", nullptr));
        action_etc->setText(QApplication::translate("MainWindow", "\345\205\266\344\273\226", nullptr));
        action_exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        action_about->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216Qt", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "QPushButton", nullptr));
        btn_1->setText(QApplication::translate("MainWindow", "\346\214\211\351\222\2561", nullptr));
        btn_2->setText(QApplication::translate("MainWindow", "\346\214\211\351\222\2562", nullptr));
        btn_3->setText(QApplication::translate("MainWindow", "\346\214\211\351\222\2563", nullptr));
        btn_4->setText(QApplication::translate("MainWindow", "\346\214\211\351\222\2564", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\346\216\247\344\273\266", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

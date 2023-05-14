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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_10;
    QPushButton *btn_openFile;
    QPushButton *btn_openFiles;
    QPushButton *btn_openDir;
    QPushButton *btn_save;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *btn_critical;
    QPushButton *btn_information;
    QPushButton *btn_question;
    QPushButton *btn_warning;
    QPushButton *btn_about;
    QPushButton *btn_aboutQt;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QPushButton *btn_getDouble;
    QPushButton *btn_getInt;
    QPushButton *btn_getItem;
    QPushButton *btn_getMultiLineText;
    QPushButton *btn_getText;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QPushButton *btn_logo;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QPushButton *btn_print;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QPushButton *btn_font;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_7;
    QPushButton *btn_color;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_8;
    QPushButton *btn_error;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_9;
    QPushButton *btn_progress;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(630, 501);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_10 = new QGridLayout(groupBox);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        btn_openFile = new QPushButton(groupBox);
        btn_openFile->setObjectName(QString::fromUtf8("btn_openFile"));

        gridLayout_10->addWidget(btn_openFile, 0, 0, 1, 1);

        btn_openFiles = new QPushButton(groupBox);
        btn_openFiles->setObjectName(QString::fromUtf8("btn_openFiles"));

        gridLayout_10->addWidget(btn_openFiles, 1, 0, 1, 1);

        btn_openDir = new QPushButton(groupBox);
        btn_openDir->setObjectName(QString::fromUtf8("btn_openDir"));

        gridLayout_10->addWidget(btn_openDir, 2, 0, 1, 1);

        btn_save = new QPushButton(groupBox);
        btn_save->setObjectName(QString::fromUtf8("btn_save"));

        gridLayout_10->addWidget(btn_save, 3, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 2, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        btn_critical = new QPushButton(groupBox_2);
        btn_critical->setObjectName(QString::fromUtf8("btn_critical"));

        gridLayout_2->addWidget(btn_critical, 0, 0, 1, 1);

        btn_information = new QPushButton(groupBox_2);
        btn_information->setObjectName(QString::fromUtf8("btn_information"));

        gridLayout_2->addWidget(btn_information, 1, 0, 1, 1);

        btn_question = new QPushButton(groupBox_2);
        btn_question->setObjectName(QString::fromUtf8("btn_question"));

        gridLayout_2->addWidget(btn_question, 2, 0, 1, 1);

        btn_warning = new QPushButton(groupBox_2);
        btn_warning->setObjectName(QString::fromUtf8("btn_warning"));

        gridLayout_2->addWidget(btn_warning, 3, 0, 1, 1);

        btn_about = new QPushButton(groupBox_2);
        btn_about->setObjectName(QString::fromUtf8("btn_about"));

        gridLayout_2->addWidget(btn_about, 4, 0, 1, 1);

        btn_aboutQt = new QPushButton(groupBox_2);
        btn_aboutQt->setObjectName(QString::fromUtf8("btn_aboutQt"));

        gridLayout_2->addWidget(btn_aboutQt, 5, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 1, 2, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        btn_getDouble = new QPushButton(groupBox_3);
        btn_getDouble->setObjectName(QString::fromUtf8("btn_getDouble"));

        gridLayout_3->addWidget(btn_getDouble, 0, 0, 1, 1);

        btn_getInt = new QPushButton(groupBox_3);
        btn_getInt->setObjectName(QString::fromUtf8("btn_getInt"));

        gridLayout_3->addWidget(btn_getInt, 1, 0, 1, 1);

        btn_getItem = new QPushButton(groupBox_3);
        btn_getItem->setObjectName(QString::fromUtf8("btn_getItem"));

        gridLayout_3->addWidget(btn_getItem, 2, 0, 1, 1);

        btn_getMultiLineText = new QPushButton(groupBox_3);
        btn_getMultiLineText->setObjectName(QString::fromUtf8("btn_getMultiLineText"));

        gridLayout_3->addWidget(btn_getMultiLineText, 3, 0, 1, 1);

        btn_getText = new QPushButton(groupBox_3);
        btn_getText->setObjectName(QString::fromUtf8("btn_getText"));

        gridLayout_3->addWidget(btn_getText, 4, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 0, 2, 2, 1);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        btn_logo = new QPushButton(groupBox_4);
        btn_logo->setObjectName(QString::fromUtf8("btn_logo"));

        gridLayout_4->addWidget(btn_logo, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_4, 0, 3, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        btn_print = new QPushButton(groupBox_5);
        btn_print->setObjectName(QString::fromUtf8("btn_print"));

        gridLayout_5->addWidget(btn_print, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_5, 1, 3, 1, 1);

        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        btn_font = new QPushButton(groupBox_6);
        btn_font->setObjectName(QString::fromUtf8("btn_font"));

        gridLayout_6->addWidget(btn_font, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_6, 2, 0, 1, 1);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_7 = new QGridLayout(groupBox_7);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        btn_color = new QPushButton(groupBox_7);
        btn_color->setObjectName(QString::fromUtf8("btn_color"));

        gridLayout_7->addWidget(btn_color, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_7, 2, 1, 1, 1);

        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_8 = new QGridLayout(groupBox_8);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        btn_error = new QPushButton(groupBox_8);
        btn_error->setObjectName(QString::fromUtf8("btn_error"));

        gridLayout_8->addWidget(btn_error, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_8, 2, 2, 1, 1);

        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_9 = new QGridLayout(groupBox_9);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        btn_progress = new QPushButton(groupBox_9);
        btn_progress->setObjectName(QString::fromUtf8("btn_progress"));

        gridLayout_9->addWidget(btn_progress, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_9, 2, 3, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout->addWidget(textEdit, 3, 0, 1, 4);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 630, 26));
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
        groupBox->setTitle(QApplication::translate("MainWindow", "QFileDialog", nullptr));
        btn_openFile->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        btn_openFiles->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\244\232\344\270\252\346\226\207\344\273\266", nullptr));
        btn_openDir->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", nullptr));
        btn_save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\226\207\344\273\266 ", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "QMessageBox", nullptr));
        btn_critical->setText(QApplication::translate("MainWindow", "critical", nullptr));
        btn_information->setText(QApplication::translate("MainWindow", "information", nullptr));
        btn_question->setText(QApplication::translate("MainWindow", "question", nullptr));
        btn_warning->setText(QApplication::translate("MainWindow", "warning", nullptr));
        btn_about->setText(QApplication::translate("MainWindow", "about", nullptr));
        btn_aboutQt->setText(QApplication::translate("MainWindow", "aboutQt", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "QInputDialog", nullptr));
        btn_getDouble->setText(QApplication::translate("MainWindow", "getDouble", nullptr));
        btn_getInt->setText(QApplication::translate("MainWindow", "getInt", nullptr));
        btn_getItem->setText(QApplication::translate("MainWindow", "getItem", nullptr));
        btn_getMultiLineText->setText(QApplication::translate("MainWindow", "getMultiLineText", nullptr));
        btn_getText->setText(QApplication::translate("MainWindow", "getText", nullptr));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "custom", nullptr));
        btn_logo->setText(QApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\345\257\271\350\257\235\346\241\206", nullptr));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "QPrintDialog", nullptr));
        btn_print->setText(QApplication::translate("MainWindow", "\346\211\223\345\215\260\346\234\272", nullptr));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "QFontDialog", nullptr));
        btn_font->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\255\227\344\275\223", nullptr));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "QColorDialog", nullptr));
        btn_color->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\351\242\234\350\211\262", nullptr));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "QErrorMessage", nullptr));
        btn_error->setText(QApplication::translate("MainWindow", "\351\224\231\350\257\257\346\266\210\346\201\257", nullptr));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "QProgressDialog", nullptr));
        btn_progress->setText(QApplication::translate("MainWindow", "\350\277\233\345\272\246\346\235\241", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

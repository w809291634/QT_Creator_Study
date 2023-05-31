/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QPushButton *pushButton_qobjectStart;
    QPushButton *pushButton_qobjectStop;
    QPushButton *pushButton_qobjectStart_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QProgressBar *progressBar_heart;
    QProgressBar *progressBar;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(495, 691);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 300));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        pushButton_qobjectStart = new QPushButton(tab_2);
        pushButton_qobjectStart->setObjectName(QString::fromUtf8("pushButton_qobjectStart"));
        pushButton_qobjectStart->setGeometry(QRect(30, 30, 221, 25));
        pushButton_qobjectStop = new QPushButton(tab_2);
        pushButton_qobjectStop->setObjectName(QString::fromUtf8("pushButton_qobjectStop"));
        pushButton_qobjectStop->setGeometry(QRect(80, 120, 191, 25));
        pushButton_qobjectStart_2 = new QPushButton(tab_2);
        pushButton_qobjectStart_2->setObjectName(QString::fromUtf8("pushButton_qobjectStart_2"));
        pushButton_qobjectStart_2->setGeometry(QRect(30, 60, 221, 25));
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        progressBar_heart = new QProgressBar(Widget);
        progressBar_heart->setObjectName(QString::fromUtf8("progressBar_heart"));
        progressBar_heart->setValue(24);

        horizontalLayout->addWidget(progressBar_heart);


        verticalLayout->addLayout(horizontalLayout);

        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMinimumSize(QSize(0, 300));

        verticalLayout->addWidget(textBrowser);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton_qobjectStart->setText(QApplication::translate("Widget", "QObject->moveToThread run", nullptr));
        pushButton_qobjectStop->setText(QApplication::translate("Widget", "stop from widget", nullptr));
        pushButton_qobjectStart_2->setText(QApplication::translate("Widget", "QObject->moveToThread run2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Widget", "\347\273\247\346\211\277QObject\347\272\277\347\250\213", nullptr));
        label->setText(QApplication::translate("Widget", "heartBeat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

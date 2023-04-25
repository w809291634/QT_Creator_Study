/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *btn_hide;
    QPushButton *btn_close;
    QPushButton *btn_send_to_main;
    QPushButton *btn_send_to_other;
    QLabel *label;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(300, 250);
        gridLayout_2 = new QGridLayout(Form);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(Form);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btn_hide = new QPushButton(groupBox);
        btn_hide->setObjectName(QString::fromUtf8("btn_hide"));

        gridLayout->addWidget(btn_hide, 0, 0, 1, 1);

        btn_close = new QPushButton(groupBox);
        btn_close->setObjectName(QString::fromUtf8("btn_close"));

        gridLayout->addWidget(btn_close, 1, 0, 1, 1);

        btn_send_to_main = new QPushButton(groupBox);
        btn_send_to_main->setObjectName(QString::fromUtf8("btn_send_to_main"));

        gridLayout->addWidget(btn_send_to_main, 2, 0, 1, 1);

        btn_send_to_other = new QPushButton(groupBox);
        btn_send_to_other->setObjectName(QString::fromUtf8("btn_send_to_other"));

        gridLayout->addWidget(btn_send_to_other, 3, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("Form", "\345\255\220\347\252\227\345\217\243", nullptr));
        btn_hide->setText(QApplication::translate("Form", "\351\232\220\350\227\217\347\252\227\345\217\243", nullptr));
        btn_close->setText(QApplication::translate("Form", "\345\205\263\351\227\255\346\234\254\347\252\227\345\217\243", nullptr));
        btn_send_to_main->setText(QApplication::translate("Form", "\345\217\221\351\200\201\344\277\241\345\217\267\345\210\260\344\270\273\347\252\227\345\217\243", nullptr));
        btn_send_to_other->setText(QApplication::translate("Form", "\345\217\221\351\200\201\344\277\241\345\217\267\345\210\260\345\220\214\347\261\273\347\252\227\345\217\243", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H

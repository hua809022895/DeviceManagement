/********************************************************************************
** Form generated from reading UI file 'DlgModifyTaskArea.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGMODIFYTASKAREA_H
#define UI_DLGMODIFYTASKAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgModifyTaskArea
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_1;
    QLabel *label_5;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QHBoxLayout *hboxLayout;
    QLineEdit *lineEdit_3;
    QLabel *label_6;
    QLabel *label_3;
    QHBoxLayout *hboxLayout1;
    QLineEdit *lineEdit_4;
    QLabel *label_7;
    QLabel *label_9;
    QHBoxLayout *hboxLayout2;
    QLineEdit *lineEdit_5;
    QLabel *label_8;
    QLabel *label_14;
    QHBoxLayout *hboxLayout3;
    QLineEdit *lineEdit_9;
    QLabel *label_13;
    QLabel *label_10;
    QHBoxLayout *hboxLayout4;
    QLineEdit *lineEdit_6;
    QLabel *label_11;
    QLabel *label_4;
    QLineEdit *lineEdit_7;
    QLabel *label_12;
    QLineEdit *lineEdit_8;
    QSpacerItem *spacerItem;
    QHBoxLayout *btnRow;
    QSpacerItem *spacerItem1;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DlgModifyTaskArea)
    {
        if (DlgModifyTaskArea->objectName().isEmpty())
            DlgModifyTaskArea->setObjectName(QString::fromUtf8("DlgModifyTaskArea"));
        DlgModifyTaskArea->resize(340, 460);
        DlgModifyTaskArea->setMinimumSize(QSize(280, 380));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        DlgModifyTaskArea->setFont(font);
        mainLayout = new QVBoxLayout(DlgModifyTaskArea);
        mainLayout->setSpacing(8);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(12, 12, 12, 12);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(8);
        formLayout->setVerticalSpacing(8);
        label = new QLabel(DlgModifyTaskArea);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_1 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_1->setObjectName(QString::fromUtf8("lineEdit_1"));
        lineEdit_1->setReadOnly(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_1);

        label_5 = new QLabel(DlgModifyTaskArea);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        lineEdit_2 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setReadOnly(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_2);

        label_2 = new QLabel(DlgModifyTaskArea);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lineEdit_3 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        hboxLayout->addWidget(lineEdit_3);

        label_6 = new QLabel(DlgModifyTaskArea);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        hboxLayout->addWidget(label_6);


        formLayout->setLayout(2, QFormLayout::FieldRole, hboxLayout);

        label_3 = new QLabel(DlgModifyTaskArea);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        lineEdit_4 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        hboxLayout1->addWidget(lineEdit_4);

        label_7 = new QLabel(DlgModifyTaskArea);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        hboxLayout1->addWidget(label_7);


        formLayout->setLayout(3, QFormLayout::FieldRole, hboxLayout1);

        label_9 = new QLabel(DlgModifyTaskArea);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_9);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        lineEdit_5 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        hboxLayout2->addWidget(lineEdit_5);

        label_8 = new QLabel(DlgModifyTaskArea);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        hboxLayout2->addWidget(label_8);


        formLayout->setLayout(4, QFormLayout::FieldRole, hboxLayout2);

        label_14 = new QLabel(DlgModifyTaskArea);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_14);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        lineEdit_9 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));

        hboxLayout3->addWidget(lineEdit_9);

        label_13 = new QLabel(DlgModifyTaskArea);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        hboxLayout3->addWidget(label_13);


        formLayout->setLayout(5, QFormLayout::FieldRole, hboxLayout3);

        label_10 = new QLabel(DlgModifyTaskArea);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_10);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        lineEdit_6 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        hboxLayout4->addWidget(lineEdit_6);

        label_11 = new QLabel(DlgModifyTaskArea);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        hboxLayout4->addWidget(label_11);


        formLayout->setLayout(6, QFormLayout::FieldRole, hboxLayout4);

        label_4 = new QLabel(DlgModifyTaskArea);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_4);

        lineEdit_7 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        formLayout->setWidget(7, QFormLayout::FieldRole, lineEdit_7);

        label_12 = new QLabel(DlgModifyTaskArea);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_12);

        lineEdit_8 = new QLineEdit(DlgModifyTaskArea);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));

        formLayout->setWidget(8, QFormLayout::FieldRole, lineEdit_8);


        mainLayout->addLayout(formLayout);

        spacerItem = new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(spacerItem);

        btnRow = new QHBoxLayout();
        btnRow->setSpacing(8);
        btnRow->setObjectName(QString::fromUtf8("btnRow"));
        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        btnRow->addItem(spacerItem1);

        okButton = new QPushButton(DlgModifyTaskArea);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        btnRow->addWidget(okButton);

        cancelButton = new QPushButton(DlgModifyTaskArea);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        btnRow->addWidget(cancelButton);


        mainLayout->addLayout(btnRow);


        retranslateUi(DlgModifyTaskArea);
        QObject::connect(cancelButton, SIGNAL(clicked()), DlgModifyTaskArea, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgModifyTaskArea);
    } // setupUi

    void retranslateUi(QDialog *DlgModifyTaskArea)
    {
        DlgModifyTaskArea->setWindowTitle(QApplication::translate("DlgModifyTaskArea", "\344\277\256\346\224\271 \344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
        label->setText(QApplication::translate("DlgModifyTaskArea", "\345\235\220\346\240\207", nullptr));
        label_5->setText(QApplication::translate("DlgModifyTaskArea", "\344\273\273\345\212\241\345\214\272\345\237\237ID", nullptr));
        label_2->setText(QApplication::translate("DlgModifyTaskArea", "\345\256\275\345\272\246", nullptr));
        label_6->setText(QApplication::translate("DlgModifyTaskArea", "\347\261\263", nullptr));
        label_3->setText(QApplication::translate("DlgModifyTaskArea", "\351\225\277\345\272\246", nullptr));
        label_7->setText(QApplication::translate("DlgModifyTaskArea", "\347\261\263", nullptr));
        label_9->setText(QApplication::translate("DlgModifyTaskArea", "\351\253\230\345\272\246\344\270\213\351\231\220", nullptr));
        label_8->setText(QApplication::translate("DlgModifyTaskArea", "\347\261\263", nullptr));
        label_14->setText(QApplication::translate("DlgModifyTaskArea", "\351\253\230\345\272\246\344\270\212\351\231\220", nullptr));
        label_13->setText(QApplication::translate("DlgModifyTaskArea", "\347\261\263", nullptr));
        label_10->setText(QApplication::translate("DlgModifyTaskArea", "\351\243\236\350\241\214\346\227\266\351\227\264", nullptr));
        label_11->setText(QApplication::translate("DlgModifyTaskArea", "\345\215\225\344\275\215(\345\210\206\351\222\237)", nullptr));
        label_4->setText(QApplication::translate("DlgModifyTaskArea", "\350\247\222\345\272\246", nullptr));
        label_12->setText(QApplication::translate("DlgModifyTaskArea", "\345\244\207\346\263\250", nullptr));
        okButton->setText(QApplication::translate("DlgModifyTaskArea", "\347\241\256\345\256\232", nullptr));
        cancelButton->setText(QApplication::translate("DlgModifyTaskArea", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgModifyTaskArea: public Ui_DlgModifyTaskArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGMODIFYTASKAREA_H

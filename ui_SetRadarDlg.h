/********************************************************************************
** Form generated from reading UI file 'SetRadarDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETRADARDLG_H
#define UI_SETRADARDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *label_9;
    QLineEdit *textEdit_1;
    QLabel *label_8;
    QLineEdit *textEdit_2;
    QLabel *label_2;
    QLineEdit *textEdit_4;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QLineEdit *textEdit_3;
    QLabel *label_7;
    QLabel *label_10;
    QHBoxLayout *hboxLayout1;
    QLineEdit *textEdit_7;
    QLabel *label_14;
    QLabel *label_11;
    QHBoxLayout *hboxLayout2;
    QLineEdit *textEdit_8;
    QLabel *label_15;
    QLabel *label_4;
    QHBoxLayout *hboxLayout3;
    QLineEdit *textEdit_6;
    QLabel *label_19;
    QLabel *label_12;
    QHBoxLayout *hboxLayout4;
    QLineEdit *textEdit_9;
    QLabel *label_16;
    QLabel *label_13;
    QHBoxLayout *hboxLayout5;
    QLineEdit *textEdit_10;
    QLabel *label_17;
    QLabel *label_5;
    QHBoxLayout *hboxLayout6;
    QLineEdit *textEdit_11;
    QLabel *label_6;
    QLabel *label_uav;
    QLineEdit *textEdit_12;
    QSpacerItem *spacerItem;
    QHBoxLayout *buttonRow;
    QSpacerItem *spacerItem1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(360, 455);
        Form->setMinimumSize(QSize(280, 360));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        Form->setFont(font);
        mainLayout = new QVBoxLayout(Form);
        mainLayout->setSpacing(8);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(12, 12, 12, 12);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(8);
        formLayout->setVerticalSpacing(8);
        label_9 = new QLabel(Form);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_9);

        textEdit_1 = new QLineEdit(Form);
        textEdit_1->setObjectName(QString::fromUtf8("textEdit_1"));

        formLayout->setWidget(0, QFormLayout::FieldRole, textEdit_1);

        label_8 = new QLabel(Form);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_8);

        textEdit_2 = new QLineEdit(Form);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setReadOnly(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, textEdit_2);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        textEdit_4 = new QLineEdit(Form);
        textEdit_4->setObjectName(QString::fromUtf8("textEdit_4"));
        textEdit_4->setReadOnly(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, textEdit_4);

        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textEdit_3 = new QLineEdit(Form);
        textEdit_3->setObjectName(QString::fromUtf8("textEdit_3"));

        hboxLayout->addWidget(textEdit_3);

        label_7 = new QLabel(Form);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        hboxLayout->addWidget(label_7);


        formLayout->setLayout(3, QFormLayout::FieldRole, hboxLayout);

        label_10 = new QLabel(Form);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_10);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textEdit_7 = new QLineEdit(Form);
        textEdit_7->setObjectName(QString::fromUtf8("textEdit_7"));

        hboxLayout1->addWidget(textEdit_7);

        label_14 = new QLabel(Form);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        hboxLayout1->addWidget(label_14);


        formLayout->setLayout(4, QFormLayout::FieldRole, hboxLayout1);

        label_11 = new QLabel(Form);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_11);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        textEdit_8 = new QLineEdit(Form);
        textEdit_8->setObjectName(QString::fromUtf8("textEdit_8"));

        hboxLayout2->addWidget(textEdit_8);

        label_15 = new QLabel(Form);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        hboxLayout2->addWidget(label_15);


        formLayout->setLayout(5, QFormLayout::FieldRole, hboxLayout2);

        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_4);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        textEdit_6 = new QLineEdit(Form);
        textEdit_6->setObjectName(QString::fromUtf8("textEdit_6"));

        hboxLayout3->addWidget(textEdit_6);

        label_19 = new QLabel(Form);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        hboxLayout3->addWidget(label_19);


        formLayout->setLayout(6, QFormLayout::FieldRole, hboxLayout3);

        label_12 = new QLabel(Form);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_12);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        textEdit_9 = new QLineEdit(Form);
        textEdit_9->setObjectName(QString::fromUtf8("textEdit_9"));

        hboxLayout4->addWidget(textEdit_9);

        label_16 = new QLabel(Form);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        hboxLayout4->addWidget(label_16);


        formLayout->setLayout(7, QFormLayout::FieldRole, hboxLayout4);

        label_13 = new QLabel(Form);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_13);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        textEdit_10 = new QLineEdit(Form);
        textEdit_10->setObjectName(QString::fromUtf8("textEdit_10"));

        hboxLayout5->addWidget(textEdit_10);

        label_17 = new QLabel(Form);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        hboxLayout5->addWidget(label_17);


        formLayout->setLayout(8, QFormLayout::FieldRole, hboxLayout5);

        label_5 = new QLabel(Form);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_5);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        textEdit_11 = new QLineEdit(Form);
        textEdit_11->setObjectName(QString::fromUtf8("textEdit_11"));

        hboxLayout6->addWidget(textEdit_11);

        label_6 = new QLabel(Form);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        hboxLayout6->addWidget(label_6);


        formLayout->setLayout(9, QFormLayout::FieldRole, hboxLayout6);

        label_uav = new QLabel(Form);
        label_uav->setObjectName(QString::fromUtf8("label_uav"));

        formLayout->setWidget(10, QFormLayout::LabelRole, label_uav);

        textEdit_12 = new QLineEdit(Form);
        textEdit_12->setObjectName(QString::fromUtf8("textEdit_12"));

        formLayout->setWidget(10, QFormLayout::FieldRole, textEdit_12);


        mainLayout->addLayout(formLayout);

        spacerItem = new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(spacerItem);

        buttonRow = new QHBoxLayout();
        buttonRow->setSpacing(8);
        buttonRow->setObjectName(QString::fromUtf8("buttonRow"));
        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonRow->addItem(spacerItem1);

        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        buttonRow->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(Form);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        buttonRow->addWidget(pushButton_3);


        mainLayout->addLayout(buttonRow);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "\346\216\242\346\265\213\350\256\276\345\244\207", nullptr));
        label_9->setText(QApplication::translate("Form", "\350\256\276\345\244\207 ID", nullptr));
        label_8->setText(QApplication::translate("Form", "\350\256\276\345\244\207\347\261\273\345\236\213", nullptr));
        label_2->setText(QApplication::translate("Form", "\350\256\276\345\244\207\344\275\215\347\275\256", nullptr));
        label->setText(QApplication::translate("Form", "\350\256\276\345\244\207\351\253\230\345\272\246", nullptr));
        label_7->setText(QApplication::translate("Form", "\345\215\225\344\275\215(\347\261\263)", nullptr));
        label_10->setText(QApplication::translate("Form", "\346\226\271\344\275\215\350\247\222\316\270", nullptr));
        label_14->setText(QApplication::translate("Form", "\345\272\246", nullptr));
        label_11->setText(QApplication::translate("Form", "\344\277\260\344\273\260\350\247\222\316\261", nullptr));
        label_15->setText(QApplication::translate("Form", "\345\272\246", nullptr));
        label_4->setText(QApplication::translate("Form", "\346\216\242\346\265\213\350\214\203\345\233\264\350\247\222\345\272\246", nullptr));
        label_19->setText(QApplication::translate("Form", "\345\272\246", nullptr));
        label_12->setText(QApplication::translate("Form", "\346\260\264\345\271\263\346\263\242\346\235\237\345\256\275\345\272\246", nullptr));
        label_16->setText(QApplication::translate("Form", "\345\272\246", nullptr));
        label_13->setText(QApplication::translate("Form", "\345\236\202\347\233\264\346\263\242\346\235\237\345\256\275\345\272\246", nullptr));
        label_17->setText(QApplication::translate("Form", "\345\272\246", nullptr));
        label_5->setText(QApplication::translate("Form", "\346\216\242\346\265\213\350\267\235\347\246\273", nullptr));
        label_6->setText(QApplication::translate("Form", "\345\215\225\344\275\215(\347\261\263)", nullptr));
        label_uav->setText(QApplication::translate("Form", "\350\243\205\350\275\275\346\227\240\344\272\272\346\234\272\346\234\272\345\217\267", nullptr));
        textEdit_12->setPlaceholderText(QApplication::translate("Form", "\347\251\272\346\210\2260=\351\235\231\346\255\242\357\274\214\345\241\253\346\234\272\345\217\267\351\232\217\346\227\240\344\272\272\346\234\272\347\247\273\345\212\250", nullptr));
        pushButton_2->setText(QApplication::translate("Form", "\347\241\256\345\256\232", nullptr));
        pushButton_3->setText(QApplication::translate("Form", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETRADARDLG_H

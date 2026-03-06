/********************************************************************************
** Form generated from reading UI file 'marketCircleDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKETCIRCLEDIALOG_H
#define UI_MARKETCIRCLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarketCircleDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *mLineEditR;
    QLineEdit *mLineEditId;
    QComboBox *mComboBoxClass;
    QLabel *label;
    QLabel *label_7;
    QLabel *label_2;
    QDateTimeEdit *mDateTimeEditEnd;
    QDateTimeEdit *mDateTimeEditBegin;
    QLabel *label_5;
    QLabel *label_3;
    QLineEdit *mLineEditName;
    QLabel *label_8;
    QLineEdit *mLineEditHeight;
    QLabel *label_6;
    QLabel *label_4;
    QLineEdit *mLineEditPosition;

    void setupUi(QDialog *MarketCircleDialog)
    {
        if (MarketCircleDialog->objectName().isEmpty())
            MarketCircleDialog->setObjectName(QString::fromUtf8("MarketCircleDialog"));
        MarketCircleDialog->resize(525, 355);
        buttonBox = new QDialogButtonBox(MarketCircleDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(170, 320, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(MarketCircleDialog);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 30, 451, 271));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        mLineEditR = new QLineEdit(gridLayoutWidget);
        mLineEditR->setObjectName(QString::fromUtf8("mLineEditR"));

        gridLayout->addWidget(mLineEditR, 5, 2, 1, 1);

        mLineEditId = new QLineEdit(gridLayoutWidget);
        mLineEditId->setObjectName(QString::fromUtf8("mLineEditId"));

        gridLayout->addWidget(mLineEditId, 0, 2, 1, 1);

        mComboBoxClass = new QComboBox(gridLayoutWidget);
        mComboBoxClass->setObjectName(QString::fromUtf8("mComboBoxClass"));
        mComboBoxClass->setEditable(true);

        gridLayout->addWidget(mComboBoxClass, 1, 2, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        mDateTimeEditEnd = new QDateTimeEdit(gridLayoutWidget);
        mDateTimeEditEnd->setObjectName(QString::fromUtf8("mDateTimeEditEnd"));

        gridLayout->addWidget(mDateTimeEditEnd, 7, 2, 1, 1);

        mDateTimeEditBegin = new QDateTimeEdit(gridLayoutWidget);
        mDateTimeEditBegin->setObjectName(QString::fromUtf8("mDateTimeEditBegin"));

        gridLayout->addWidget(mDateTimeEditBegin, 6, 2, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        mLineEditName = new QLineEdit(gridLayoutWidget);
        mLineEditName->setObjectName(QString::fromUtf8("mLineEditName"));

        gridLayout->addWidget(mLineEditName, 2, 2, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        mLineEditHeight = new QLineEdit(gridLayoutWidget);
        mLineEditHeight->setObjectName(QString::fromUtf8("mLineEditHeight"));

        gridLayout->addWidget(mLineEditHeight, 3, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        mLineEditPosition = new QLineEdit(gridLayoutWidget);
        mLineEditPosition->setObjectName(QString::fromUtf8("mLineEditPosition"));

        gridLayout->addWidget(mLineEditPosition, 4, 2, 1, 1);


        retranslateUi(MarketCircleDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MarketCircleDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MarketCircleDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MarketCircleDialog);
    } // setupUi

    void retranslateUi(QDialog *MarketCircleDialog)
    {
        MarketCircleDialog->setWindowTitle(QApplication::translate("MarketCircleDialog", "\345\234\206\345\275\242\346\240\207\346\263\250\350\256\276\347\275\256", nullptr));
        label->setText(QApplication::translate("MarketCircleDialog", "\347\274\226\345\217\267\357\274\232", nullptr));
        label_7->setText(QApplication::translate("MarketCircleDialog", "\350\265\267\345\247\213\346\227\266\351\227\264\357\274\232", nullptr));
        label_2->setText(QApplication::translate("MarketCircleDialog", "\347\261\273\345\210\253\357\274\232", nullptr));
        label_5->setText(QApplication::translate("MarketCircleDialog", "\347\246\201\351\243\236\351\253\230\345\272\246\357\274\232", nullptr));
        label_3->setText(QApplication::translate("MarketCircleDialog", "\345\220\215\347\247\260\357\274\232", nullptr));
        label_8->setText(QApplication::translate("MarketCircleDialog", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", nullptr));
        label_6->setText(QApplication::translate("MarketCircleDialog", "\347\246\201\351\243\236\350\214\203\345\233\264(\347\261\263)\357\274\232", nullptr));
        label_4->setText(QApplication::translate("MarketCircleDialog", "\344\275\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MarketCircleDialog: public Ui_MarketCircleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKETCIRCLEDIALOG_H

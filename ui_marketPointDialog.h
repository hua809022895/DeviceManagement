/********************************************************************************
** Form generated from reading UI file 'marketPointDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKETPOINTDIALOG_H
#define UI_MARKETPOINTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarketPointDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *mLineEditName;
    QLabel *label_2;
    QComboBox *mComboBoxClass;
    QLabel *label_3;
    QLineEdit *mLineEditId;
    QLineEdit *mLineEditPosition;

    void setupUi(QDialog *MarketPointDialog)
    {
        if (MarketPointDialog->objectName().isEmpty())
            MarketPointDialog->setObjectName(QString::fromUtf8("MarketPointDialog"));
        MarketPointDialog->resize(502, 276);
        buttonBox = new QDialogButtonBox(MarketPointDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(130, 220, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(MarketPointDialog);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 30, 441, 171));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        mLineEditName = new QLineEdit(gridLayoutWidget);
        mLineEditName->setObjectName(QString::fromUtf8("mLineEditName"));

        gridLayout->addWidget(mLineEditName, 2, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        mComboBoxClass = new QComboBox(gridLayoutWidget);
        mComboBoxClass->setObjectName(QString::fromUtf8("mComboBoxClass"));
        mComboBoxClass->setEditable(true);

        gridLayout->addWidget(mComboBoxClass, 1, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        mLineEditId = new QLineEdit(gridLayoutWidget);
        mLineEditId->setObjectName(QString::fromUtf8("mLineEditId"));

        gridLayout->addWidget(mLineEditId, 0, 1, 1, 1);

        mLineEditPosition = new QLineEdit(gridLayoutWidget);
        mLineEditPosition->setObjectName(QString::fromUtf8("mLineEditPosition"));

        gridLayout->addWidget(mLineEditPosition, 3, 1, 1, 1);


        retranslateUi(MarketPointDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MarketPointDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MarketPointDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MarketPointDialog);
    } // setupUi

    void retranslateUi(QDialog *MarketPointDialog)
    {
        MarketPointDialog->setWindowTitle(QApplication::translate("MarketPointDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("MarketPointDialog", "\347\274\226\345\217\267\357\274\232", nullptr));
        label_4->setText(QApplication::translate("MarketPointDialog", "\344\275\215\347\275\256\357\274\232", nullptr));
        label_2->setText(QApplication::translate("MarketPointDialog", "\347\261\273\345\210\253\357\274\232", nullptr));
        label_3->setText(QApplication::translate("MarketPointDialog", "\345\220\215\347\247\260\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MarketPointDialog: public Ui_MarketPointDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKETPOINTDIALOG_H

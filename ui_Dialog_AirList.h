/********************************************************************************
** Form generated from reading UI file 'Dialog_AirList.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_AIRLIST_H
#define UI_DIALOG_AIRLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogAirList
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *mAirTable;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *ok_Button;
    QPushButton *pushExitBtn;

    void setupUi(QDialog *DialogAirList)
    {
        if (DialogAirList->objectName().isEmpty())
            DialogAirList->setObjectName(QString::fromUtf8("DialogAirList"));
        DialogAirList->resize(412, 790);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        DialogAirList->setFont(font);
        DialogAirList->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(DialogAirList);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mAirTable = new QTableWidget(DialogAirList);
        mAirTable->setObjectName(QString::fromUtf8("mAirTable"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(mAirTable->sizePolicy().hasHeightForWidth());
        mAirTable->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(mAirTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(DialogAirList);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(DialogAirList);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ok_Button = new QPushButton(DialogAirList);
        ok_Button->setObjectName(QString::fromUtf8("ok_Button"));

        horizontalLayout->addWidget(ok_Button);

        pushExitBtn = new QPushButton(DialogAirList);
        pushExitBtn->setObjectName(QString::fromUtf8("pushExitBtn"));

        horizontalLayout->addWidget(pushExitBtn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogAirList);
        QObject::connect(pushExitBtn, SIGNAL(clicked()), DialogAirList, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogAirList);
    } // setupUi

    void retranslateUi(QDialog *DialogAirList)
    {
        DialogAirList->setWindowTitle(QApplication::translate("DialogAirList", "\345\256\236\346\227\266\346\216\242\346\265\213\346\227\240\344\272\272\346\234\272\344\277\241\346\201\257", nullptr));
        label->setText(QApplication::translate("DialogAirList", "\350\243\205\345\244\207\345\210\227\350\241\250", nullptr));
        ok_Button->setText(QApplication::translate("DialogAirList", "\347\255\233\351\200\211", nullptr));
        pushExitBtn->setText(QApplication::translate("DialogAirList", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogAirList: public Ui_DialogAirList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_AIRLIST_H

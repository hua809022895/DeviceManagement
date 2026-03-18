/********************************************************************************
** Form generated from reading UI file 'DlgAreaRadarList.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGAREARADARLIST_H
#define UI_DLGAREARADARLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgAreaRaderList
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *headerLayout;
    QLabel *label;
    QSpacerItem *headerSpacer;
    QLabel *label_2;
    QHBoxLayout *listLayout;
    QListWidget *listWidget1;
    QListWidget *listWidget2;
    QHBoxLayout *bottomLayout;
    QLabel *label_3;
    QSpacerItem *bottomSpacer;
    QPushButton *cancelButton;

    void setupUi(QDialog *DlgAreaRaderList)
    {
        if (DlgAreaRaderList->objectName().isEmpty())
            DlgAreaRaderList->setObjectName(QString::fromUtf8("DlgAreaRaderList"));
        DlgAreaRaderList->resize(345, 407);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        DlgAreaRaderList->setFont(font);
        DlgAreaRaderList->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(DlgAreaRaderList);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName(QString::fromUtf8("headerLayout"));
        label = new QLabel(DlgAreaRaderList);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(12);
        label->setFont(font1);

        headerLayout->addWidget(label);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        headerLayout->addItem(headerSpacer);

        label_2 = new QLabel(DlgAreaRaderList);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        headerLayout->addWidget(label_2);


        verticalLayout->addLayout(headerLayout);

        listLayout = new QHBoxLayout();
        listLayout->setObjectName(QString::fromUtf8("listLayout"));
        listWidget1 = new QListWidget(DlgAreaRaderList);
        listWidget1->setObjectName(QString::fromUtf8("listWidget1"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget1->sizePolicy().hasHeightForWidth());
        listWidget1->setSizePolicy(sizePolicy);

        listLayout->addWidget(listWidget1);

        listWidget2 = new QListWidget(DlgAreaRaderList);
        listWidget2->setObjectName(QString::fromUtf8("listWidget2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget2->sizePolicy().hasHeightForWidth());
        listWidget2->setSizePolicy(sizePolicy1);

        listLayout->addWidget(listWidget2);


        verticalLayout->addLayout(listLayout);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setObjectName(QString::fromUtf8("bottomLayout"));
        label_3 = new QLabel(DlgAreaRaderList);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        bottomLayout->addWidget(label_3);

        bottomSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomLayout->addItem(bottomSpacer);

        cancelButton = new QPushButton(DlgAreaRaderList);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        bottomLayout->addWidget(cancelButton);


        verticalLayout->addLayout(bottomLayout);


        retranslateUi(DlgAreaRaderList);
        QObject::connect(cancelButton, SIGNAL(clicked()), DlgAreaRaderList, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgAreaRaderList);
    } // setupUi

    void retranslateUi(QDialog *DlgAreaRaderList)
    {
        DlgAreaRaderList->setWindowTitle(QApplication::translate("DlgAreaRaderList", "\350\243\205\345\244\207\350\246\206\347\233\226\345\210\227\350\241\250", nullptr));
        label->setText(QApplication::translate("DlgAreaRaderList", "\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
        label_2->setText(QApplication::translate("DlgAreaRaderList", "\346\211\200\350\246\206\347\233\226\350\243\205\345\244\207\345\220\215\347\247\260", nullptr));
        label_3->setText(QApplication::translate("DlgAreaRaderList", "\344\275\223\347\247\257\345\215\225\344\275\215: \347\253\213\346\226\271\345\215\203\347\261\263 (km3)", nullptr));
        cancelButton->setText(QApplication::translate("DlgAreaRaderList", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgAreaRaderList: public Ui_DlgAreaRaderList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGAREARADARLIST_H

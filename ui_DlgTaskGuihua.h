/********************************************************************************
** Form generated from reading UI file 'DlgTaskGuihua.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGTASKGUIHUA_H
#define UI_DLGTASKGUIHUA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgTaskGuihua
{
public:
    QVBoxLayout *mainLayout;
    QHBoxLayout *tableLayout;
    QTableWidget *mWidget;
    QTableWidget *mWidget_2;
    QHBoxLayout *btnLayout;
    QSpacerItem *btnSpacer;
    QPushButton *okButton;

    void setupUi(QDialog *DlgTaskGuihua)
    {
        if (DlgTaskGuihua->objectName().isEmpty())
            DlgTaskGuihua->setObjectName(QString::fromUtf8("DlgTaskGuihua"));
        DlgTaskGuihua->resize(960, 580);
        mainLayout = new QVBoxLayout(DlgTaskGuihua);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        tableLayout = new QHBoxLayout();
        tableLayout->setObjectName(QString::fromUtf8("tableLayout"));
        mWidget = new QTableWidget(DlgTaskGuihua);
        mWidget->setObjectName(QString::fromUtf8("mWidget"));
        mWidget->setMinimumWidth(300);

        tableLayout->addWidget(mWidget);

        mWidget_2 = new QTableWidget(DlgTaskGuihua);
        mWidget_2->setObjectName(QString::fromUtf8("mWidget_2"));
        mWidget_2->setMinimumWidth(500);

        tableLayout->addWidget(mWidget_2);


        mainLayout->addLayout(tableLayout);

        btnLayout = new QHBoxLayout();
        btnLayout->setObjectName(QString::fromUtf8("btnLayout"));
        btnSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        btnLayout->addItem(btnSpacer);

        okButton = new QPushButton(DlgTaskGuihua);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumWidth(100);

        btnLayout->addWidget(okButton);


        mainLayout->addLayout(btnLayout);


        retranslateUi(DlgTaskGuihua);
        QObject::connect(okButton, SIGNAL(clicked()), DlgTaskGuihua, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgTaskGuihua);
    } // setupUi

    void retranslateUi(QDialog *DlgTaskGuihua)
    {
        DlgTaskGuihua->setWindowTitle(QApplication::translate("DlgTaskGuihua", "\344\273\273\345\212\241\350\247\204\345\210\222", nullptr));
        okButton->setText(QApplication::translate("DlgTaskGuihua", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgTaskGuihua: public Ui_DlgTaskGuihua {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGTASKGUIHUA_H

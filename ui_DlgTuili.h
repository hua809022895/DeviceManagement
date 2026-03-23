/********************************************************************************
** Form generated from reading UI file 'DlgTuili.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGTUILI_H
#define UI_DLGTUILI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgTuili
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topLayout;
    QPushButton *ok_Button;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushExitBtn;
    QTableWidget *mAirTable;

    void setupUi(QDialog *DlgTuili)
    {
        if (DlgTuili->objectName().isEmpty())
            DlgTuili->setObjectName(QString::fromUtf8("DlgTuili"));
        DlgTuili->resize(800, 600);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        DlgTuili->setFont(font);
        verticalLayout = new QVBoxLayout(DlgTuili);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        topLayout = new QHBoxLayout();
        topLayout->setObjectName(QString::fromUtf8("topLayout"));
        ok_Button = new QPushButton(DlgTuili);
        ok_Button->setObjectName(QString::fromUtf8("ok_Button"));
        ok_Button->setMinimumSize(QSize(100, 0));
        ok_Button->setMaximumSize(QSize(160, 16777215));

        topLayout->addWidget(ok_Button);

        label = new QLabel(DlgTuili);
        label->setObjectName(QString::fromUtf8("label"));

        topLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        topLayout->addItem(horizontalSpacer);

        pushExitBtn = new QPushButton(DlgTuili);
        pushExitBtn->setObjectName(QString::fromUtf8("pushExitBtn"));
        pushExitBtn->setMaximumSize(QSize(100, 16777215));

        topLayout->addWidget(pushExitBtn);


        verticalLayout->addLayout(topLayout);

        mAirTable = new QTableWidget(DlgTuili);
        mAirTable->setObjectName(QString::fromUtf8("mAirTable"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mAirTable->sizePolicy().hasHeightForWidth());
        mAirTable->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(mAirTable);


        retranslateUi(DlgTuili);
        QObject::connect(pushExitBtn, SIGNAL(clicked()), DlgTuili, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgTuili);
    } // setupUi

    void retranslateUi(QDialog *DlgTuili)
    {
        DlgTuili->setWindowTitle(QApplication::translate("DlgTuili", "\345\256\236\346\227\266\351\233\206\347\276\244\346\216\250\347\220\206", nullptr));
        ok_Button->setText(QApplication::translate("DlgTuili", "\345\274\200\345\247\213\346\216\250\347\220\206", nullptr));
        label->setText(QApplication::translate("DlgTuili", "....", nullptr));
        pushExitBtn->setText(QApplication::translate("DlgTuili", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgTuili: public Ui_DlgTuili {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGTUILI_H

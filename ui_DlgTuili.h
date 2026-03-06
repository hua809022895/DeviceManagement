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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgTuili
{
public:
    QPushButton *pushExitBtn;
    QPushButton *ok_Button;
    QLabel *label;
    QTableWidget *mAirTable;

    void setupUi(QDialog *DlgTuili)
    {
        if (DlgTuili->objectName().isEmpty())
            DlgTuili->setObjectName(QString::fromUtf8("DlgTuili"));
        DlgTuili->resize(717, 815);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        DlgTuili->setFont(font);
        DlgTuili->setWindowOpacity(6.000000000000000);
        pushExitBtn = new QPushButton(DlgTuili);
        pushExitBtn->setObjectName(QString::fromUtf8("pushExitBtn"));
        pushExitBtn->setGeometry(QRect(620, 10, 75, 27));
        ok_Button = new QPushButton(DlgTuili);
        ok_Button->setObjectName(QString::fromUtf8("ok_Button"));
        ok_Button->setGeometry(QRect(20, 10, 100, 27));
        label = new QLabel(DlgTuili);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 10, 211, 30));
        mAirTable = new QTableWidget(DlgTuili);
        mAirTable->setObjectName(QString::fromUtf8("mAirTable"));
        mAirTable->setGeometry(QRect(0, 40, 711, 771));

        retranslateUi(DlgTuili);
        QObject::connect(pushExitBtn, SIGNAL(clicked()), DlgTuili, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgTuili);
    } // setupUi

    void retranslateUi(QDialog *DlgTuili)
    {
        DlgTuili->setWindowTitle(QApplication::translate("DlgTuili", "\345\256\236\346\227\266\351\233\206\347\276\244\346\216\250\347\220\206", nullptr));
        pushExitBtn->setText(QApplication::translate("DlgTuili", "\345\205\263\351\227\255", nullptr));
        ok_Button->setText(QApplication::translate("DlgTuili", "\345\274\200\345\247\213\346\216\250\347\220\206", nullptr));
        label->setText(QApplication::translate("DlgTuili", "....", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgTuili: public Ui_DlgTuili {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGTUILI_H

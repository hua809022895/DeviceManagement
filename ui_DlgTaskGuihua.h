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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgTaskGuihua
{
public:
    QTableWidget *mWidget;
    QPushButton *okButton;
    QFrame *frame_4;
    QTableWidget *mWidget_2;

    void setupUi(QDialog *DlgTaskGuihua)
    {
        if (DlgTaskGuihua->objectName().isEmpty())
            DlgTaskGuihua->setObjectName(QString::fromUtf8("DlgTaskGuihua"));
        DlgTaskGuihua->resize(844, 565);
        mWidget = new QTableWidget(DlgTaskGuihua);
        mWidget->setObjectName(QString::fromUtf8("mWidget"));
        mWidget->setGeometry(QRect(0, 0, 321, 521));
        okButton = new QPushButton(DlgTaskGuihua);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(720, 530, 100, 30));
        frame_4 = new QFrame(DlgTaskGuihua);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(500, 369, 20, 151));
        frame_4->setFrameShape(QFrame::VLine);
        frame_4->setFrameShadow(QFrame::Raised);
        mWidget_2 = new QTableWidget(DlgTaskGuihua);
        mWidget_2->setObjectName(QString::fromUtf8("mWidget_2"));
        mWidget_2->setGeometry(QRect(320, 0, 521, 521));

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

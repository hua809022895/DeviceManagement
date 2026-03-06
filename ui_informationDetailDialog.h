/********************************************************************************
** Form generated from reading UI file 'informationDetailDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATIONDETAILDIALOG_H
#define UI_INFORMATIONDETAILDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InformationDetailDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *mFrameTop;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QLabel *lableposition;
    QLineEdit *lineeditpostion;
    QLabel *labletype;
    QComboBox *comboBoxtype;
    QLabel *labeltime;
    QLineEdit *lineedittime;
    QFrame *mFrameBottom;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *picture;

    void setupUi(QDialog *InformationDetailDialog)
    {
        if (InformationDetailDialog->objectName().isEmpty())
            InformationDetailDialog->setObjectName(QString::fromUtf8("InformationDetailDialog"));
        InformationDetailDialog->resize(498, 507);
        verticalLayout = new QVBoxLayout(InformationDetailDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mFrameTop = new QFrame(InformationDetailDialog);
        mFrameTop->setObjectName(QString::fromUtf8("mFrameTop"));
        mFrameTop->setMaximumSize(QSize(16777215, 118));
        mFrameTop->setFrameShape(QFrame::StyledPanel);
        mFrameTop->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(mFrameTop);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        lableposition = new QLabel(mFrameTop);
        lableposition->setObjectName(QString::fromUtf8("lableposition"));

        gridLayout->addWidget(lableposition, 2, 0, 1, 1);

        lineeditpostion = new QLineEdit(mFrameTop);
        lineeditpostion->setObjectName(QString::fromUtf8("lineeditpostion"));

        gridLayout->addWidget(lineeditpostion, 2, 1, 1, 1);

        labletype = new QLabel(mFrameTop);
        labletype->setObjectName(QString::fromUtf8("labletype"));

        gridLayout->addWidget(labletype, 3, 0, 1, 1);

        comboBoxtype = new QComboBox(mFrameTop);
        comboBoxtype->setObjectName(QString::fromUtf8("comboBoxtype"));

        gridLayout->addWidget(comboBoxtype, 3, 1, 1, 1);

        labeltime = new QLabel(mFrameTop);
        labeltime->setObjectName(QString::fromUtf8("labeltime"));

        gridLayout->addWidget(labeltime, 0, 0, 1, 1);

        lineedittime = new QLineEdit(mFrameTop);
        lineedittime->setObjectName(QString::fromUtf8("lineedittime"));

        gridLayout->addWidget(lineedittime, 0, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);


        verticalLayout->addWidget(mFrameTop);

        mFrameBottom = new QFrame(InformationDetailDialog);
        mFrameBottom->setObjectName(QString::fromUtf8("mFrameBottom"));
        mFrameBottom->setFrameShape(QFrame::StyledPanel);
        mFrameBottom->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(mFrameBottom);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        picture = new QLabel(mFrameBottom);
        picture->setObjectName(QString::fromUtf8("picture"));

        verticalLayout_2->addWidget(picture);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addWidget(mFrameBottom);


        retranslateUi(InformationDetailDialog);

        QMetaObject::connectSlotsByName(InformationDetailDialog);
    } // setupUi

    void retranslateUi(QDialog *InformationDetailDialog)
    {
        InformationDetailDialog->setWindowTitle(QApplication::translate("InformationDetailDialog", "\350\257\246\346\203\205", nullptr));
        lableposition->setText(QApplication::translate("InformationDetailDialog", "\344\275\215\347\275\256\357\274\232", nullptr));
        labletype->setText(QApplication::translate("InformationDetailDialog", "\347\261\273\345\236\213\357\274\232", nullptr));
        labeltime->setText(QApplication::translate("InformationDetailDialog", "\347\224\237\346\210\220\346\227\266\351\227\264\357\274\232", nullptr));
        picture->setText(QApplication::translate("InformationDetailDialog", "\345\233\276\347\211\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InformationDetailDialog: public Ui_InformationDetailDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATIONDETAILDIALOG_H

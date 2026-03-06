/********************************************************************************
** Form generated from reading UI file 'gridDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDDIALOG_H
#define UI_GRIDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GridDialog
{
public:
    QDialogButtonBox *mButtonBox;
    QLineEdit *mLineEditExtent;
    QLabel *label_2;
    QLabel *mLabel1;
    QLineEdit *mLineEditSideLength;
    QLabel *label;
    QComboBox *mComboBoxRoadLayer;
    QPushButton *mButtonDrawOnCanvas;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *mComboBoxRoadTypeField;
    QPushButton *mPushButtonDeleteAllGrid;
    QProgressBar *mProgressBar;
    QLabel *label_5;

    void setupUi(QDialog *GridDialog)
    {
        if (GridDialog->objectName().isEmpty())
            GridDialog->setObjectName(QString::fromUtf8("GridDialog"));
        GridDialog->resize(574, 235);
        mButtonBox = new QDialogButtonBox(GridDialog);
        mButtonBox->setObjectName(QString::fromUtf8("mButtonBox"));
        mButtonBox->setGeometry(QRect(250, 190, 271, 32));
        mButtonBox->setOrientation(Qt::Horizontal);
        mButtonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Ok);
        mLineEditExtent = new QLineEdit(GridDialog);
        mLineEditExtent->setObjectName(QString::fromUtf8("mLineEditExtent"));
        mLineEditExtent->setGeometry(QRect(90, 30, 281, 25));
        label_2 = new QLabel(GridDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 150, 80, 17));
        mLabel1 = new QLabel(GridDialog);
        mLabel1->setObjectName(QString::fromUtf8("mLabel1"));
        mLabel1->setGeometry(QRect(30, 30, 51, 17));
        mLineEditSideLength = new QLineEdit(GridDialog);
        mLineEditSideLength->setObjectName(QString::fromUtf8("mLineEditSideLength"));
        mLineEditSideLength->setGeometry(QRect(110, 150, 50, 25));
        label = new QLabel(GridDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 70, 51, 17));
        mComboBoxRoadLayer = new QComboBox(GridDialog);
        mComboBoxRoadLayer->setObjectName(QString::fromUtf8("mComboBoxRoadLayer"));
        mComboBoxRoadLayer->setGeometry(QRect(90, 70, 281, 25));
        mButtonDrawOnCanvas = new QPushButton(GridDialog);
        mButtonDrawOnCanvas->setObjectName(QString::fromUtf8("mButtonDrawOnCanvas"));
        mButtonDrawOnCanvas->setGeometry(QRect(390, 30, 89, 25));
        label_3 = new QLabel(GridDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(390, 70, 221, 17));
        label_4 = new QLabel(GridDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 110, 41, 17));
        mComboBoxRoadTypeField = new QComboBox(GridDialog);
        mComboBoxRoadTypeField->setObjectName(QString::fromUtf8("mComboBoxRoadTypeField"));
        mComboBoxRoadTypeField->setGeometry(QRect(90, 110, 281, 25));
        mPushButtonDeleteAllGrid = new QPushButton(GridDialog);
        mPushButtonDeleteAllGrid->setObjectName(QString::fromUtf8("mPushButtonDeleteAllGrid"));
        mPushButtonDeleteAllGrid->setGeometry(QRect(30, 190, 131, 25));
        mProgressBar = new QProgressBar(GridDialog);
        mProgressBar->setObjectName(QString::fromUtf8("mProgressBar"));
        mProgressBar->setGeometry(QRect(313, 150, 251, 20));
        mProgressBar->setStyleSheet(QString::fromUtf8("background-color: rgb(114, 159, 207);\n"
"color: rgb(238, 238, 236);"));
        mProgressBar->setValue(5);
        label_5 = new QLabel(GridDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(190, 150, 120, 17));

        retranslateUi(GridDialog);
        QObject::connect(mButtonBox, SIGNAL(accepted()), GridDialog, SLOT(accept()));
        QObject::connect(mButtonBox, SIGNAL(rejected()), GridDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(GridDialog);
    } // setupUi

    void retranslateUi(QDialog *GridDialog)
    {
        GridDialog->setWindowTitle(QApplication::translate("GridDialog", "\346\210\230\345\234\272\345\210\235\345\247\213\350\256\276\347\275\256", nullptr));
        label_2->setText(QApplication::translate("GridDialog", "\347\275\221\346\240\274\350\276\271\351\225\277\357\274\210\347\261\263\357\274\211", nullptr));
        mLabel1->setText(QApplication::translate("GridDialog", "\350\214\203\345\233\264", nullptr));
        label->setText(QApplication::translate("GridDialog", "\345\233\276\345\261\202", nullptr));
        mButtonDrawOnCanvas->setText(QApplication::translate("GridDialog", "\346\210\230\345\234\272\350\214\203\345\233\264", nullptr));
        label_3->setText(QApplication::translate("GridDialog", "\357\274\210\346\263\250\357\274\232\345\233\276\345\261\202\350\246\201\346\234\211\350\247\204\345\256\232\347\232\204\351\201\223\350\267\257\345\255\227\346\256\265\357\274\211", nullptr));
        label_4->setText(QApplication::translate("GridDialog", "\345\255\227\346\256\265", nullptr));
        mPushButtonDeleteAllGrid->setText(QApplication::translate("GridDialog", "\346\270\205\351\231\244\345\267\262\346\234\211\350\256\276\347\275\256", nullptr));
        label_5->setText(QApplication::translate("GridDialog", "\346\255\243\345\234\250\345\244\204\347\220\206\357\274\214\350\257\267\347\250\215\345\220\216...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GridDialog: public Ui_GridDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDDIALOG_H

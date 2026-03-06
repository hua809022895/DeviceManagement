/********************************************************************************
** Form generated from reading UI file 'qDialogTarget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGTARGET_H
#define UI_QDIALOGTARGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QDialogTarget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QComboBox *mComboBoxTaskType;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *mLineEditPosition;
    QLineEdit *mLineEditPlaneID;
    QLineEdit *mLineEditPlaneGroupID;
    QLineEdit *mLineEditPlaneCount;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *mComboTargettype;
    QLineEdit *mLineEditTargetid;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_6;
    QComboBox *mComboBoxStatic;
    QLabel *label_9;
    QLineEdit *mLineEditHeight;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QDialogTarget)
    {
        if (QDialogTarget->objectName().isEmpty())
            QDialogTarget->setObjectName(QString::fromUtf8("QDialogTarget"));
        QDialogTarget->resize(420, 399);
        verticalLayout = new QVBoxLayout(QDialogTarget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mComboBoxTaskType = new QComboBox(QDialogTarget);
        mComboBoxTaskType->setObjectName(QString::fromUtf8("mComboBoxTaskType"));

        gridLayout->addWidget(mComboBoxTaskType, 2, 1, 1, 1);

        label_2 = new QLabel(QDialogTarget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(QDialogTarget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        mLineEditPosition = new QLineEdit(QDialogTarget);
        mLineEditPosition->setObjectName(QString::fromUtf8("mLineEditPosition"));

        gridLayout->addWidget(mLineEditPosition, 7, 1, 1, 1);

        mLineEditPlaneID = new QLineEdit(QDialogTarget);
        mLineEditPlaneID->setObjectName(QString::fromUtf8("mLineEditPlaneID"));

        gridLayout->addWidget(mLineEditPlaneID, 4, 1, 1, 1);

        mLineEditPlaneGroupID = new QLineEdit(QDialogTarget);
        mLineEditPlaneGroupID->setObjectName(QString::fromUtf8("mLineEditPlaneGroupID"));

        gridLayout->addWidget(mLineEditPlaneGroupID, 5, 1, 1, 1);

        mLineEditPlaneCount = new QLineEdit(QDialogTarget);
        mLineEditPlaneCount->setObjectName(QString::fromUtf8("mLineEditPlaneCount"));

        gridLayout->addWidget(mLineEditPlaneCount, 3, 1, 1, 1);

        label_7 = new QLabel(QDialogTarget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        label_4 = new QLabel(QDialogTarget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_5 = new QLabel(QDialogTarget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        mComboTargettype = new QComboBox(QDialogTarget);
        mComboTargettype->setObjectName(QString::fromUtf8("mComboTargettype"));

        gridLayout->addWidget(mComboTargettype, 1, 1, 1, 1);

        mLineEditTargetid = new QLineEdit(QDialogTarget);
        mLineEditTargetid->setObjectName(QString::fromUtf8("mLineEditTargetid"));

        gridLayout->addWidget(mLineEditTargetid, 0, 1, 1, 1);

        label_3 = new QLabel(QDialogTarget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_8 = new QLabel(QDialogTarget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        label_6 = new QLabel(QDialogTarget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        mComboBoxStatic = new QComboBox(QDialogTarget);
        mComboBoxStatic->setObjectName(QString::fromUtf8("mComboBoxStatic"));

        gridLayout->addWidget(mComboBoxStatic, 6, 1, 1, 1);

        label_9 = new QLabel(QDialogTarget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 8, 0, 1, 1);

        mLineEditHeight = new QLineEdit(QDialogTarget);
        mLineEditHeight->setObjectName(QString::fromUtf8("mLineEditHeight"));

        gridLayout->addWidget(mLineEditHeight, 8, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(QDialogTarget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QDialogTarget);
        QObject::connect(buttonBox, SIGNAL(accepted()), QDialogTarget, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QDialogTarget, SLOT(reject()));

        QMetaObject::connectSlotsByName(QDialogTarget);
    } // setupUi

    void retranslateUi(QDialog *QDialogTarget)
    {
        QDialogTarget->setWindowTitle(QApplication::translate("QDialogTarget", "Dialog", nullptr));
        label_2->setText(QApplication::translate("QDialogTarget", "\347\233\256\346\240\207\347\261\273\345\236\213\357\274\232", nullptr));
        label->setText(QApplication::translate("QDialogTarget", "\347\233\256\346\240\207ID\357\274\232", nullptr));
        label_7->setText(QApplication::translate("QDialogTarget", "\347\212\266\346\200\201\357\274\232", nullptr));
        label_4->setText(QApplication::translate("QDialogTarget", "\345\217\202\344\270\216\344\273\273\345\212\241\347\232\204\351\243\236\346\234\272\346\225\260\351\207\217\357\274\232", nullptr));
        label_5->setText(QApplication::translate("QDialogTarget", "\346\211\200\345\261\236\346\210\220\345\221\230ID\357\274\232", nullptr));
        label_3->setText(QApplication::translate("QDialogTarget", "\344\273\273\345\212\241\347\261\273\345\236\213\357\274\232", nullptr));
        label_8->setText(QApplication::translate("QDialogTarget", "\344\275\215\347\275\256", nullptr));
        label_6->setText(QApplication::translate("QDialogTarget", "\346\211\200\345\261\236\346\210\230\346\226\227\347\273\204\357\274\232", nullptr));
        label_9->setText(QApplication::translate("QDialogTarget", "\351\253\230\345\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QDialogTarget: public Ui_QDialogTarget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGTARGET_H

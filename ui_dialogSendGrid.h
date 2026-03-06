/********************************************************************************
** Form generated from reading UI file 'dialogSendGrid.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSENDGRID_H
#define UI_DIALOGSENDGRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DialogSendGrid
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QComboBox *mComboBoxMapSendOption;

    void setupUi(QDialog *DialogSendGrid)
    {
        if (DialogSendGrid->objectName().isEmpty())
            DialogSendGrid->setObjectName(QString::fromUtf8("DialogSendGrid"));
        DialogSendGrid->resize(407, 192);
        buttonBox = new QDialogButtonBox(DialogSendGrid);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 120, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(DialogSendGrid);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 50, 121, 17));
        mComboBoxMapSendOption = new QComboBox(DialogSendGrid);
        mComboBoxMapSendOption->setObjectName(QString::fromUtf8("mComboBoxMapSendOption"));
        mComboBoxMapSendOption->setGeometry(QRect(160, 50, 201, 25));

        retranslateUi(DialogSendGrid);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogSendGrid, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogSendGrid, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogSendGrid);
    } // setupUi

    void retranslateUi(QDialog *DialogSendGrid)
    {
        DialogSendGrid->setWindowTitle(QApplication::translate("DialogSendGrid", "Dialog", nullptr));
        label->setText(QApplication::translate("DialogSendGrid", "\345\234\260\345\233\276\351\207\215\347\275\256\346\226\271\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogSendGrid: public Ui_DialogSendGrid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSENDGRID_H

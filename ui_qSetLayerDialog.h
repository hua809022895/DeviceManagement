/********************************************************************************
** Form generated from reading UI file 'qSetLayerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSETLAYERDIALOG_H
#define UI_QSETLAYERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QSetLayerDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *mGridLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QSetLayerDialog)
    {
        if (QSetLayerDialog->objectName().isEmpty())
            QSetLayerDialog->setObjectName(QString::fromUtf8("QSetLayerDialog"));
        QSetLayerDialog->resize(607, 423);
        verticalLayout = new QVBoxLayout(QSetLayerDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mGridLayout = new QGridLayout();
        mGridLayout->setObjectName(QString::fromUtf8("mGridLayout"));

        verticalLayout->addLayout(mGridLayout);

        buttonBox = new QDialogButtonBox(QSetLayerDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QSetLayerDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), QSetLayerDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QSetLayerDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(QSetLayerDialog);
    } // setupUi

    void retranslateUi(QDialog *QSetLayerDialog)
    {
        QSetLayerDialog->setWindowTitle(QApplication::translate("QSetLayerDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QSetLayerDialog: public Ui_QSetLayerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSETLAYERDIALOG_H

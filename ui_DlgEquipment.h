/********************************************************************************
** Form generated from reading UI file 'DlgEquipment.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGEQUIPMENT_H
#define UI_DLGEQUIPMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DlgEquipment
{
public:
    QVBoxLayout *mainLayout;
    QHBoxLayout *contentLayout;
    QTableWidget *mWidget;
    QGroupBox *groupBox;
    QVBoxLayout *imgLayout;
    QFrame *frame;
    QVBoxLayout *frameLay;
    QLabel *labelImage;
    QHBoxLayout *btnLayout;
    QPushButton *pushAddBtn;
    QPushButton *pushModifyBtn;
    QPushButton *pushDeleteBtn;
    QPushButton *pushAllDelBtn;
    QSpacerItem *btnSpacer;
    QPushButton *okButton;

    void setupUi(QDialog *DlgEquipment)
    {
        if (DlgEquipment->objectName().isEmpty())
            DlgEquipment->setObjectName(QString::fromUtf8("DlgEquipment"));
        DlgEquipment->resize(1200, 600);
        mainLayout = new QVBoxLayout(DlgEquipment);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        contentLayout = new QHBoxLayout();
        contentLayout->setObjectName(QString::fromUtf8("contentLayout"));
        mWidget = new QTableWidget(DlgEquipment);
        mWidget->setObjectName(QString::fromUtf8("mWidget"));
        mWidget->setMinimumWidth(550);

        contentLayout->addWidget(mWidget);

        groupBox = new QGroupBox(DlgEquipment);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(true);
        imgLayout = new QVBoxLayout(groupBox);
        imgLayout->setObjectName(QString::fromUtf8("imgLayout"));
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setMinimumWidth(400);
        frameLay = new QVBoxLayout(frame);
        frameLay->setObjectName(QString::fromUtf8("frameLay"));
        labelImage = new QLabel(frame);
        labelImage->setObjectName(QString::fromUtf8("labelImage"));
        labelImage->setAlignment(Qt::AlignCenter);

        frameLay->addWidget(labelImage);


        imgLayout->addWidget(frame);


        contentLayout->addWidget(groupBox);


        mainLayout->addLayout(contentLayout);

        btnLayout = new QHBoxLayout();
        btnLayout->setObjectName(QString::fromUtf8("btnLayout"));
        pushAddBtn = new QPushButton(DlgEquipment);
        pushAddBtn->setObjectName(QString::fromUtf8("pushAddBtn"));

        btnLayout->addWidget(pushAddBtn);

        pushModifyBtn = new QPushButton(DlgEquipment);
        pushModifyBtn->setObjectName(QString::fromUtf8("pushModifyBtn"));

        btnLayout->addWidget(pushModifyBtn);

        pushDeleteBtn = new QPushButton(DlgEquipment);
        pushDeleteBtn->setObjectName(QString::fromUtf8("pushDeleteBtn"));

        btnLayout->addWidget(pushDeleteBtn);

        pushAllDelBtn = new QPushButton(DlgEquipment);
        pushAllDelBtn->setObjectName(QString::fromUtf8("pushAllDelBtn"));

        btnLayout->addWidget(pushAllDelBtn);

        btnSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        btnLayout->addItem(btnSpacer);

        okButton = new QPushButton(DlgEquipment);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumWidth(100);

        btnLayout->addWidget(okButton);


        mainLayout->addLayout(btnLayout);


        retranslateUi(DlgEquipment);
        QObject::connect(okButton, SIGNAL(clicked()), DlgEquipment, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgEquipment);
    } // setupUi

    void retranslateUi(QDialog *DlgEquipment)
    {
        DlgEquipment->setWindowTitle(QApplication::translate("DlgEquipment", "\350\243\205\345\244\207\345\272\223\347\256\241\347\220\206", nullptr));
        groupBox->setTitle(QApplication::translate("DlgEquipment", "\345\233\276\345\203\217", nullptr));
        labelImage->setText(QApplication::translate("DlgEquipment", "    \345\233\276\345\203\217\345\214\272\345\237\237", nullptr));
        pushAddBtn->setText(QApplication::translate("DlgEquipment", "\346\267\273\345\212\240", nullptr));
        pushModifyBtn->setText(QApplication::translate("DlgEquipment", "\344\277\256\346\224\271", nullptr));
        pushDeleteBtn->setText(QApplication::translate("DlgEquipment", "\345\210\240\351\231\244", nullptr));
        pushAllDelBtn->setText(QApplication::translate("DlgEquipment", "\345\205\250\351\203\250\345\210\240\351\231\244", nullptr));
        okButton->setText(QApplication::translate("DlgEquipment", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgEquipment: public Ui_DlgEquipment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGEQUIPMENT_H

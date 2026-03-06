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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgEquipment
{
public:
    QTableWidget *mWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushAddBtn;
    QPushButton *pushModifyBtn;
    QPushButton *pushDeleteBtn;
    QPushButton *pushAllDelBtn;
    QPushButton *okButton;
    QGroupBox *groupBox;
    QFrame *frame;
    QLabel *labelImage;
    QFrame *frame_2;
    QFrame *frame_3;
    QFrame *frame_4;

    void setupUi(QDialog *DlgEquipment)
    {
        if (DlgEquipment->objectName().isEmpty())
            DlgEquipment->setObjectName(QString::fromUtf8("DlgEquipment"));
        DlgEquipment->resize(1195, 583);
        mWidget = new QTableWidget(DlgEquipment);
        mWidget->setObjectName(QString::fromUtf8("mWidget"));
        mWidget->setGeometry(QRect(0, 0, 651, 521));
        horizontalLayoutWidget = new QWidget(DlgEquipment);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 530, 461, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushAddBtn = new QPushButton(horizontalLayoutWidget);
        pushAddBtn->setObjectName(QString::fromUtf8("pushAddBtn"));

        horizontalLayout->addWidget(pushAddBtn);

        pushModifyBtn = new QPushButton(horizontalLayoutWidget);
        pushModifyBtn->setObjectName(QString::fromUtf8("pushModifyBtn"));

        horizontalLayout->addWidget(pushModifyBtn);

        pushDeleteBtn = new QPushButton(horizontalLayoutWidget);
        pushDeleteBtn->setObjectName(QString::fromUtf8("pushDeleteBtn"));

        horizontalLayout->addWidget(pushDeleteBtn);

        pushAllDelBtn = new QPushButton(horizontalLayoutWidget);
        pushAllDelBtn->setObjectName(QString::fromUtf8("pushAllDelBtn"));

        horizontalLayout->addWidget(pushAllDelBtn);

        okButton = new QPushButton(DlgEquipment);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(1060, 540, 100, 30));
        groupBox = new QGroupBox(DlgEquipment);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(660, 10, 530, 511));
        groupBox->setTabletTracking(true);
        groupBox->setFlat(true);
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 20, 521, 481));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        labelImage = new QLabel(frame);
        labelImage->setObjectName(QString::fromUtf8("labelImage"));
        labelImage->setGeometry(QRect(0, 0, 521, 481));
        frame_2 = new QFrame(groupBox);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(0, 500, 530, 16));
        frame_2->setFrameShape(QFrame::HLine);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(DlgEquipment);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(1180, 16, 16, 500));
        frame_3->setFrameShape(QFrame::VLine);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_4 = new QFrame(DlgEquipment);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(648, 10, 16, 510));
        frame_4->setFrameShape(QFrame::VLine);
        frame_4->setFrameShadow(QFrame::Raised);

        retranslateUi(DlgEquipment);
        QObject::connect(okButton, SIGNAL(clicked()), DlgEquipment, SLOT(accept()));

        QMetaObject::connectSlotsByName(DlgEquipment);
    } // setupUi

    void retranslateUi(QDialog *DlgEquipment)
    {
        DlgEquipment->setWindowTitle(QApplication::translate("DlgEquipment", "\350\243\205\345\244\207\345\272\223\347\256\241\347\220\206", nullptr));
        pushAddBtn->setText(QApplication::translate("DlgEquipment", "\346\267\273\345\212\240", nullptr));
        pushModifyBtn->setText(QApplication::translate("DlgEquipment", "\344\277\256\346\224\271", nullptr));
        pushDeleteBtn->setText(QApplication::translate("DlgEquipment", "\345\210\240\351\231\244", nullptr));
        pushAllDelBtn->setText(QApplication::translate("DlgEquipment", "\345\205\250\351\203\250\345\210\240\351\231\244", nullptr));
        okButton->setText(QApplication::translate("DlgEquipment", "\351\200\200\345\207\272", nullptr));
        groupBox->setTitle(QApplication::translate("DlgEquipment", "\345\233\276\345\203\217", nullptr));
        labelImage->setText(QApplication::translate("DlgEquipment", "    \345\233\276\345\203\217\345\214\272\345\237\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgEquipment: public Ui_DlgEquipment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGEQUIPMENT_H

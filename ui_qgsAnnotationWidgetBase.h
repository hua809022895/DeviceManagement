/********************************************************************************
** Form generated from reading UI file 'qgsAnnotationWidgetBase.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGSANNOTATIONWIDGETBASE_H
#define UI_QGSANNOTATIONWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QgsAnnotationWidgetBase
{
public:
    QGridLayout *gridLayout;
    QLineEdit *mlineEditXY;
    QCheckBox *mCheckBoxFourAngle;
    QLabel *label;
    QCheckBox *mMapPositionFixedCheckBox;
    QLineEdit *mLineEditAngle;
    QLineEdit *lmLineEditScaleY;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QLabel *mMapMarketlabel_5;
    QLineEdit *mLineEditScaleX;
    QLineEdit *mLineEditLeftTop;
    QLineEdit *mLineEditRightTop;
    QLineEdit *mLineEditLeftDown;
    QLineEdit *mLineEditRightDown;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *QgsAnnotationWidgetBase)
    {
        if (QgsAnnotationWidgetBase->objectName().isEmpty())
            QgsAnnotationWidgetBase->setObjectName(QString::fromUtf8("QgsAnnotationWidgetBase"));
        QgsAnnotationWidgetBase->resize(377, 361);
        QgsAnnotationWidgetBase->setWindowTitle(QString::fromUtf8("Form"));
        gridLayout = new QGridLayout(QgsAnnotationWidgetBase);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mlineEditXY = new QLineEdit(QgsAnnotationWidgetBase);
        mlineEditXY->setObjectName(QString::fromUtf8("mlineEditXY"));

        gridLayout->addWidget(mlineEditXY, 1, 0, 1, 2);

        mCheckBoxFourAngle = new QCheckBox(QgsAnnotationWidgetBase);
        mCheckBoxFourAngle->setObjectName(QString::fromUtf8("mCheckBoxFourAngle"));

        gridLayout->addWidget(mCheckBoxFourAngle, 6, 0, 1, 1);

        label = new QLabel(QgsAnnotationWidgetBase);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        mMapPositionFixedCheckBox = new QCheckBox(QgsAnnotationWidgetBase);
        mMapPositionFixedCheckBox->setObjectName(QString::fromUtf8("mMapPositionFixedCheckBox"));

        gridLayout->addWidget(mMapPositionFixedCheckBox, 0, 0, 1, 2);

        mLineEditAngle = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditAngle->setObjectName(QString::fromUtf8("mLineEditAngle"));

        gridLayout->addWidget(mLineEditAngle, 2, 1, 1, 1);

        lmLineEditScaleY = new QLineEdit(QgsAnnotationWidgetBase);
        lmLineEditScaleY->setObjectName(QString::fromUtf8("lmLineEditScaleY"));

        gridLayout->addWidget(lmLineEditScaleY, 5, 1, 1, 1);

        label_2 = new QLabel(QgsAnnotationWidgetBase);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 11, 1, 1, 1);

        mMapMarketlabel_5 = new QLabel(QgsAnnotationWidgetBase);
        mMapMarketlabel_5->setObjectName(QString::fromUtf8("mMapMarketlabel_5"));

        gridLayout->addWidget(mMapMarketlabel_5, 2, 0, 1, 1);

        mLineEditScaleX = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditScaleX->setObjectName(QString::fromUtf8("mLineEditScaleX"));

        gridLayout->addWidget(mLineEditScaleX, 4, 1, 1, 1);

        mLineEditLeftTop = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditLeftTop->setObjectName(QString::fromUtf8("mLineEditLeftTop"));

        gridLayout->addWidget(mLineEditLeftTop, 7, 1, 1, 1);

        mLineEditRightTop = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditRightTop->setObjectName(QString::fromUtf8("mLineEditRightTop"));

        gridLayout->addWidget(mLineEditRightTop, 8, 1, 1, 1);

        mLineEditLeftDown = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditLeftDown->setObjectName(QString::fromUtf8("mLineEditLeftDown"));

        gridLayout->addWidget(mLineEditLeftDown, 9, 1, 1, 1);

        mLineEditRightDown = new QLineEdit(QgsAnnotationWidgetBase);
        mLineEditRightDown->setObjectName(QString::fromUtf8("mLineEditRightDown"));

        gridLayout->addWidget(mLineEditRightDown, 10, 1, 1, 1);

        label_3 = new QLabel(QgsAnnotationWidgetBase);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 7, 0, 1, 1);

        label_4 = new QLabel(QgsAnnotationWidgetBase);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 8, 0, 1, 1);

        label_5 = new QLabel(QgsAnnotationWidgetBase);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 9, 0, 1, 1);

        label_6 = new QLabel(QgsAnnotationWidgetBase);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 10, 0, 1, 1);


        retranslateUi(QgsAnnotationWidgetBase);

        QMetaObject::connectSlotsByName(QgsAnnotationWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *QgsAnnotationWidgetBase)
    {
        mCheckBoxFourAngle->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\233\233\350\247\222\345\233\272\345\256\232\347\273\217\347\272\254\345\272\246", nullptr));
        label->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\256\275\345\272\246\347\274\251\346\224\276\346\257\224\344\276\213", nullptr));
        mMapPositionFixedCheckBox->setText(QApplication::translate("QgsAnnotationWidgetBase", "\344\270\255\345\277\203\345\233\272\345\256\232\347\273\217\347\272\254\345\272\246", nullptr));
        label_2->setText(QApplication::translate("QgsAnnotationWidgetBase", "\351\253\230\345\272\246\347\274\251\346\224\276\346\257\224\344\276\213", nullptr));
        mMapMarketlabel_5->setText(QApplication::translate("QgsAnnotationWidgetBase", "\346\227\213\350\275\254\350\247\222(\345\272\246)", nullptr));
        label_3->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\267\246\344\270\212", nullptr));
        label_4->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\217\263\344\270\212", nullptr));
        label_5->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\267\246\344\270\213", nullptr));
        label_6->setText(QApplication::translate("QgsAnnotationWidgetBase", "\345\217\263\344\270\213", nullptr));
        Q_UNUSED(QgsAnnotationWidgetBase);
    } // retranslateUi

};

namespace Ui {
    class QgsAnnotationWidgetBase: public Ui_QgsAnnotationWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGSANNOTATIONWIDGETBASE_H

/********************************************************************************
** Form generated from reading UI file 'lookDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOOKDIALOG_H
#define UI_LOOKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LookDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *mTabWidget;
    QWidget *tab1;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QFrame *frame2;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_3;
    QGridLayout *gridLayout_2;
    QComboBox *mComboBoxTask;
    QComboBox *mComboBoxPlaneModel;
    QLabel *label_5;
    QLabel *label_7;
    QComboBox *mComboBoxPlaneStatus;
    QLabel *label_10;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_11;
    QLineEdit *mLineEditPlanePosion;
    QPushButton *mActionPushbtnDelCurrent;
    QPushButton *mActionButtonAddCurrent;
    QLineEdit *mLineEditPower;
    QLineEdit *mLineEditgroup;
    QLineEdit *mLineEditPlaneNumber;
    QLabel *label_6;
    QTableWidget *mTableWidgetPicture;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QFrame *frame1;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *mPushButtonLocate;
    QPushButton *mActionButtonDelete;
    QPushButton *mPushButtonDeleteAll;
    QPushButton *mPushButtonExport;
    QPushButton *mPushButtonHistory;
    QPushButton *mActionButtonquery;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_4;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *mLineEditExtend;
    QDateTimeEdit *mDateTimeEditEnd;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label;
    QComboBox *mComboBoxHandletype;
    QDateTimeEdit *mDateTimeEditbegin;
    QComboBox *mComboBoxWeapon;
    QPushButton *mPushButtonExtend;
    QProgressBar *mProgressBar;
    QTableWidget *mTableWidgetHistory;
    QWidget *tab2;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *mPushButtonRefreshPlane;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *mTableWidgetPlane;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LookDialog)
    {
        if (LookDialog->objectName().isEmpty())
            LookDialog->setObjectName(QString::fromUtf8("LookDialog"));
        LookDialog->resize(1409, 656);
        verticalLayout = new QVBoxLayout(LookDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mTabWidget = new QTabWidget(LookDialog);
        mTabWidget->setObjectName(QString::fromUtf8("mTabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        horizontalLayout = new QHBoxLayout(tab1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(tab1);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(8);
        frame2 = new QFrame(splitter);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setFrameShape(QFrame::StyledPanel);
        frame2->setFrameShadow(QFrame::Plain);
        frame2->setLineWidth(0);
        verticalLayout_2 = new QVBoxLayout(frame2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_3 = new QFrame(frame2);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mComboBoxTask = new QComboBox(frame_3);
        mComboBoxTask->setObjectName(QString::fromUtf8("mComboBoxTask"));
        mComboBoxTask->setEditable(true);

        gridLayout_2->addWidget(mComboBoxTask, 0, 14, 1, 1);

        mComboBoxPlaneModel = new QComboBox(frame_3);
        mComboBoxPlaneModel->setObjectName(QString::fromUtf8("mComboBoxPlaneModel"));
        mComboBoxPlaneModel->setEditable(true);

        gridLayout_2->addWidget(mComboBoxPlaneModel, 0, 10, 1, 1);

        label_5 = new QLabel(frame_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_5, 0, 2, 1, 1);

        label_7 = new QLabel(frame_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 0, 4, 1, 1);

        mComboBoxPlaneStatus = new QComboBox(frame_3);
        mComboBoxPlaneStatus->setObjectName(QString::fromUtf8("mComboBoxPlaneStatus"));
        mComboBoxPlaneStatus->setEditable(true);

        gridLayout_2->addWidget(mComboBoxPlaneStatus, 0, 8, 1, 1);

        label_10 = new QLabel(frame_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_10, 0, 15, 1, 1);

        label_8 = new QLabel(frame_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 0, 9, 1, 1);

        label_9 = new QLabel(frame_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 0, 13, 1, 1);

        label_11 = new QLabel(frame_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_11, 0, 18, 1, 1);

        mLineEditPlanePosion = new QLineEdit(frame_3);
        mLineEditPlanePosion->setObjectName(QString::fromUtf8("mLineEditPlanePosion"));
        mLineEditPlanePosion->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(mLineEditPlanePosion, 0, 20, 1, 1);

        mActionPushbtnDelCurrent = new QPushButton(frame_3);
        mActionPushbtnDelCurrent->setObjectName(QString::fromUtf8("mActionPushbtnDelCurrent"));
        mActionPushbtnDelCurrent->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(mActionPushbtnDelCurrent, 5, 20, 1, 1);

        mActionButtonAddCurrent = new QPushButton(frame_3);
        mActionButtonAddCurrent->setObjectName(QString::fromUtf8("mActionButtonAddCurrent"));

        gridLayout_2->addWidget(mActionButtonAddCurrent, 5, 18, 1, 1);

        mLineEditPower = new QLineEdit(frame_3);
        mLineEditPower->setObjectName(QString::fromUtf8("mLineEditPower"));
        mLineEditPower->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(mLineEditPower, 0, 16, 1, 1);

        mLineEditgroup = new QLineEdit(frame_3);
        mLineEditgroup->setObjectName(QString::fromUtf8("mLineEditgroup"));
        mLineEditgroup->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(mLineEditgroup, 0, 3, 1, 1);

        mLineEditPlaneNumber = new QLineEdit(frame_3);
        mLineEditPlaneNumber->setObjectName(QString::fromUtf8("mLineEditPlaneNumber"));
        mLineEditPlaneNumber->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(mLineEditPlaneNumber, 0, 6, 1, 1);

        label_6 = new QLabel(frame_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 0, 7, 1, 1);


        verticalLayout_2->addWidget(frame_3);

        mTableWidgetPicture = new QTableWidget(frame2);
        mTableWidgetPicture->setObjectName(QString::fromUtf8("mTableWidgetPicture"));

        verticalLayout_2->addWidget(mTableWidgetPicture);

        splitter->addWidget(frame2);

        horizontalLayout->addWidget(splitter);

        mTabWidget->addTab(tab1, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        frame1 = new QFrame(tab);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setFrameShape(QFrame::StyledPanel);
        frame1->setFrameShadow(QFrame::Plain);
        frame1->setLineWidth(0);
        verticalLayout_3 = new QVBoxLayout(frame1);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(frame1);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        mPushButtonLocate = new QPushButton(frame);
        mPushButtonLocate->setObjectName(QString::fromUtf8("mPushButtonLocate"));

        horizontalLayout_3->addWidget(mPushButtonLocate);

        mActionButtonDelete = new QPushButton(frame);
        mActionButtonDelete->setObjectName(QString::fromUtf8("mActionButtonDelete"));

        horizontalLayout_3->addWidget(mActionButtonDelete);

        mPushButtonDeleteAll = new QPushButton(frame);
        mPushButtonDeleteAll->setObjectName(QString::fromUtf8("mPushButtonDeleteAll"));

        horizontalLayout_3->addWidget(mPushButtonDeleteAll);

        mPushButtonExport = new QPushButton(frame);
        mPushButtonExport->setObjectName(QString::fromUtf8("mPushButtonExport"));

        horizontalLayout_3->addWidget(mPushButtonExport);

        mPushButtonHistory = new QPushButton(frame);
        mPushButtonHistory->setObjectName(QString::fromUtf8("mPushButtonHistory"));
        mPushButtonHistory->setEnabled(false);

        horizontalLayout_3->addWidget(mPushButtonHistory);

        mActionButtonquery = new QPushButton(frame);
        mActionButtonquery->setObjectName(QString::fromUtf8("mActionButtonquery"));

        horizontalLayout_3->addWidget(mActionButtonquery);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(frame);

        frame_4 = new QFrame(frame1);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMinimumSize(QSize(0, 100));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayoutWidget = new QWidget(frame_4);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(19, -1, 801, 101));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        mLineEditExtend = new QLineEdit(gridLayoutWidget);
        mLineEditExtend->setObjectName(QString::fromUtf8("mLineEditExtend"));

        gridLayout->addWidget(mLineEditExtend, 1, 1, 1, 2);

        mDateTimeEditEnd = new QDateTimeEdit(gridLayoutWidget);
        mDateTimeEditEnd->setObjectName(QString::fromUtf8("mDateTimeEditEnd"));

        gridLayout->addWidget(mDateTimeEditEnd, 0, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 5, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        mComboBoxHandletype = new QComboBox(gridLayoutWidget);
        mComboBoxHandletype->setObjectName(QString::fromUtf8("mComboBoxHandletype"));

        gridLayout->addWidget(mComboBoxHandletype, 0, 4, 1, 1);

        mDateTimeEditbegin = new QDateTimeEdit(gridLayoutWidget);
        mDateTimeEditbegin->setObjectName(QString::fromUtf8("mDateTimeEditbegin"));
        mDateTimeEditbegin->setInputMethodHints(Qt::ImhFormattedNumbersOnly|Qt::ImhPreferNumbers);

        gridLayout->addWidget(mDateTimeEditbegin, 0, 1, 1, 1);

        mComboBoxWeapon = new QComboBox(gridLayoutWidget);
        mComboBoxWeapon->setObjectName(QString::fromUtf8("mComboBoxWeapon"));

        gridLayout->addWidget(mComboBoxWeapon, 0, 6, 1, 1);

        mPushButtonExtend = new QPushButton(gridLayoutWidget);
        mPushButtonExtend->setObjectName(QString::fromUtf8("mPushButtonExtend"));
        mPushButtonExtend->setMaximumSize(QSize(30, 16777215));

        gridLayout->addWidget(mPushButtonExtend, 1, 3, 1, 1);

        mProgressBar = new QProgressBar(gridLayoutWidget);
        mProgressBar->setObjectName(QString::fromUtf8("mProgressBar"));
        mProgressBar->setStyleSheet(QString::fromUtf8("background-color: rgb(114, 159, 207);\n"
"color: rgb(238, 238, 236);"));
        mProgressBar->setValue(5);

        gridLayout->addWidget(mProgressBar, 1, 4, 1, 3);


        verticalLayout_3->addWidget(frame_4);

        mTableWidgetHistory = new QTableWidget(frame1);
        mTableWidgetHistory->setObjectName(QString::fromUtf8("mTableWidgetHistory"));

        verticalLayout_3->addWidget(mTableWidgetHistory);


        horizontalLayout_4->addWidget(frame1);

        mTabWidget->addTab(tab, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        verticalLayout_4 = new QVBoxLayout(tab2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frame_2 = new QFrame(tab2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        mPushButtonRefreshPlane = new QPushButton(frame_2);
        mPushButtonRefreshPlane->setObjectName(QString::fromUtf8("mPushButtonRefreshPlane"));

        horizontalLayout_2->addWidget(mPushButtonRefreshPlane);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_4->addWidget(frame_2);

        mTableWidgetPlane = new QTableWidget(tab2);
        mTableWidgetPlane->setObjectName(QString::fromUtf8("mTableWidgetPlane"));

        verticalLayout_4->addWidget(mTableWidgetPlane);

        mTabWidget->addTab(tab2, QString());

        verticalLayout->addWidget(mTabWidget);

        buttonBox = new QDialogButtonBox(LookDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(LookDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LookDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LookDialog, SLOT(reject()));

        mTabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LookDialog);
    } // setupUi

    void retranslateUi(QDialog *LookDialog)
    {
        LookDialog->setWindowTitle(QApplication::translate("LookDialog", "\346\203\205\346\212\245\346\237\245\350\257\242\345\257\274\345\207\272", nullptr));
        label_5->setText(QApplication::translate("LookDialog", "\351\243\236\346\234\272\347\273\204\345\217\267\357\274\232", nullptr));
        label_7->setText(QApplication::translate("LookDialog", "\351\243\236\346\234\272\347\274\226\345\217\267\357\274\232", nullptr));
        label_10->setText(QApplication::translate("LookDialog", "\347\224\265\351\207\217\357\274\232", nullptr));
        label_8->setText(QApplication::translate("LookDialog", "\351\243\236\346\234\272\346\234\272\345\236\213\357\274\232", nullptr));
        label_9->setText(QApplication::translate("LookDialog", "\344\273\273\345\212\241\357\274\232", nullptr));
        label_11->setText(QApplication::translate("LookDialog", "\344\275\215\347\275\256\357\274\232", nullptr));
        mActionPushbtnDelCurrent->setText(QApplication::translate("LookDialog", "\345\210\240\351\231\244", nullptr));
        mActionButtonAddCurrent->setText(QApplication::translate("LookDialog", "\345\242\236\345\212\240", nullptr));
        label_6->setText(QApplication::translate("LookDialog", "\351\243\236\346\234\272\347\212\266\346\200\201\357\274\232", nullptr));
        mTabWidget->setTabText(mTabWidget->indexOf(tab1), QApplication::translate("LookDialog", "\351\243\236\346\234\272\345\272\223\345\255\230", nullptr));
        mPushButtonLocate->setText(QApplication::translate("LookDialog", "\345\256\232\344\275\215", nullptr));
        mActionButtonDelete->setText(QApplication::translate("LookDialog", "\345\210\240\351\231\244", nullptr));
        mPushButtonDeleteAll->setText(QApplication::translate("LookDialog", "\345\210\240\351\231\244\345\205\250\351\203\250", nullptr));
        mPushButtonExport->setText(QApplication::translate("LookDialog", "\345\257\274\345\207\272", nullptr));
        mPushButtonHistory->setText(QApplication::translate("LookDialog", "\346\237\245\350\257\242\345\205\250\351\203\250", nullptr));
        mActionButtonquery->setText(QApplication::translate("LookDialog", "\346\214\211\346\235\241\344\273\266\346\237\245\350\257\242", nullptr));
        mDateTimeEditEnd->setDisplayFormat(QApplication::translate("LookDialog", "yyyy/M/d hh:mm", nullptr));
        label_3->setText(QApplication::translate("LookDialog", "           \345\244\204\347\220\206\346\226\271\345\274\217\357\274\232", nullptr));
        label_4->setText(QApplication::translate("LookDialog", "     \346\255\246\345\231\250\347\261\273\345\236\213\357\274\232", nullptr));
        label_2->setText(QApplication::translate("LookDialog", "\344\275\215\347\275\256\350\214\203\345\233\264\357\274\232", nullptr));
        label->setText(QApplication::translate("LookDialog", "\350\265\267\346\255\242\346\227\266\351\227\264\357\274\232", nullptr));
        mDateTimeEditbegin->setDisplayFormat(QApplication::translate("LookDialog", "yyyy/M/d hh:mm", nullptr));
        mPushButtonExtend->setText(QApplication::translate("LookDialog", "...", nullptr));
        mTabWidget->setTabText(mTabWidget->indexOf(tab), QApplication::translate("LookDialog", "\345\216\206\345\217\262\346\203\205\346\212\245", nullptr));
        mPushButtonRefreshPlane->setText(QApplication::translate("LookDialog", "\345\210\267\346\226\260\351\243\236\346\234\272", nullptr));
        mTabWidget->setTabText(mTabWidget->indexOf(tab2), QApplication::translate("LookDialog", "\346\255\243\345\234\250\346\211\247\350\241\214\344\273\273\345\212\241\347\232\204\351\243\236\346\234\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LookDialog: public Ui_LookDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOOKDIALOG_H

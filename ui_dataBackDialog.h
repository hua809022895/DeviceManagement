/********************************************************************************
** Form generated from reading UI file 'dataBackDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATABACKDIALOG_H
#define UI_DATABACKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dataBackDlgClass
{
public:
    QVBoxLayout   *verticalLayout;
    QHBoxLayout   *timeLayout;
    QLabel        *label;
    QDateTimeEdit *mDateTimeEditbegin;
    QLabel        *label_3;
    QDateTimeEdit *mDateTimeEditEnd;
    QSpacerItem   *timeSpacer;
    QHBoxLayout   *idLayout;
    QLabel        *label_2;
    QLineEdit     *lineEdit;
    QPushButton   *addBtn;
    QPushButton   *deleteBtn;
    QPushButton   *queryIdBtn;   // query available IDs from DB
    QSpacerItem   *idSpacer;
    QHBoxLayout   *contentLayout;
    QListView     *listView;
    QVBoxLayout   *btnLayout;
    QPushButton   *pushQueryBtn;
    QPushButton   *pushPauseBtn;
    QComboBox     *speedCombo;   // playback speed: 0.5x/1x/2x/5x/10x
    QPushButton   *pushExportBtn;
    QSpacerItem   *btnSpacer;
    QPushButton   *pushExitBtn;

    void setupUi(QDialog *dataBackDlgClass)
    {
        if (dataBackDlgClass->objectName().isEmpty())
            dataBackDlgClass->setObjectName(QString::fromUtf8("dataBackDlgClass"));
        dataBackDlgClass->resize(600, 300);
        dataBackDlgClass->setMinimumSize(QSize(520, 260));
        dataBackDlgClass->setModal(false);

        verticalLayout = new QVBoxLayout(dataBackDlgClass);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        verticalLayout->setSpacing(8);

        // ---- 起止时间行 ----
        timeLayout = new QHBoxLayout();
        timeLayout->setObjectName(QString::fromUtf8("timeLayout"));
        timeLayout->setSpacing(6);

        label = new QLabel(dataBackDlgClass);
        label->setObjectName(QString::fromUtf8("label"));
        timeLayout->addWidget(label);

        mDateTimeEditbegin = new QDateTimeEdit(dataBackDlgClass);
        mDateTimeEditbegin->setObjectName(QString::fromUtf8("mDateTimeEditbegin"));
        mDateTimeEditbegin->setMinimumWidth(150);
        mDateTimeEditbegin->setInputMethodHints(Qt::ImhFormattedNumbersOnly|Qt::ImhPreferNumbers);
        timeLayout->addWidget(mDateTimeEditbegin);

        label_3 = new QLabel(dataBackDlgClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);
        timeLayout->addWidget(label_3);

        mDateTimeEditEnd = new QDateTimeEdit(dataBackDlgClass);
        mDateTimeEditEnd->setObjectName(QString::fromUtf8("mDateTimeEditEnd"));
        mDateTimeEditEnd->setMinimumWidth(150);
        timeLayout->addWidget(mDateTimeEditEnd);

        timeSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        timeLayout->addItem(timeSpacer);

        verticalLayout->addLayout(timeLayout);

        // ---- 飞机ID行 ----
        idLayout = new QHBoxLayout();
        idLayout->setObjectName(QString::fromUtf8("idLayout"));
        idLayout->setSpacing(6);

        label_2 = new QLabel(dataBackDlgClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        idLayout->addWidget(label_2);

        lineEdit = new QLineEdit(dataBackDlgClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumWidth(100);
        idLayout->addWidget(lineEdit);

        addBtn = new QPushButton(dataBackDlgClass);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));
        addBtn->setMinimumWidth(60);
        idLayout->addWidget(addBtn);

        deleteBtn = new QPushButton(dataBackDlgClass);
        deleteBtn->setObjectName(QString::fromUtf8("deleteBtn"));
        deleteBtn->setMinimumWidth(60);
        idLayout->addWidget(deleteBtn);

        queryIdBtn = new QPushButton(dataBackDlgClass);
        queryIdBtn->setObjectName(QString::fromUtf8("queryIdBtn"));
        queryIdBtn->setMinimumWidth(70);
        idLayout->addWidget(queryIdBtn);

        idSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        idLayout->addItem(idSpacer);

        verticalLayout->addLayout(idLayout);

        // ---- 列表 + 右侧按钮 ----
        contentLayout = new QHBoxLayout();
        contentLayout->setObjectName(QString::fromUtf8("contentLayout"));
        contentLayout->setSpacing(8);

        listView = new QListView(dataBackDlgClass);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sp.setHorizontalStretch(1);
        listView->setSizePolicy(sp);
        contentLayout->addWidget(listView);

        btnLayout = new QVBoxLayout();
        btnLayout->setObjectName(QString::fromUtf8("btnLayout"));
        btnLayout->setSpacing(6);

        pushQueryBtn = new QPushButton(dataBackDlgClass);
        pushQueryBtn->setObjectName(QString::fromUtf8("pushQueryBtn"));
        pushQueryBtn->setMinimumWidth(90);
        btnLayout->addWidget(pushQueryBtn);

        pushPauseBtn = new QPushButton(dataBackDlgClass);
        pushPauseBtn->setObjectName(QString::fromUtf8("pushPauseBtn"));
        pushPauseBtn->setMinimumWidth(90);
        btnLayout->addWidget(pushPauseBtn);

        speedCombo = new QComboBox(dataBackDlgClass);
        speedCombo->setObjectName(QString::fromUtf8("speedCombo"));
        speedCombo->setMinimumWidth(90);
        speedCombo->addItem("0.5x");
        speedCombo->addItem("1x");
        speedCombo->addItem("2x");
        speedCombo->addItem("5x");
        speedCombo->addItem("10x");
        speedCombo->setCurrentIndex(1);  // default 1x
        btnLayout->addWidget(speedCombo);

        pushExportBtn = new QPushButton(dataBackDlgClass);
        pushExportBtn->setObjectName(QString::fromUtf8("pushExportBtn"));
        pushExportBtn->setMinimumWidth(90);
        btnLayout->addWidget(pushExportBtn);

        btnSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        btnLayout->addItem(btnSpacer);

        pushExitBtn = new QPushButton(dataBackDlgClass);
        pushExitBtn->setObjectName(QString::fromUtf8("pushExitBtn"));
        pushExitBtn->setMinimumWidth(90);
        btnLayout->addWidget(pushExitBtn);

        contentLayout->addLayout(btnLayout);
        verticalLayout->addLayout(contentLayout);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(mDateTimeEditbegin);
        label_3->setBuddy(mDateTimeEditEnd);
        label_2->setBuddy(lineEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(dataBackDlgClass);
        QMetaObject::connectSlotsByName(dataBackDlgClass);
    } // setupUi

    void retranslateUi(QDialog *dataBackDlgClass)
    {
        // 无人机数据回放
        dataBackDlgClass->setWindowTitle(QApplication::translate("dataBackDlgClass",
            "\346\227\240\344\272\272\346\234\272\346\225\260\346\215\256\345\233\236\346\224\276", nullptr));
        // 起止时间：
        label->setText(QApplication::translate("dataBackDlgClass",
            "\350\265\267\346\255\242\346\227\266\351\227\264\357\274\232", nullptr));
        mDateTimeEditbegin->setDisplayFormat(QApplication::translate("dataBackDlgClass",
            "yyyy/M/d hh:mm", nullptr));
        // 到
        label_3->setText(QApplication::translate("dataBackDlgClass",
            "\345\210\260", nullptr));
        mDateTimeEditEnd->setDisplayFormat(QApplication::translate("dataBackDlgClass",
            "yyyy/M/d hh:mm", nullptr));
        // 飞机ID：
        label_2->setText(QApplication::translate("dataBackDlgClass",
            "\351\243\236\346\234\272ID\357\274\232", nullptr));
        // 添加
        addBtn->setText(QApplication::translate("dataBackDlgClass",
            "\346\267\273\345\212\240", nullptr));
        // 删除
        deleteBtn->setText(QApplication::translate("dataBackDlgClass",
            "\345\210\240\351\231\244", nullptr));
        // 查询ID
        queryIdBtn->setText(QApplication::translate("dataBackDlgClass",
            "\346\237\245\350\257\242ID", nullptr));
        // 检索回放
        pushQueryBtn->setText(QApplication::translate("dataBackDlgClass",
            "\346\243\200\347\264\242\345\233\236\346\224\276", nullptr));
        // 暂停
        pushPauseBtn->setText(QApplication::translate("dataBackDlgClass",
            "\346\232\202\345\201\234", nullptr));
        // 导出日志
        pushExportBtn->setText(QApplication::translate("dataBackDlgClass",
            "\345\257\274\345\207\272\346\227\245\345\277\227", nullptr));
        // 退出
        pushExitBtn->setText(QApplication::translate("dataBackDlgClass",
            "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dataBackDlgClass: public Ui_dataBackDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABACKDIALOG_H

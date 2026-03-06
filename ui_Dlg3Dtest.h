/********************************************************************************
** Form generated from reading UI file 'Dlg3Dtest.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG3DTEST_H
#define UI_DLG3DTEST_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_Dlg3Dtest
{
public:
    QWebEngineView *webEngineView;

    void setupUi(QDialog *Dlg3Dtest)
    {
        if (Dlg3Dtest->objectName().isEmpty())
            Dlg3Dtest->setObjectName(QString::fromUtf8("Dlg3Dtest"));
        Dlg3Dtest->resize(1277, 809);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        Dlg3Dtest->setFont(font);
        Dlg3Dtest->setWindowOpacity(6.000000000000000);
        webEngineView = new QWebEngineView(Dlg3Dtest);
        webEngineView->setObjectName(QString::fromUtf8("webEngineView"));
        webEngineView->setGeometry(QRect(0, 10, 1061, 711));
        webEngineView->setTabletTracking(false);
        webEngineView->setUrl(QUrl(QString::fromUtf8("file:///D:/SRTM/huayin14/index.html")));

        retranslateUi(Dlg3Dtest);

        QMetaObject::connectSlotsByName(Dlg3Dtest);
    } // setupUi

    void retranslateUi(QDialog *Dlg3Dtest)
    {
        Dlg3Dtest->setWindowTitle(QApplication::translate("Dlg3Dtest", "\345\256\236\346\227\266\351\233\206\347\276\244\346\216\250\347\220\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dlg3Dtest: public Ui_Dlg3Dtest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG3DTEST_H

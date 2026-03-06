#include "stdafx.h"
#include "Dlg3Dtest.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>

#include "comm.h"

Dlg3Dtest::Dlg3Dtest(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	//connect(ui.ok_Button, &QAbstractButton::clicked, this, &DlgTuili::on_queryBtn_click);	//筛选命令按钮
	//connect(ui.pushExitBtn, &QAbstractButton::clicked, this, &DlgTuili::on_exitBtn_click);	//定位命令按钮
	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//central->addWidget(ui.mAirTable, 2, 0, 1, 1, Qt::AlignBottom);
	//MainWindow *pDlg = (MainWindow*)this->parentWidget();
	QString mPath = QCoreApplication::applicationDirPath();

	//m_pWebEngineView =new QWebEngineView(this);
	//m_pWebEngineView->setAutoFillBackground(true);

	//m_pWebEngineView->setUrl(QUrl(QString::fromUtf8("file:///D:/SRTM/huayin10/index.html")));
	//m_pWebEngineView->show();

	//QString url=mPath+"/3Dquan/index.html";
	//ui.webEngineView->load(url);
	
	
}

Dlg3Dtest::~Dlg3Dtest()
{}

//窗口大小缩放事件
void Dlg3Dtest::resizeEvent(QResizeEvent* event)
{
	//动态设置窗口最大化
	QRect rc= this->rect();
	//m_pWebEngineView->setGeometry(0, 0, rc.width(), rc.height());
}

#include "stdafx.h"
//#include "App1.h"
#include "mainWindow.h"
#include <QtWidgets/QApplication>
#include <qgsApplication.h>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	// 启用 WebEngine 远程调试（Chrome 打开 http://localhost:9222 查看 JS 控制台）
	qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9222");
	//QApplication a(argc, argv);
	QgsApplication a(argc, argv, true);
	QgsApplication::initQgis();
	//App1 w;
	MainWindow w;
	w.show();
	return a.exec();
}

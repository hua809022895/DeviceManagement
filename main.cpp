#include "stdafx.h"
//#include "App1.h"
#include "mainWindow.h"
#include <QtWidgets/QApplication>
#include <qgsApplication.h>

int main(int argc, char *argv[])
{
	//QApplication a(argc, argv);
	QgsApplication a(argc, argv, true);
	QgsApplication::initQgis();
	//App1 w;
	MainWindow w;
	w.show();
	return a.exec();
}

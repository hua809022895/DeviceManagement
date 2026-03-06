#include "stdafx.h"
#include <QThread>
#include "comm.h"
//#include "./mainwindow.h"
#include <QDateTime>
#include <qdebug.h>
#include "ddsThread.h"
//#include <Python.h>
#include <string>
#include <vector>
#include "global.h"
//#include "moc_ddsThread.cpp"

DDSThreadHandler::DDSThreadHandler()
{
}

void DDSThreadHandler::run()
{
	char *pResult = NULL;
	double xy[8] = { 0 };
	QString mPath= QCoreApplication::applicationDirPath();
	//PyObject* pyParams = NULL;
	while (true)
	{
		if (!g_RecievedDDS.isEmpty())
		{
			BigImg air = { 0 };
			g_ddsMutex.lock();
			air = g_RecievedDDS[0];
			g_RecievedDDS.removeFirst();
			g_ddsMutex.unlock();
			
			qDebug()<< "recevied big picture" << "\n";
			
			//uint quene = QDateTime::currentDateTime().toTime_t();
			int airID= air.airID;
			uint uID = air.bigImgID;
						
			QString file = mPath+ QString("/recjpg/air%1-%2.jpg").arg(airID).arg(uID);
			QFileInfo fileInfo(file);
			if (fileInfo.exists() == false)
			{
				qDebug() << "big picture not exists" << "\n";
				continue;
			}

			//解算左上角坐标
			//pyParams = Py_BuildValue("(iiffffff)", 0, 0, air.lat, air.lng, air.hgt, air.xy[0], air.xy[1], air.xy[2]);//c++类型转python类型			
			//pArg = PyEval_CallObject(pFunc, pyParams);//调用函数
			//PyArg_Parse(pArg, "s", &pResult);//python类型转c++类型

			qDebug() << pResult <<"\n";
			std::vector<std::string> tokens = split(pResult, ",");
			xy[0] = atof(tokens[0].c_str());
			xy[1] = atof(tokens[1].c_str());

			//解算右上角坐标
			//pyParams = Py_BuildValue("(iiffffff)",air.width,0,air.lat, air.lng, air.hgt, air.xy[0], air.xy[1], air.xy[2]);//c++类型转python类型			
			//pArg = PyEval_CallObject(pFunc, pyParams);//调用函数
			//PyArg_Parse(pArg, "s", &pResult);//python类型转c++类型
			tokens = split(pResult, ",");
			xy[2] = atof(tokens[0].c_str());
			xy[3] = atof(tokens[1].c_str());
			
			//解算左下角坐标
			//pyParams = Py_BuildValue("(iiffffff)",0,air.height,air.lat, air.lng, air.hgt, air.xy[0], air.xy[1], air.xy[2]);//c++类型转python类型			
			//pArg = PyEval_CallObject(pFunc, pyParams);//调用函数
			//PyArg_Parse(pArg, "s", &pResult);//python类型转c++类型
			tokens = split(pResult, ",");
			xy[4] = atof(tokens[0].c_str());
			xy[5] = atof(tokens[1].c_str());
			
			//解算右下角坐标
			//pyParams = Py_BuildValue("(iiffffff)",air.width,air.height, air.lat, air.lng, air.hgt, air.xy[0], air.xy[1],air.xy[2]);//c++类型转python类型			
			//pArg = PyEval_CallObject(pFunc, pyParams);//调用函数
			//PyArg_Parse(pArg, "s", &pResult);//python类型转c++类型
			tokens = split(pResult, ",");
			xy[6] = atof(tokens[0].c_str());
			xy[7] = atof(tokens[1].c_str());

			emit receviePictureMessage(xy[0], xy[1], xy[2], xy[3],xy[4],xy[5],xy[6], xy[7], file);
		}
		this->msleep(100);
	}
}





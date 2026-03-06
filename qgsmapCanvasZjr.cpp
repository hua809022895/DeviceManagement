#include "stdafx.h"
#include "qgsmapcanvasZjr.h"
#include <Windows.h>
#include <QMainWindow>
#include "MainWindow.h"

QgsMapCanvasZjr::QgsMapCanvasZjr(QWidget *parent): QgsMapCanvas(parent)
{
}

QgsMapCanvasZjr::~QgsMapCanvasZjr()
{
}

void QgsMapCanvasZjr::mousePressEvent(QMouseEvent * e)
{
	QgsMapCanvas::mousePressEvent(e);
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
	pDlg->mousePressEvent(e);
}

void QgsMapCanvasZjr::mouseReleaseEvent(QMouseEvent * e)
{
	QgsMapCanvas::mouseReleaseEvent(e);
	MainWindow *pDlg=(MainWindow*)this->parentWidget();	
	pDlg->mouseReleaseEvent(e);
}

void QgsMapCanvasZjr::mouseDoubleClickEvent(QMouseEvent * e)
{
	QgsFeature f;
	//if(m_pDlg->m_pSelectLayer && m_pDlg->m_pSelectLayer->selectedFeatureCount()>0)
	{
		//f=m_pDlg->m_pSelectLayer->selectedFeatures()[0];
		//if(m_pDlg->CheckPointDistance(f))
		QgsMapCanvas::mouseDoubleClickEvent(e);
	}
	//int i=WM_USER+1;
	//QCoreApplication::sendEvent(this->parentWidget(), e) ;把鼠标双击事件直接发给父窗口

	//使用用户自定义事件，把鼠标双击事件发给父窗口
//	MyEvent myEvent(MyEventType);
	//QCoreApplication::sendEvent(this->parentWidget()->parentWidget(), &myEvent) ;//sendEvent(&myArmy,&myEvent);
	//WM_MBUTTONDBLCLK
	//QCoreApplication::sendEvent(this->parentWidget(),   (QEvent *)&i);
	//this->parentWidget()->parentWidget()->mouseDoubleClickEvent(e);
}

/*
bool QgsMapCanvasZjr::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) 
	{
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key_Tab) 
		{
			return true;
		}
	} 

	if (event->type() == QEvent::MouseButtonDblClick) 
	{
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key_Tab) 
		{
			return true;
		}
	} 

	if (event->type() ==(WM_USER + 1)) 
	{
		int i=0;
	}
	return QWidget::event(event);
}*/
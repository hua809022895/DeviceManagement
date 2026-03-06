#ifndef QGSMAPCANVASZJR_H
#define QGSMAPCANVASZJR_H

#include <QgsMapCanvas.H>
//#include <QMainWindow>

//class MainWindow;
class QgsMapCanvasZjr : public QgsMapCanvas
{
	Q_OBJECT
public:
	QgsMapCanvasZjr(QWidget *parent);
	~QgsMapCanvasZjr();

	//MainWindow			*m_pDlg;
	void mousePressEvent(QMouseEvent *);  //virtual
	void mouseReleaseEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *event);
	//bool event(QEvent *event);
private:
	
};

#endif // QGSMAPCANVASZJR_H

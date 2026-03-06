/***************************************************************************
    qgsannotationmarkeritem.cpp
 ***************************************************************************/
#include "stdafx.h"
#include "qgsAnnotationMarkerItem2.h"
#include <qgssymbol.h>
#include <qgssymbollayerutils.h>
#include <QPainter>
#include "comm.h"
#include <qmath.h>
#include "mainWindow.h"

//渲染图像
void QgsAnnotationMarkerItem2::render(QgsRenderContext &context)
{
	QPainter *painter = context.painter();

	if (!painter)
		return;

	QgsPointXY leftupCanvasXy = context.mapToPixel().transform(mLeftup);
	QgsPointXY rightupCanvasXy = context.mapToPixel().transform(mRightUp);
	QgsPointXY leftdownCanvasXy = context.mapToPixel().transform(mLeftdown);
	QgsPointXY rightDownCanvasXy = context.mapToPixel().transform(mRightdown);

	if (rightDownCanvasXy.x() < 0 || rightupCanvasXy.x() < 0)
		return;

	//if (rightDownCanvasXy.y() < 0 || leftdownCanvasXy.y() < 0)
		//return;

	int iMapWidth = mMapCanvas->width();
	int iMapHeight = mMapCanvas->height();

	if (leftupCanvasXy.x() > iMapWidth || leftdownCanvasXy.x() > mMapCanvas->width())
		return;

	//if (leftupCanvasXy.y() > iMapHeight || rightupCanvasXy.y() > mMapCanvas->height())
		//return;

	QImage mJpg;
	mJpg.load(mFilePath, "jpg");

	double k1 = double(rightupCanvasXy.y() - rightDownCanvasXy.y()) / double(rightupCanvasXy.x() - rightDownCanvasXy.x());
	double k2 = double(leftupCanvasXy.y() - leftdownCanvasXy.y()) / double(leftupCanvasXy.x() - leftdownCanvasXy.x());

	double kh1 = 0;
	if (abs(rightupCanvasXy.x() - leftupCanvasXy.x()) > 0.1)
	{
		kh1 = double(rightupCanvasXy.y() - leftupCanvasXy.y()) / double(rightupCanvasXy.x() - leftupCanvasXy.x());
	}
	else {
		kh1 = 0;
		return;
	}

	double verifyAngle = 0;	//旋转角度	
	double stride = 2.0;	//图像跨度
	int type = 0;			//要处理的图像类型

	QgsPointXY interPt;//图像中心点坐标
	double interpointX = (k1*rightDownCanvasXy.x() - rightDownCanvasXy.y() - kh1 * leftdownCanvasXy.x() + leftdownCanvasXy.y()) / (k1 - kh1);
	interPt.setX(interpointX);

	double interpointY = k1 * interpointX - k1 * rightDownCanvasXy.x() + rightDownCanvasXy.y();
	interPt.setY(interpointY);

	double h1 = rightupCanvasXy.distance(interPt);
	double h2 = rightDownCanvasXy.distance(interPt);
	double h = rightDownCanvasXy.distance(rightupCanvasXy);

	if (h < h1 || h < h2)
		type = 1;
	else
		type = 0;

	if (0 == type)
	{
		QgsPointXY interPoint;//图像中心点坐标
		double interpointX = (k1*rightDownCanvasXy.x() - rightDownCanvasXy.y() - kh1 * leftdownCanvasXy.x() + leftdownCanvasXy.y()) / (k1 - kh1);
		interPoint.setX(interpointX);
		double interpointY = k1 * interpointX - k1 * rightDownCanvasXy.x() + rightDownCanvasXy.y();
		interPoint.setY(interpointY);

		int h1 = rightupCanvasXy.distance(interPoint);
		int h2 = rightDownCanvasXy.distance(interPoint);
		int h = rightDownCanvasXy.distance(rightupCanvasXy);

		double angle = leftupCanvasXy.azimuth(rightupCanvasXy);//计算此点和其他点之间的方位角
		int segnumber = h1 / stride;
		int allSegnumber = h / stride;

		if (segnumber == 0 || allSegnumber == 0)
			return;

		double downDeltaX = 0;
		double downDeltaY = 0;
		double triangleRightDeltaX = 0;
		double triangleRightDeltaY = 0;
		int triangleSegnumber = 0;

		if (h2 < 3) {
			triangleSegnumber = h2;
			downDeltaX = (rightDownCanvasXy.x() - leftdownCanvasXy.x()) / double(h2);
			downDeltaY = (rightDownCanvasXy.y() - leftdownCanvasXy.y()) / double(h2);
			triangleRightDeltaX = (rightDownCanvasXy.x() - interPoint.x()) / double(h2);
			triangleRightDeltaY = (rightDownCanvasXy.y() - interPoint.y()) / double(h2);
		}
		else  if ((3 <= h2) && (h2 <= 10)) {
			triangleSegnumber = h2 / stride;
			downDeltaX = (rightDownCanvasXy.x() - leftdownCanvasXy.x()) / double(triangleSegnumber);
			downDeltaY = (rightDownCanvasXy.y() - leftdownCanvasXy.y()) / double(triangleSegnumber);
			triangleRightDeltaX = (rightDownCanvasXy.x() - interPoint.x()) / double(triangleSegnumber);
			triangleRightDeltaY = (rightDownCanvasXy.y() - interPoint.y()) / double(triangleSegnumber);
		}
		else {
			triangleSegnumber = h2 / stride;
			downDeltaX = (rightDownCanvasXy.x() - leftdownCanvasXy.x()) / double(triangleSegnumber);
			downDeltaY = (rightDownCanvasXy.y() - leftdownCanvasXy.y()) / double(triangleSegnumber);
			triangleRightDeltaX = (rightDownCanvasXy.x() - interPoint.x()) / double(triangleSegnumber);
			triangleRightDeltaY = (rightDownCanvasXy.y() - interPoint.y()) / double(triangleSegnumber);
		}
		double imageSegmentY = mJpg.height() / allSegnumber;
		double tringleImageSegment = mJpg.height() - imageSegmentY * segnumber;
		double scaleY = h2 / tringleImageSegment;
		double tringelTride = tringleImageSegment / triangleSegnumber;

		QPainter::PixmapFragment *framents = new QPainter::PixmapFragment[segnumber + triangleSegnumber];

		double leftDeltaX = (leftdownCanvasXy.x() - leftupCanvasXy.x()) / double(segnumber);
		double leftDeltaY = (leftdownCanvasXy.y() - leftupCanvasXy.y()) / double(segnumber);
		double rightDeltaX = (interPoint.x() - rightupCanvasXy.x()) / double(segnumber);
		double rightDeltaY = (interPoint.y() - rightupCanvasXy.y()) / double(segnumber);

		for (int i = 0; i < segnumber; i++) {
			QPointF LeftPoint_n0(leftupCanvasXy.x() + i * leftDeltaX, leftupCanvasXy.y() + i * leftDeltaY);
			QPointF RightPoint_n0(rightupCanvasXy.x() + i * rightDeltaX, rightupCanvasXy.y() + i * rightDeltaY);
			QPointF RightPoint_n1(rightupCanvasXy.x() + (i + 1) * rightDeltaX, rightupCanvasXy.y() + (i + 1) * rightDeltaY);

			QVector2D upSegment(RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
			double scaleX = upSegment.length() / mJpg.width();
			double scaleY = double(stride) / imageSegmentY;
			QPointF posCenter((LeftPoint_n0.x() + RightPoint_n1.x()) / 2.0, (LeftPoint_n0.y() + RightPoint_n1.y()) / 2.0);
			QRectF sourceRect(0, imageSegmentY * i, mJpg.width(), imageSegmentY);

			framents[i] = QPainter::PixmapFragment::create(posCenter, sourceRect, scaleX, scaleY, 90 - angle + verifyAngle);
			framents[i].opacity = 0.65;
		}

		for (int i = 0; i < triangleSegnumber; i++)
		{
			QPointF LeftPoint_n0(leftdownCanvasXy.x() + i * downDeltaX, leftdownCanvasXy.y() + i * downDeltaY);
			QPointF RightPoint_n0(interPoint.x() + i * triangleRightDeltaX, interPoint.y() + i * triangleRightDeltaY);
			QPointF RightPoint_n1(interPoint.x() + (i + 1) * triangleRightDeltaX, interPoint.y() + (i + 1) * triangleRightDeltaY);

			QVector2D upSegment(RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
			double scaleX = upSegment.length() / mJpg.width();

			QPointF posCenter((LeftPoint_n0.x() + RightPoint_n1.x()) / 2.0, (LeftPoint_n0.y() + RightPoint_n1.y()) / 2.0);
			QRectF sourceRect(0, imageSegmentY*segnumber + tringelTride * i, mJpg.width(), tringelTride);

			framents[segnumber + i] = QPainter::PixmapFragment::create(posCenter, sourceRect, scaleX, scaleY, 90 - angle + verifyAngle);
			framents[segnumber + i].opacity = 0.65;
		}
		QPixmap pix = QPixmap::fromImage(mJpg);
		painter->drawPixmapFragments(framents, segnumber, pix);
	}
	else
	{	//正常图像
		QgsPointXY interP;//图像中心点坐标
		double interpointX = (kh1*rightDownCanvasXy.x() - k2 * leftdownCanvasXy.x() - rightDownCanvasXy.y() + leftdownCanvasXy.y()) / (kh1 - k2);
		interP.setX(interpointX);
		double interpointY = k2 * interpointX - k2 * leftdownCanvasXy.x() + leftdownCanvasXy.y();
		interP.setY(interpointY);

		double h1 = leftupCanvasXy.distance(interP);
		double h2 = leftdownCanvasXy.distance(interP);
		double h = leftupCanvasXy.distance(leftdownCanvasXy);

		float angle = leftupCanvasXy.azimuth(rightupCanvasXy);//计算此点和其他点之间的方位角
		int segnumber = qFloor(h1 / stride);	//分段数量
		int allSegnumber = qFloor(h / stride);	//全部分段数量

		if (segnumber == 0 || allSegnumber == 0)
			return;

		double downDeltaX = 0;
		double downDeltaY = 0;
		double triangleLeftDeltaX = 0;
		double triangleLeftDeltaY = 0;
		int		triangleSegnumber = 0;//三角形分段编号

		if (h2 < 3) {
			triangleSegnumber = h2;
			downDeltaX = (leftdownCanvasXy.x() - rightDownCanvasXy.x()) / double(h2);
			downDeltaY = (leftdownCanvasXy.y() - rightDownCanvasXy.y()) / double(h2);
			triangleLeftDeltaX = (leftdownCanvasXy.x() - interP.x()) / double(h2);
			triangleLeftDeltaY = (leftdownCanvasXy.y() - interP.y()) / double(h2);
		}
		else  if ((3 <= h2) && (h2 <= 10)) {
			triangleSegnumber = h2 / 2;
			downDeltaX = (leftdownCanvasXy.x() - rightDownCanvasXy.x()) / double(triangleSegnumber);
			downDeltaY = (leftdownCanvasXy.y() - rightDownCanvasXy.y()) / double(triangleSegnumber);
			triangleLeftDeltaX = (leftdownCanvasXy.x() - interP.x()) / double(triangleSegnumber);
			triangleLeftDeltaY = (leftdownCanvasXy.y() - interP.y()) / double(triangleSegnumber);
		}
		else {
			triangleSegnumber = h2 / 2;
			downDeltaX = (leftdownCanvasXy.x() - rightDownCanvasXy.x()) / double(triangleSegnumber);
			downDeltaY = (leftdownCanvasXy.y() - rightDownCanvasXy.y()) / double(triangleSegnumber);
			triangleLeftDeltaX = (leftdownCanvasXy.x() - interP.x()) / double(triangleSegnumber);
			triangleLeftDeltaY = (leftdownCanvasXy.y() - interP.y()) / double(triangleSegnumber);
		}

		MainWindow* pmain = (MainWindow*)gMainWindow;
		int zoomlevev = pmain->m_zoomLevel;
		int ijpgh = mJpg.height();
		double imageSegmentY = double(mJpg.height()) / double(allSegnumber);
		double tringleImageSegment = mJpg.height() - imageSegmentY * segnumber;
		double scaleY = h2 / tringleImageSegment;
		double tringelTride = tringleImageSegment / triangleSegnumber;

		if (tringleImageSegment == 0 || imageSegmentY == 0)
			return;

		QPainter::PixmapFragment *framents = new QPainter::PixmapFragment[segnumber + triangleSegnumber];

		double leftDeltaX = (interP.x() - leftupCanvasXy.x()) / double(segnumber);
		double leftDeltaY = (interP.y() - leftupCanvasXy.y()) / double(segnumber);
		double rightDeltaX = (rightDownCanvasXy.x() - rightupCanvasXy.x()) / double(segnumber);
		double rightDeltaY = (rightDownCanvasXy.y() - rightupCanvasXy.y()) / double(segnumber);

		for (int i = 0; i < segnumber; i++) {
			QPointF LeftPoint_n0(leftupCanvasXy.x() + i * leftDeltaX, leftupCanvasXy.y() + i * leftDeltaY);
			QPointF LeftPoint_n1(leftupCanvasXy.x() + (i + 1) *  leftDeltaX, leftupCanvasXy.y() + (i + 1) * leftDeltaY);
			QPointF RightPoint_n0(rightupCanvasXy.x() + i * rightDeltaX, rightupCanvasXy.y() + i * rightDeltaY);
			QPointF RightPoint_n1(rightupCanvasXy.x() + (i + 1) * rightDeltaX, rightupCanvasXy.y() + (i + 1) * rightDeltaY);

			QVector2D upSegment(RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
			double scaleX = upSegment.length() / mJpg.width();
			double scaleY = stride / imageSegmentY;
			QPointF posCenter((LeftPoint_n0.x() + RightPoint_n1.x()) / 2.0, (LeftPoint_n0.y() + RightPoint_n1.y()) / 2.0);
			QRectF sourceRect(0, imageSegmentY * i, mJpg.width(), imageSegmentY);

			framents[i] = QPainter::PixmapFragment::create(posCenter, sourceRect, scaleX, scaleY, 90 - angle + verifyAngle);
			framents[i].opacity = 0.65; // 设置半透明
		}

		//triangleSegnumber = 0;
		for (int i = 0; i < triangleSegnumber; i++)
		{
			QPointF LeftPoint_n0(interP.x() + i * triangleLeftDeltaX, interP.y() + i * triangleLeftDeltaY);
			QPointF LeftPoint_n1(interP.x() + (i + 1) *  triangleLeftDeltaX, interP.y() + (i + 1) * triangleLeftDeltaY);
			QPointF RightPoint_n0(rightDownCanvasXy.x() + i * downDeltaX, rightDownCanvasXy.y() + i * downDeltaY);
			QPointF RightPoint_n1(rightDownCanvasXy.x() + (i + 1) * downDeltaX, rightDownCanvasXy.y() + (i + 1) * downDeltaY);

			QVector2D upSegment(RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
			double scaleX = upSegment.length() / mJpg.width();

			QPointF posCenter((LeftPoint_n0.x() + RightPoint_n1.x()) / 2.0, (LeftPoint_n0.y() + RightPoint_n1.y()) / 2.0);
			QRectF sourceRect(0, imageSegmentY*segnumber + tringelTride * i, mJpg.width(), tringelTride);

			framents[segnumber +i] = QPainter::PixmapFragment::create(posCenter, sourceRect, scaleX, scaleY, 90 - angle + verifyAngle);
			framents[segnumber +i].opacity = 0.65; // 设置半透明
		}

		QPixmap pix = QPixmap::fromImage(mJpg);
		painter->drawPixmapFragments(framents, segnumber, pix);
	}
}

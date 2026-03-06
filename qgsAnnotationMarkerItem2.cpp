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

QgsAnnotationMarkerItem2::QgsAnnotationMarkerItem2(const QgsPoint &point): QgsAnnotationItem()
    , mPoint(point), mSymbol( qgis::make_unique< QgsMarkerSymbol >())
{
    mIsfirst= new bool(true);
    mIsmove = new bool(false);
}

QgsAnnotationMarkerItem2::~QgsAnnotationMarkerItem2() = default;

QString QgsAnnotationMarkerItem2::type() const
{
	return QStringLiteral("marker");
}

//渲染图像
void QgsAnnotationMarkerItem2::render(QgsRenderContext &context, QgsFeedback *)
{
    QPainter *painter = context.painter();

    if (!painter)
      return;

	render(context);
	return;
	QgsPointXY leftup	= context.mapToPixel().transform(mLeftup);
	QgsPointXY rightup	= context.mapToPixel().transform(mRightUp);
	QgsPointXY leftdown = context.mapToPixel().transform(mLeftdown);
	QgsPointXY rightDown= context.mapToPixel().transform(mRightdown);

//	if (rightDown.x() < 0 || rightup.x() < 0)
	//	return;

	//if (rightDown.y() < 0 || leftdown.y() < 0)
	//	return;

	int iMapWidth = mMapCanvas->width();
	int iMapHeight= mMapCanvas->height();

//	if (leftup.x()>iMapWidth || leftdown.x()>mMapCanvas->width())
//		return;

	//if (leftup.y() > iMapHeight || rightup.y() > mMapCanvas->height())
		//return;

	//render(context);
	//return;
	QImage mJpg;
	mJpg.load(mFilePath, "jpg");

	double verifyAngle = 0;	//旋转角度		
	int w = rightup.x() - leftup.x();
	int h = leftdown.y()- rightup.y();

	//painter->resetTransform();  // 重置平移变换

	//如果无人机机头朝南
	if (mLeftdown.y() > mLeftup.y())
	{		
		QRect rect(rightDown.x(), rightDown.y()+abs(h/5), abs(w)-5, abs(h));
		//QRect rect(leftup.x(), leftup.y(), abs(w), abs(h));
		QPixmap pix = QPixmap::fromImage(mJpg);
		//painter->rotate(20);
		QTransform transform;
		//transform.scale(-1, 1); // 水平翻转
		//transform.scale(1, -1); // 垂直翻转
		transform.scale(-1, -1); // 水平,垂直都翻转
		QPixmap Pixmap = pix.transformed(transform, Qt::SmoothTransformation);
		
		// 旋转图像
		//QPixmap rotatedPixmap = Pixmap.transformed(QTransform().rotate(7));//针对图片big1-001
		QPixmap rotatedPixmap = pix.transformed(QTransform().rotate(-101));//针对图片big1-2189

		painter->drawPixmap(rect, rotatedPixmap);
	}
	else
	{
		//如果无人机机头朝北
//		QRect rect(leftup.x()+w/2, leftup.y(), w, h);// mJpg.width(), mJpg.height());
		QPixmap pix = QPixmap::fromImage(mJpg);
		//painter->rotate(45);

		QPainter::PixmapFragment fragments[1];
		fragments[0] = QPainter::PixmapFragment::create(QPointF(leftup.x() + w /3, leftup.y()+h/6), QRectF(0, 0, w,h)); // 左上角部分
		// 调整片段属性
		fragments[0].rotation =2; // 旋转 45 度
		fragments[0].opacity = 0.85; // 设置半透明
		
		painter->drawPixmapFragments(fragments, 1, pix);
		//painter->drawPixmap(rect, pix);
	}
}

void QgsAnnotationMarkerItem2::renderDirectly(QPainter *painter)
{
}

bool QgsAnnotationMarkerItem2::writeXml(QDomElement &element, QDomDocument &document, const QgsReadWriteContext &context) const
{
	element.setAttribute(QStringLiteral("x"), qgsDoubleToString(mPoint.x()));
	element.setAttribute(QStringLiteral("y"), qgsDoubleToString(mPoint.y()));
	element.setAttribute(QStringLiteral("zIndex"), zIndex());

	element.appendChild(QgsSymbolLayerUtils::saveSymbol(QStringLiteral("markerSymbol"), mSymbol.get(), document, context));
	return true;
}

QgsAnnotationMarkerItem2 *QgsAnnotationMarkerItem2::create()
{
	return new QgsAnnotationMarkerItem2(QgsPoint() );
}

bool QgsAnnotationMarkerItem2::readXml(const QDomElement &element, const QgsReadWriteContext &context )
{
	const double x = element.attribute(QStringLiteral("x")).toDouble();
	const double y = element.attribute(QStringLiteral("y")).toDouble();
	mPoint = QgsPoint(x, y);
	setZIndex(element.attribute(QStringLiteral("zIndex")).toInt());

	const QDomElement symbolElem = element.firstChildElement(QStringLiteral("symbol"));
	if (!symbolElem.isNull())
		setSymbol(QgsSymbolLayerUtils::loadSymbol< QgsMarkerSymbol >(symbolElem, context));

	return true;
}

QgsAnnotationMarkerItem2 *QgsAnnotationMarkerItem2::clone()
{
	std::unique_ptr< QgsAnnotationMarkerItem2 > item = qgis::make_unique< QgsAnnotationMarkerItem2 >(mPoint);
	// item->mJpg = mJpg;
	item->mFilePath = mFilePath;
	item->mMapCanvas = mMapCanvas;
	item->mIsfirst = mIsfirst;
	item->mIsmove = mIsmove;
	item->mLeftup = mLeftup;
	item->mRightUp = mRightUp;
	item->mRightdown = mRightdown;
	item->mLeftdown = mLeftdown;
	item->setSymbol(mSymbol->clone());
	item->setZIndex(zIndex());
	return item.release();
}

QgsRectangle QgsAnnotationMarkerItem2::boundingBox() const
{
    QgsPointXY  pt  = mMapCanvas->getCoordinateTransform()->transform( mPoint.x(), mPoint.y());
    QgsPointXY leftupCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mLeftup );
    QgsPointXY rightupCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mRightUp );
    QgsPointXY leftdownCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mLeftdown );
    QgsPointXY rightDownCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mRightdown );

    double x0 = leftupCanvasXy.x();
    double y0 = leftupCanvasXy.y();

    if (x0 > rightupCanvasXy.x())
    {
        x0 = rightupCanvasXy.x();
    }
    if (x0 > leftdownCanvasXy.x())
    {
        x0 = leftdownCanvasXy.x();
    }
    if (x0 > rightDownCanvasXy.x())
        x0 = rightDownCanvasXy.x();

    if (y0 > rightupCanvasXy.y())
    {
        y0 = rightupCanvasXy.y();
    }
    if (y0 > leftdownCanvasXy.y())
    {
        y0 = leftdownCanvasXy.y();
    }
    if (y0 > rightDownCanvasXy.y())
    {
        y0 = rightDownCanvasXy.y();
    }

    double x1 = leftupCanvasXy.x();
    double y1 = leftupCanvasXy.y();;
    if (x1 < rightupCanvasXy.x())
    {
        x1 = rightupCanvasXy.x();
    }
    if (x1 < leftdownCanvasXy.x())
    {
        x1 = leftdownCanvasXy.x();
    }
    if (x1 < rightDownCanvasXy.x())
    {
        x1 = rightDownCanvasXy.x();
    }

    if (y1 < rightupCanvasXy.y())
    {
        y1 = rightupCanvasXy.y();
    }
    if (y1 < leftdownCanvasXy.y())
    {
        y1 = leftdownCanvasXy.y();
    }
    if (y1 < rightDownCanvasXy.y())
    {
        y1 = rightDownCanvasXy.y();
    }
    QgsRectangle f(x0,y0,x1,y1);
	return f;
}

const QgsMarkerSymbol *QgsAnnotationMarkerItem2::symbol() const
{
	return mSymbol.get();
}

void QgsAnnotationMarkerItem2::setSymbol( QgsMarkerSymbol *symbol)
{
    mSymbol.reset( symbol);
}

int QgsAnnotationMarkerItem2::testFourLineType()
{
    return 0;
}

void QgsAnnotationMarkerItem2::setFilePath(const QString &file)
{
    mFilePath = file;
   // emit appearanceChanged();
}

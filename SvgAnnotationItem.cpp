#include "SvgAnnotationItem.h"

#include "qgsproject.h"
#include <QDomDocument>
#include <QDomElement>

SvgAnnotationImageItem::SvgAnnotationImageItem(QgsMapCanvas* canvas) : QgsAnnotationItem(canvas)
{
	mSvgRenderer = new QSvgRenderer();
}

SvgAnnotationImageItem::~SvgAnnotationImageItem()
{
	mSvgRenderer->deleteLater();
}
void SvgAnnotationImageItem::writeXML(QDomDocument& doc) const
{
	QDomElement documentElem = doc.documentElement();
	if (documentElem.isNull())
	{
		return;
	}
	QDomElement svgAnnotationElem = doc.createElement("SVGAnnotationItem");
	svgAnnotationElem.setAttribute("file", QgsProject::instance()->writePath(mFilePath));
	_writeXML(doc, svgAnnotationElem);
	documentElem.appendChild(svgAnnotationElem);
}

void SvgAnnotationImageItem::readXML(const QDomDocument& doc, const QDomElement& itemElem)
{
	QString filePath = QgsProject::instance()->readPath(itemElem.attribute("file"));
	setFilePath(filePath);
	QDomElement annotationElem = itemElem.firstChildElement("AnnotationItem");
	if (!annotationElem.isNull())
	{
		_readXML(doc, annotationElem);
	}
}
void SvgAnnotationImageItem::paint(QPainter* painter)
{
	if (!painter)
	{
		return;
	}
	//keep width/height ratio of svg
	QRect viewBox = mSvgRenderer->viewBox();
	if (viewBox.isValid())
	{
		double widthRatio = mFrameSize.width() / viewBox.width();
		double heightRatio = mFrameSize.height() / viewBox.height();
		double renderWidth = 0;
		double renderHeight = 0;
		if (widthRatio <= heightRatio)
		{
			renderWidth = mFrameSize.width();
			renderHeight = viewBox.height() * mFrameSize.width() / viewBox.width();
		}
		else
		{
			renderHeight = mFrameSize.height();
			renderWidth = viewBox.width() * mFrameSize.height() / viewBox.height();
		}
		mSvgRenderer->render(painter, QRectF(mOffsetFromReferencePoint.x(), mOffsetFromReferencePoint.y(), renderWidth,
			renderHeight));
	}
	if (isSelected())
	{
		drawSelectionBoxes(painter);
	}
}

void SvgAnnotationImageItem::setFilePath(const QString& file)
{
	mFilePath = file;
	mSvgRenderer->load(mFilePath);
}

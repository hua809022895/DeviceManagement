#include "stdafx.h"
#include "jpgAnnotation.h"
#include <qgsreadwritecontext.h>
#include <qgsproject.h>
#include <qgssymbollayerutils.h>

#include <QDomDocument>
#include <QDomElement>
#include <QPainter>
#include <qmath.h>

JpgAnnotation::JpgAnnotation(QObject *parent ): QgsAnnotation( parent )
{
    width  = 0;
    height = 0;
}

JpgAnnotation *JpgAnnotation::clone() const
{
	std::unique_ptr< JpgAnnotation > c(new JpgAnnotation());
	copyCommonProperties(c.get());
	c->setFilePath(mFilePath);
	return c.release();
}

void JpgAnnotation::writeXml(QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const
{
	QString filePath = QgsSymbolLayerUtils::svgSymbolPathToName(mFilePath, context.pathResolver());
	QDomElement jpgAnnotationElem = doc.createElement(QStringLiteral("JPGAnnotationItem"));
	jpgAnnotationElem.setAttribute(QStringLiteral("file"), filePath);
	_writeXml(jpgAnnotationElem, doc, context);
	elem.appendChild(jpgAnnotationElem);
}

void JpgAnnotation::readXml(const QDomElement &itemElem, const QgsReadWriteContext &context )
{
	QString filePath = QgsSymbolLayerUtils::svgSymbolNameToPath(itemElem.attribute(QStringLiteral("file")), context.pathResolver());
	setFilePath(filePath);
	QDomElement annotationElem = itemElem.firstChildElement(QStringLiteral("AnnotationItem"));
	if (!annotationElem.isNull())
	{
		_readXml(annotationElem, context);
	}
}

void JpgAnnotation::renderAnnotation( QgsRenderContext &context, QSizeF size ) const
{
	QPainter *painter = context.painter();

	/**
	  QgsCoordinateTransform t( mAnnotation->mapPositionCrs(), mMapCanvas->mapSettings().destinationCrs(), QgsProject::instance() );
	  QgsPointXY coord = mAnnotation->mapPosition();
	  try
	  {
		coord = t.transform( coord );
	  }
	  catch ( QgsCsException & )
	  {}
	  setPos( toCanvasCoordinates( coord ) );

	  painter->translate( context.convertToPainterUnits( mOffsetFromReferencePoint.x(), QgsUnitTypes::RenderMillimeters ) + context.convertToPainterUnits( mContentsMargins.left(), QgsUnitTypes::RenderMillimeters ),
						  context.convertToPainterUnits( mOffsetFromReferencePoint.y(), QgsUnitTypes::RenderMillimeters ) + context.convertToPainterUnits( mContentsMargins.top(), QgsUnitTypes::RenderMillimeters ) );

	  */
	if (!painter)
		return;

	// QList<QGraphicsItem *> gItemList = mCanvas->scene()->selectedItems();
	if ((width == 0) || (height == 0))
	{
		//      QgsPointXY minCanvasXy = mMapCanvas->getCoordinateTransform()->transform( minMapXy );
		//      QgsPointXY maxCanvasXy = mMapCanvas->getCoordinateTransform()->transform( maxMapXy );
		//      int w = maxCanvasXy.x() - minCanvasXy.x();
		//      int h = maxCanvasXy.y() - minCanvasXy.y();
		//      QRectF target( -w/2, -h/2, w , h  );
		//        painter->rotate(30);
		//        painter->drawImage( target,mJpg);
	}
	else if (isrender) 
	{
		QRect target(-int(width) / 2, -int(height) / 2, width, height);
		QSqlQuery query(g_sqliteDbase);

		query.exec("select jpg from objectimage where messageId=" + mFilePath);
		if (query.next())
		{
			QByteArray jpg = query.value("jpg").toByteArray();
			QPixmap pixmap;
			pixmap.loadFromData(jpg, "jpg");
			painter->drawPixmap(target, pixmap);
			painter->setPen(QPen(Qt::red, 2));

			QFont font = painter->font();
			font.setPixelSize(24);
			painter->setFont(font);
			painter->drawText(QRect(-int(width) / 2, int(height) / 2 + 2, 300, 30),
				QString("%1,%2").arg(QString::number(this->mapPosition().x(), 'f', 5))
								.arg(QString::number(this->mapPosition().y(), 'f', 5)));
		}
	}
	// painter->drawImage(QRectF( QPoint( minCanvasXy.x(),minCanvasXy.y() ), QPoint( maxCanvasXy.x(),maxCanvasXy.y() )),mJpg );
}

void JpgAnnotation::setFilePath(const QString &file)
{
	mFilePath = file;
	// mJpg.load( mFilePath, "jpg" );
	emit appearanceChanged();
}

void JpgAnnotation::setImageTransparent(QImage &sourceImage, QImage &destintImage, int trans)
{
//    QPainter painter(&destintImage);
//       painter.setCompositionMode(QPainter::CompositionMode_Source);
//       painter.fillRect(resultImage.rect(), Qt::transparent);
//       painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//       //painter.drawImage(0, 0, destinationImage);
//       //painter.setCompositionMode(mode);
//       painter.drawImage(0, 0, sourceImage);
//       //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//       //painter.fillRect(resultImage.rect(), Qt::white);
//    //painter.fillRect(resultImage.rect(), QColor(0, 0, 0, alpha));

//    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
//    painter.fillRect(resultImage.rect(), QColor(0, 0, 0, alpha));
//       painter.end();
}

JpgAnnotation2::JpgAnnotation2(QObject *parent ): QgsAnnotation( parent )
{
    width  = 0;
    height = 0;
}

JpgAnnotation2 *JpgAnnotation2::clone() const
{
  std::unique_ptr< JpgAnnotation2 > c( new JpgAnnotation2() );
  copyCommonProperties( c.get() );
  c->setFilePath( mFilePath );
  return c.release();
}

void JpgAnnotation2::writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const
{
  QString filePath = QgsSymbolLayerUtils::svgSymbolPathToName( mFilePath, context.pathResolver() );
  QDomElement jpgAnnotationElem = doc.createElement( QStringLiteral( "JPGAnnotationItem" ) );
  jpgAnnotationElem.setAttribute( QStringLiteral( "file" ), filePath );
  _writeXml( jpgAnnotationElem, doc, context );
  elem.appendChild( jpgAnnotationElem );
}

void JpgAnnotation2::readXml( const QDomElement &itemElem, const QgsReadWriteContext &context )
{
  QString filePath = QgsSymbolLayerUtils::svgSymbolNameToPath( itemElem.attribute( QStringLiteral( "file" ) ), context.pathResolver() );
  setFilePath( filePath );
  QDomElement annotationElem = itemElem.firstChildElement( QStringLiteral( "AnnotationItem" ) );
  if ( !annotationElem.isNull() )
  {
    _readXml( annotationElem, context );
  }
}

void JpgAnnotation2::renderAnnotation( QgsRenderContext &context, QSizeF size ) const
{
    QPainter *painter = context.painter();

    if ( !painter )
    {
      return;
    }

    QgsPointXY  pt  = mMapCanvas->getCoordinateTransform()->transform( mPoint.x(), mPoint.y());

    QImage mJpg;
    mJpg.load(mFilePath,"png");
    //mSymbol->startRender( context );
    //mSymbol->renderPoint( pt, nullptr, context );
    //mSymbol->stopRender( context );

    painter->save();
    painter->translate( -pt.x(),-pt.y() );

    QgsPointXY leftupCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mLeftup );
    QgsPointXY rightupCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mRightUp );
    QgsPointXY leftdownCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mLeftdown );
    QgsPointXY rightDownCanvasXy = mMapCanvas->getCoordinateTransform()->transform( mRightdown );

    double k1 = double( rightupCanvasXy.y() - rightDownCanvasXy.y() ) / double( rightupCanvasXy.x()-rightDownCanvasXy.x());
    double k2 = double( leftupCanvasXy.y() - leftdownCanvasXy.y() ) / double( leftupCanvasXy.x()-leftdownCanvasXy.x());

    double kh1 = 0;
    if ( abs(rightupCanvasXy.x() - leftupCanvasXy.x()) > 0.1 )
    {
    kh1 = double( rightupCanvasXy.y() - leftupCanvasXy.y() ) / double( rightupCanvasXy.x() - leftupCanvasXy.x() );
    }else{
    kh1 = 0;
    return;
    }

    double stride = 2.0;
    double verifyAngle = 0;
    int type = 0;

    QgsPointXY interpoint;
    double interpointX = ( k1*rightDownCanvasXy.x() - rightDownCanvasXy.y()
                     - kh1* leftdownCanvasXy.x() +leftdownCanvasXy.y() ) / ( k1 - kh1 );
    interpoint.setX( interpointX );
    double interpointY =  k1 * interpointX - k1 * rightDownCanvasXy.x() + rightDownCanvasXy.y();
    interpoint.setY( interpointY );

    //double w =  leftupCanvasXy.distance( rightupCanvasXy );
    double h1 =  rightupCanvasXy.distance( interpoint );
    double h2 =  rightDownCanvasXy.distance( interpoint );
    double h =   rightDownCanvasXy.distance(rightupCanvasXy);

    if ( h < h1 || h <h2 )
    {
    type = 1;
    }else{
    type = 0;
    }


  if ( 0 == type )
  {
      QgsPointXY interpoint;
      double interpointX = ( k1*rightDownCanvasXy.x() - rightDownCanvasXy.y()
                             - kh1* leftdownCanvasXy.x() +leftdownCanvasXy.y() ) / ( k1 - kh1 );
      interpoint.setX( interpointX );
      double interpointY =  k1 * interpointX - k1 * rightDownCanvasXy.x() + rightDownCanvasXy.y();
      interpoint.setY( interpointY );

      //int w =  leftupCanvasXy.distance( rightupCanvasXy );
      int h1 =  rightupCanvasXy.distance( interpoint );
      int h2 =  rightDownCanvasXy.distance( interpoint );
      int h =   rightDownCanvasXy.distance(rightupCanvasXy);

      double angle = leftupCanvasXy.azimuth( rightupCanvasXy );


      int segnumber = h1 / stride;
      int allSegnumber = h / stride;

      if (segnumber==0  || allSegnumber==0 )
      {
          return;
      }

      double downDeltaX = 0;
      double downDeltaY = 0;
      double triangleRightDeltaX = 0;
      double triangleRightDeltaY = 0;

      int triangleSegnumber = 0;
      if ( h2 < 3){
          triangleSegnumber = h2;
          downDeltaX  = ( rightDownCanvasXy.x() - leftdownCanvasXy.x() ) / double(h2);
          downDeltaY  = ( rightDownCanvasXy.y() - leftdownCanvasXy.y() ) / double(h2);
          triangleRightDeltaX = ( rightDownCanvasXy.x()- interpoint.x() ) / double(h2);
          triangleRightDeltaY = ( rightDownCanvasXy.y()- interpoint.y() ) / double(h2);
      }else  if ( (3 <= h2) && (h2 <= 10) ) {
          triangleSegnumber = h2/stride;
          downDeltaX  = ( rightDownCanvasXy.x() - leftdownCanvasXy.x() ) / double(triangleSegnumber);
          downDeltaY  = ( rightDownCanvasXy.y() - leftdownCanvasXy.y() ) / double(triangleSegnumber);
          triangleRightDeltaX = ( rightDownCanvasXy.x()- interpoint.x() ) / double(triangleSegnumber);
          triangleRightDeltaY = ( rightDownCanvasXy.y()- interpoint.y() ) / double(triangleSegnumber);
      }else{
          triangleSegnumber = h2/stride;
          downDeltaX  = ( rightDownCanvasXy.x() - leftdownCanvasXy.x() ) / double(triangleSegnumber);
          downDeltaY = ( rightDownCanvasXy.y() - leftdownCanvasXy.y() )  / double(triangleSegnumber);
          triangleRightDeltaX = ( rightDownCanvasXy.x()- interpoint.x() ) / double(triangleSegnumber);
          triangleRightDeltaY = ( rightDownCanvasXy.y()- interpoint.y() ) / double(triangleSegnumber);
      }
      double imageSegmentY = mJpg.height() / allSegnumber;
      double tringleImageSegment = mJpg.height() - imageSegmentY*segnumber;
      double scaleY =   h2 / tringleImageSegment;
      double tringelTride = tringleImageSegment / triangleSegnumber;

      QPainter::PixmapFragment *framents = new QPainter::PixmapFragment[segnumber+triangleSegnumber];

      double leftDeltaX = ( leftdownCanvasXy.x() - leftupCanvasXy.x() ) / double( segnumber );
      double leftDeltaY = ( leftdownCanvasXy.y() - leftupCanvasXy.y() ) / double( segnumber );
      double rightDeltaX = ( interpoint.x() - rightupCanvasXy.x() ) / double( segnumber );
      double rightDeltaY = ( interpoint.y() - rightupCanvasXy.y() ) / double( segnumber );

      for ( int i = 0; i < segnumber; i ++ ) {
         QPointF LeftPoint_n0( leftupCanvasXy.x() + i *  leftDeltaX, leftupCanvasXy.y() + i * leftDeltaY);
         QPointF LeftPoint_n1( leftupCanvasXy.x() + (i+1) *  leftDeltaX, leftupCanvasXy.y() + (i+1) * leftDeltaY);
         QPointF RightPoint_n0( rightupCanvasXy.x() + i * rightDeltaX, rightupCanvasXy.y() + i * rightDeltaY );
         QPointF RightPoint_n1( rightupCanvasXy.x() + (i+1) * rightDeltaX, rightupCanvasXy.y() + (i+1) * rightDeltaY );

         QVector2D upSegment( RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
         double scaleX = upSegment.length() / mJpg.width() ;
         double scaleY =  double(stride)/imageSegmentY;
         QPointF posCenter( (LeftPoint_n0.x() +RightPoint_n1.x())/2.0 ,(LeftPoint_n0.y() +RightPoint_n1.y())/2.0  );
         QRectF sourceRect(0, imageSegmentY * i, mJpg.width(), imageSegmentY );

         framents[i] = QPainter::PixmapFragment :: create( posCenter,sourceRect, scaleX, scaleY,90 - angle+verifyAngle  );
      }

      for ( int i = 0; i < triangleSegnumber ; i++)
      {
          QPointF LeftPoint_n0( leftdownCanvasXy.x() + i *  downDeltaX, leftdownCanvasXy.y() + i * downDeltaY);
          QPointF LeftPoint_n1( leftdownCanvasXy.x() + (i+1) *  downDeltaX, leftdownCanvasXy.y() + (i+1) * downDeltaX);
          QPointF RightPoint_n0( interpoint.x() + i * triangleRightDeltaX, interpoint.y() + i * triangleRightDeltaY );
          QPointF RightPoint_n1( interpoint.x() + (i+1) * triangleRightDeltaX, interpoint.y() + (i+1) * triangleRightDeltaY );

          QVector2D upSegment( RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
          double scaleX = upSegment.length() / mJpg.width() ;

          QPointF posCenter( ( LeftPoint_n0.x() + RightPoint_n1.x() ) / 2.0, ( LeftPoint_n0.y() + RightPoint_n1.y() ) / 2.0 );
          QRectF sourceRect(0, imageSegmentY*segnumber + tringelTride * i, mJpg.width(), tringelTride );

          framents[segnumber+i] = QPainter::PixmapFragment :: create( posCenter,sourceRect, scaleX, scaleY, 90 - angle+verifyAngle );
      }
      QPixmap pix = QPixmap::fromImage(mJpg);
      painter->drawPixmapFragments( framents,segnumber,pix);
  }
  else
  {
      QgsPointXY interpoint;
      double interpointX = (kh1*rightDownCanvasXy.x()-k2*leftdownCanvasXy.x() -rightDownCanvasXy.y()+leftdownCanvasXy.y())/(kh1-k2);
      interpoint.setX( interpointX );
      double interpointY = k2*interpointX -k2*leftdownCanvasXy.x() + leftdownCanvasXy.y();
      interpoint.setY(interpointY);
      //double w =  leftupCanvasXy.distance( rightupCanvasXy );
      double h1 =  leftupCanvasXy.distance( interpoint );
      double h2 =  leftdownCanvasXy.distance( interpoint );
      double h =   leftupCanvasXy.distance(leftdownCanvasXy);
      float angle = leftupCanvasXy.azimuth( rightupCanvasXy );
      int segnumber = qFloor(h1 / stride);
      int allSegnumber = qFloor(h / stride);

      if ( segnumber == 0  || allSegnumber == 0 )
      {
          return;
      }

      double downDeltaX = 0;
      double downDeltaY = 0;
      double triangleLeftDeltaX = 0;
      double triangleLeftDeltaY = 0;

      int triangleSegnumber = 0;
      if ( h2 < 3){
          triangleSegnumber = h2;
          downDeltaX  = ( leftdownCanvasXy.x() - rightDownCanvasXy.x() ) / double(h2);
          downDeltaY  = ( leftdownCanvasXy.y() - rightDownCanvasXy.y() ) / double(h2);
          triangleLeftDeltaX = ( leftdownCanvasXy.x()- interpoint.x() ) / double(h2);
          triangleLeftDeltaY = ( leftdownCanvasXy.y()- interpoint.y() ) / double(h2);
      }else  if ( (3 <= h2) && (h2 <= 10) ) {
          triangleSegnumber = h2/2;
          downDeltaX  = ( leftdownCanvasXy.x() - rightDownCanvasXy.x() ) / double(triangleSegnumber);
          downDeltaY  = ( leftdownCanvasXy.y() - rightDownCanvasXy.y() ) / double(triangleSegnumber);
          triangleLeftDeltaX = ( leftdownCanvasXy.x()- interpoint.x() ) / double(triangleSegnumber);
                triangleLeftDeltaY = ( leftdownCanvasXy.y()- interpoint.y() ) / double(triangleSegnumber);
      }else{
          triangleSegnumber = h2/2;
          downDeltaX  = ( leftdownCanvasXy.x() - rightDownCanvasXy.x() ) / double(triangleSegnumber);
          downDeltaY = ( leftdownCanvasXy.y() - rightDownCanvasXy.y()  )  / double(triangleSegnumber);
          triangleLeftDeltaX = ( leftdownCanvasXy.x()- interpoint.x() ) / double(triangleSegnumber);
          triangleLeftDeltaY = ( leftdownCanvasXy.y()- interpoint.y() ) / double(triangleSegnumber);
      }
      double imageSegmentY = double(mJpg.height()) /double(allSegnumber);
      double tringleImageSegment = mJpg.height() - imageSegmentY*segnumber;
      double scaleY =   h2 / tringleImageSegment;
      double tringelTride = tringleImageSegment / triangleSegnumber;

      QPainter::PixmapFragment *framents = new QPainter::PixmapFragment[segnumber+triangleSegnumber];

      double leftDeltaX = ( interpoint.x() - leftupCanvasXy.x() ) / double( segnumber );
      double leftDeltaY = ( interpoint.y()  - leftupCanvasXy.y() ) / double( segnumber );
      double rightDeltaX =  ( rightDownCanvasXy.x() - rightupCanvasXy.x() ) / double( segnumber );
      double rightDeltaY = ( rightDownCanvasXy.y() - rightupCanvasXy.y() ) / double( segnumber );
   
      for ( int i = 0; i < segnumber; i ++ ) {
         QPointF LeftPoint_n0( leftupCanvasXy.x() + i *  leftDeltaX, leftupCanvasXy.y() + i * leftDeltaY);
         QPointF LeftPoint_n1( leftupCanvasXy.x() + (i+1) *  leftDeltaX, leftupCanvasXy.y() + (i+1) * leftDeltaY);
         QPointF RightPoint_n0( rightupCanvasXy.x() + i * rightDeltaX, rightupCanvasXy.y() + i * rightDeltaY );
         QPointF RightPoint_n1( rightupCanvasXy.x() + (i+1) * rightDeltaX, rightupCanvasXy.y() + (i+1) * rightDeltaY );

         QVector2D upSegment( RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
         double scaleX = upSegment.length() / mJpg.width() ;
         double scaleY =  stride / imageSegmentY;
         QPointF posCenter( ( LeftPoint_n0.x() + RightPoint_n1.x() ) / 2.0, ( LeftPoint_n0.y() + RightPoint_n1.y() ) / 2.0 );
         QRectF sourceRect(0, imageSegmentY * i, mJpg.width(), imageSegmentY );

         framents[i] = QPainter::PixmapFragment :: create( posCenter,sourceRect, scaleX, scaleY, 90 - angle+verifyAngle);
      }

      for ( int i = 0; i < triangleSegnumber ; i++)
      {
          QPointF LeftPoint_n0( interpoint.x() + i *  triangleLeftDeltaX, interpoint.y() + i * triangleLeftDeltaY);
          QPointF LeftPoint_n1( interpoint.x() + (i+1) *  triangleLeftDeltaX, interpoint.y() + (i+1) * triangleLeftDeltaY);
          QPointF RightPoint_n0( rightDownCanvasXy.x() + i * downDeltaX, rightDownCanvasXy.y() + i * downDeltaY );
          QPointF RightPoint_n1( rightDownCanvasXy.x() + (i+1) * downDeltaX, rightDownCanvasXy.y() + (i+1) * downDeltaY );

          QVector2D upSegment( RightPoint_n0.x() - LeftPoint_n0.x(), RightPoint_n0.y() - LeftPoint_n0.y());
          double scaleX = upSegment.length() / mJpg.width() ;

          QPointF posCenter( ( LeftPoint_n0.x() + RightPoint_n1.x() ) / 2.0, ( LeftPoint_n0.y() + RightPoint_n1.y() ) / 2.0 );
          QRectF sourceRect(0, imageSegmentY*segnumber + tringelTride * i, mJpg.width(), tringelTride );

          framents[segnumber+i] = QPainter::PixmapFragment :: create( posCenter,sourceRect, scaleX, scaleY, 90 - angle+verifyAngle );
      }
      QPixmap pix = QPixmap::fromImage(mJpg);
      painter->drawPixmapFragments( framents,segnumber,pix);
  }
   painter->restore();
}

void JpgAnnotation2::setFilePath( const QString &file )
{
  mFilePath = file;
 // mJpg.load( mFilePath, "jpg" );
  emit appearanceChanged();
}

QRectF JpgAnnotation2::getBound()
{
//    painter->save();
//    painter->translate( -pt.x(),-pt.y() );
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
    {
        x0 = rightDownCanvasXy.x();
    }


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

    QRectF f(x0-pt.x(),y0-pt.y(),x1-x0,y1-y0);
   // painter->restore();
    return f;

}

void JpgAnnotation2::setImageTransparent(QImage &sourceImage, QImage &destintImage, int trans)
{
//    QPainter painter(&destintImage);
//       painter.setCompositionMode(QPainter::CompositionMode_Source);
//       painter.fillRect(resultImage.rect(), Qt::transparent);
//       painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//       //painter.drawImage(0, 0, destinationImage);
//       //painter.setCompositionMode(mode);
//       painter.drawImage(0, 0, sourceImage);
//       //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//       //painter.fillRect(resultImage.rect(), Qt::white);
//    //painter.fillRect(resultImage.rect(), QColor(0, 0, 0, alpha));

//    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
//    painter.fillRect(resultImage.rect(), QColor(0, 0, 0, alpha));

//       painter.end();
}



#include "stdafx.h"
#include "modifyBoundance.h"
#include "qgsMapToolSelectUtils.h"

QMutex ModifyBoundance::mutexMessages;
QMutex ModifyBoundance::mutexModify;
QVector< QgsPointXY > ModifyBoundance::messages;

ModifyBoundance::ModifyBoundance(QgsVectorLayer* gridLayer,  QgsMapCanvas* mapCavas)
{
    mGridLayer    = gridLayer;
    mQgsMapCanvas = mapCavas;
}

void ModifyBoundance::handlePlaneMessage(const QgsPointXY &qgsPoint)
{
    ModifyBoundance::mutexMessages.lock();
    ModifyBoundance::messages.push_front( qgsPoint );
    ModifyBoundance::mutexMessages.unlock();
}

//mavlink协议处理
void ModifyBoundance::run()
{
    QDateTime   lastTimeBundance = QDateTime::currentDateTime();
    QDateTime	currentTime = QDateTime::currentDateTime();

    int T = 6;
	mQgsMapCanvas->setCurrentLayer(mGridLayer);
	mGridLayer->startEditing();

    while ( true )
    {
		ModifyBoundance::mutexModify.lock();
		QDateTime currentTime = QDateTime::currentDateTime();
		if (currentTime > lastTimeBundance.addSecs(T))
		{
			QgsFeatureIterator featureit = mGridLayer->getFeatures();
			QgsFeature feature;

			while (featureit.nextFeature(feature))
			{
				double q = feature.attribute("dundance1").toDouble();
				double r = feature.attribute("increase").toDouble();
				double k = feature.attribute("capacity").toDouble();
				if (k > 0.01) //k涓嶄负0
				{
					q = q + r * 0.001 * (1.0 - q / k) * double(T);
					if (q <= 1.0)
					{

						feature.setAttribute("dundance1", q);
						mGridLayer->updateFeature(feature);
					}
				}
			}
			lastTimeBundance = currentTime;
			mGridLayer->triggerRepaint(true);
		}

		int count = ModifyBoundance::messages.count();
		if (count > 0)
		{
			QgsPointXY xy;

			if (!ModifyBoundance::messages.empty())
			{
				xy = ModifyBoundance::messages.last();
				ModifyBoundance::messages.pop_back();
			}

			if (!xy.isEmpty())
			{
				handlePlaneZeroMessage(xy);
			}
		}
		else {

		}
		ModifyBoundance::mutexModify.unlock();
		this->msleep(10);
    }
}

void ModifyBoundance::handlePlaneZeroMessage(const QgsPointXY &qgsPoint)
{
    double proportion  = 111319.5;//绫冲拰绾害杞崲

//    int counter = mGridLayer->featureCount();
//    QgsFeature lefttop = mGridLayer->getFeature( QgsFeatureId( 0 ) );
//    QgsFeature rightbottom = mGridLayer->getFeature( QgsFeatureId( counter -1 ) );

    QgsFeatureIterator featureit = mGridLayer->getFeatures();
    QgsFeature feature;
    double y0 = 0.0;
    int posY = 0;

    while ( featureit.nextFeature( feature ) )
    {
        if ( 0 == posY )
        {
            y0 = feature.attribute( "centery" ).toDouble();
        }
        posY++;
        double r = feature.attribute( "increase" ).toDouble();
        if ( r > 0.01 )
        {
            double  x = feature.attribute( "centerx" ).toDouble();
            double  y = feature.attribute( "centery" ).toDouble();
            double  width = feature.attribute( "width" ).toDouble();
            double  height = feature.attribute( "height" ).toDouble();
            QgsPoint centerPoint( x,y);
            QgsRectangle centerSizeRect = QgsRectangle::fromCenterAndSize( centerPoint, width / proportion / cos( y0 ), height / proportion );
            if ( centerSizeRect.contains( qgsPoint ) )
            {
                mQgsMapCanvas->setCurrentLayer( mGridLayer );
                mGridLayer->startEditing();
                feature.setAttribute( "dundance1", 0.0 );
                mGridLayer->updateFeature( feature );

                break;
            }
        }
    }
   // mGridLayer->triggerRepaint();
}


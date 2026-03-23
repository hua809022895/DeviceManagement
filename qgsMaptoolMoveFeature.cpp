/***************************************************************************
    qgsmaptoolmovefeature.cpp  -  map tool for translating feat
 ***************************************************************************/
#include "stdafx.h"
//#include "qgisapp.h"
#include "qgsAdvanceddigitizingdockwidget.h"
#include "qgsFeatureiterator.h"
#include "qgsGeometry.h"
#include "qgsLogger.h"
#include "qgsMapCanvas.h"
#include "qgsMaptoolMoveFeature.h"
#include "qgsRubberband.h"
#include "qgsTolerance.h"
#include "qgsVectorlayer.h"
#include "qgsVectorlayertools.h"
#include "qgsSnapindicator.h"
#include "qgsMapmouseevent.h"

#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <limits>
#include "comm.h"
#include "mainWindow.h"

QgsMapToolMoveFeature::QgsMapToolMoveFeature( QgsMapCanvas *canvas, MoveMode mode, QgsAdvancedDigitizingDockWidget *cad)
  : QgsMapToolAdvancedDigitizing( canvas, cad)
  , mSnapIndicator( qgis::make_unique< QgsSnapIndicator>(canvas)), mMode(mode)
{
	mToolName = tr( "Move feature" );
}

QgsMapToolMoveFeature::~QgsMapToolMoveFeature()
{
  delete mRubberBand;
}

void QgsMapToolMoveFeature::cadCanvasMoveEvent( QgsMapMouseEvent *e )
{
  if ( mRubberBand )
  {
    QgsPointXY pointCanvasCoords = e->mapPoint();
    double offsetX = pointCanvasCoords.x() - mStartPointMapCoords.x();
    double offsetY = pointCanvasCoords.y() - mStartPointMapCoords.y();
    mRubberBand->setTranslationOffset( offsetX, offsetY );
    mRubberBand->updatePosition();
    mRubberBand->update();
    mSnapIndicator->setMatch( e->mapPointMatch() );
  }
  else
  {
    mSnapIndicator->setMatch( e->mapPointMatch() );
  }
}

void QgsMapToolMoveFeature::cadCanvasReleaseEvent( QgsMapMouseEvent *e )
{
	QgsVectorLayer *vlayer = currentVectorLayer();
	if (!vlayer || !vlayer->isEditable())
	{
		delete mRubberBand;
		mRubberBand = nullptr;
		mSnapIndicator->setMatch(QgsPointLocator::Match());
		cadDockWidget()->clear();
		notifyNotEditableLayer();
		return;
	}

	if (!mRubberBand)
	{
		//find first geometry under mouse cursor and store iterator to it
		QgsPointXY layerCoords = toLayerCoordinates(vlayer, e->mapPoint());
		double searchRadius = QgsTolerance::vertexSearchRadius(mCanvas->currentLayer(), mCanvas->mapSettings());
		QgsRectangle selectRect(layerCoords.x() - searchRadius, layerCoords.y() - searchRadius,
			layerCoords.x() + searchRadius, layerCoords.y() + searchRadius);

		if (vlayer->selectedFeatureCount() == 0)
		{
			QgsFeatureIterator fit = vlayer->getFeatures(QgsFeatureRequest().setFilterRect(selectRect).setNoAttributes());

			//find the closest feature
			QgsGeometry pointGeometry = QgsGeometry::fromPointXY(layerCoords);
			if (pointGeometry.isNull())
			{
				cadDockWidget()->clear();
				return;
			}

			double minDistance = std::numeric_limits<double>::max();

			QgsFeature cf;
			QgsFeature f;
			while (fit.nextFeature(f))
			{
				if (f.hasGeometry())
				{
					double currentDistance = pointGeometry.distance(f.geometry());
					if (currentDistance < minDistance)
					{
						minDistance = currentDistance;
						cf = f;
					}
				}
			}

			if (minDistance == std::numeric_limits<double>::max())
			{
				cadDockWidget()->clear();
				return;
			}

			mMovedFeatures.clear();
			mMovedFeatures << cf.id(); //todo: take the closest feature, not the first one...

			mRubberBand = createRubberBand(vlayer->geometryType());
			mRubberBand->setToGeometry(cf.geometry(), vlayer);
		}
		else
		{
			mMovedFeatures = vlayer->selectedFeatureIds();

			mRubberBand = createRubberBand(vlayer->geometryType());
			QgsFeature feat;
			QgsFeatureIterator it = vlayer->getSelectedFeatures(QgsFeatureRequest().setNoAttributes());

			bool allFeaturesInView = true;
			QgsRectangle viewRect = mCanvas->mapSettings().mapToLayerCoordinates(vlayer, mCanvas->extent());

			while (it.nextFeature(feat))
			{
				mRubberBand->addGeometry(feat.geometry(), vlayer);

				if (allFeaturesInView && !viewRect.intersects(feat.geometry().boundingBox()))
					allFeaturesInView = false;
			}

			if (!allFeaturesInView)
			{
				// for extra safety to make sure we are not modifying geometries by accident

				int res = QMessageBox::warning(mCanvas, tr("Move features"),
					tr("Some of the selected features are outside of the current map view. Would you still like to continue?"),
					QMessageBox::Yes | QMessageBox::No);
				if (res != QMessageBox::Yes)
				{
					mMovedFeatures.clear();
					delete mRubberBand;
					mRubberBand = nullptr;
					mSnapIndicator->setMatch(QgsPointLocator::Match());
					return;
				}
			}
		}

		mStartPointMapCoords = e->mapPoint();
		mRubberBand->show();
	}
	else
	{
		// copy and move mode
		if (e->button() != Qt::LeftButton)
		{
			cadDockWidget()->clear();
			delete mRubberBand;
			mRubberBand = nullptr;
			mSnapIndicator->setMatch(QgsPointLocator::Match());
			return;
		}

		QgsPointXY startPointLayerCoords = toLayerCoordinates((QgsMapLayer *)vlayer, mStartPointMapCoords);
		QgsPointXY stopPointLayerCoords = toLayerCoordinates((QgsMapLayer *)vlayer, e->mapPoint());

		double dx = stopPointLayerCoords.x() - startPointLayerCoords.x();
		double dy = stopPointLayerCoords.y() - startPointLayerCoords.y();

		vlayer->beginEditCommand(mMode == Move ? tr("Feature moved") : tr("Feature copied and moved"));

		switch (mMode)
		{
		case Move:
			for (QgsFeatureId id : qgis::as_const(mMovedFeatures))
			{
				// snap point feature to exact drop position
				bool pointSnapped = false;
				if (vlayer->geometryType() == QgsWkbTypes::PointGeometry && mMovedFeatures.size() == 1)
				{
					QgsGeometry exactPt = QgsGeometry::fromPointXY(stopPointLayerCoords);
					pointSnapped = vlayer->changeGeometry(id, exactPt);
				}
				if (!pointSnapped)
					vlayer->translateFeature(id, dx, dy);

				if (QgsProject::instance()->topologicalEditing())
				{
					if (mSnapIndicator && (mSnapIndicator->match().layer() != nullptr))
					{
						mSnapIndicator->match().layer()->addTopologicalPoints(vlayer->getGeometry(id));
					}
					vlayer->addTopologicalPoints(vlayer->getGeometry(id));
				}
			}
			delete mRubberBand;
			mRubberBand = nullptr;
			mSnapIndicator->setMatch(QgsPointLocator::Match());
			cadDockWidget()->clear();
			break;
		case CopyMove:
			QgsFeatureRequest request;
			request.setFilterFids(mMovedFeatures);
			QString *errorMsg = new QString();
			if (!((MainWindow*)gMainWindow)->mVectorLayerTools->copyMoveFeatures(vlayer, request, dx, dy, errorMsg, QgsProject::instance()->topologicalEditing(), mSnapIndicator->match().layer()))
			{
				emit messageEmitted(*errorMsg, Qgis::Critical);
				delete mRubberBand;
				mRubberBand = nullptr;
				mSnapIndicator->setMatch(QgsPointLocator::Match());
			}
			break;
		}

		vlayer->endEditCommand();
		vlayer->triggerRepaint();

		// refresh radar labels after move
		QTimer::singleShot(100, [](){ ((MainWindow*)gMainWindow)->ShowRadarTip(); });

		m_stopPoint = stopPointLayerCoords;
	}
}

void QgsMapToolMoveFeature::deactivate()
{
	//delete rubber band
	delete mRubberBand;
	mRubberBand = nullptr;
	mSnapIndicator->setMatch(QgsPointLocator::Match());

	QgsMapToolAdvancedDigitizing::deactivate();
}

void QgsMapToolMoveFeature::keyReleaseEvent( QKeyEvent *e )
{
	if (mRubberBand && e->key() == Qt::Key_Escape)
	{
		cadDockWidget()->clear();
		delete mRubberBand;
		mRubberBand = nullptr;
		mSnapIndicator->setMatch(QgsPointLocator::Match());
	}
}

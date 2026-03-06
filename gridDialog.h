/***************************************************************************
    griddialog.h  -  初始化情报丰度
    ---------------------
    begin                : October 2021
    writer               : writed by suzhisheng
    email                : 656282026@qq.com
 ***************************************************************************/
#ifndef GRIDDIALOG_H
#define GRIDDIALOG_H

#include <QDialog>
#include <qgsmapcanvas.h>
#include <qgsmaptoolextent.h>
#include <qgsfeaturesink.h>
#include "comm.h"

namespace Ui {class GridDialog;}

//战场设置窗口
class GridDialog : public QDialog
{
	Q_OBJECT
public:
	enum ExtentState
	{
		OriginalExtent,       //!< Layer's extent
		CurrentExtent,        //!< Map canvas extent
		UserExtent,           //!< Extent manually entered/modified by the user
		ProjectLayerExtent,   //!< Extent taken from a layer within the project
		DrawOnCanvas,         //!< Extent taken from a rectangled drawn onto the map canvas
	};

	explicit GridDialog(QWidget *parent = nullptr);
	~GridDialog();
	/**
	 * Returns the current fixed aspect ratio to be used when dragging extent onto the canvas.
	 * If the aspect ratio isn't fixed, the width and height will be set to zero.      */
	QSize ratio() const { return mRatio; }
	void setLayersOptions();                //添加图层

	QPointer< QgsMapTool >              mMapToolPrevious = nullptr;
	std::unique_ptr< QgsMapToolExtent > mMapToolExtent;
	QgsMapCanvas                        *mCanvas = nullptr;

public slots:
	void setOutExtentFromDrawOnCanvas();
	void extentDrawn(const QgsRectangle &extent);
	void setRoadTypeOptions(int index);
	void createGridAbundance();
	void cleartGird();

signals:
	/** Emitted when the widget's extent is changed.   */
	void extentChanged(const QgsRectangle &r);
	void validationChanged(bool valid);
	void toggleDialogVisibility(bool visible);

private:
	void setOutputExtent(const QgsRectangle &r, const QgsCoordinateReferenceSystem &srcCrs, GridDialog::ExtentState state);
	void setValid(bool valid);
	double conculateBundance(QgsFeatureIds ids, double* r, double* k);

	Ui::GridDialog *ui;
	ExtentState mExtentState = OriginalExtent;
	//QgsCoordinateReferenceSystem mOutputCrs;
	//QgsRectangle mCurrentExtent;
	//QgsCoordinateReferenceSystem mCurrentCrs;
	//QgsRectangle mOriginalExtent;
	//QgsCoordinateReferenceSystem mOriginalCrs;
	bool            mIsValid = false;
	QSize           mRatio;
	QgsRectangle    m_extent;		//作战矩形区域
	QList<GridInfo> m_nodes;		//网格结构列表
	QgsVectorLayer  *m_pRoadLayer;	//道路，路网图层
};

#endif // GRIDDIALOG_H

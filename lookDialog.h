#ifndef LOOKDIALOG_H
#define LOOKDIALOG_H

#include <QDialog>
#include <qstandarditemmodel.h>

#include <qgsMapcanvas.h>
#include <qgsMaptoolextent.h>
#include <qgsFeaturesink.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "comm.h"

namespace Ui {class LookDialog;}

class LookDialog : public QDialog	//情报查询导出，窗口
{
    Q_OBJECT
public:
    enum ExtentState
    {
      OriginalExtent,  //!< Layer's extent
      CurrentExtent,   //!< Map canvas extent
      UserExtent,      //!< Extent manually entered/modified by the user
      ProjectLayerExtent, //!< Extent taken from a layer within the project
      DrawOnCanvas, //!< Extent taken from a rectangled drawn onto the map canvas
    };

    explicit LookDialog(QWidget *parent = nullptr);
    ~LookDialog();

    void setPlanes(QVector<tag_PlaneMessage>& planes);
 
	QPointer <QgsMapTool>			 mMapToolPrevious = nullptr;
    std::unique_ptr<QgsMapToolExtent> mMapToolExtent;
    
    QSize ratio() const { return mRatio; }
signals:
    void addJpgAnnotaionCenter1Message(Mappoint *,double width,double height, const QString imageid);

    /** Emitted when the widget's extent is changed.  */
    void extentChanged(const QgsRectangle &r);

    /**  * Emitted when the widget's validation state changes.  */
    void validationChanged(bool valid);
    void toggleDialogVisibility(bool visible );

public slots:
    //void appendImage(QString currentID,const QByteArray& array);
    void setMapCavas(QgsMapCanvas* mapcanvas);
    void refreshPlane();
    void historyPicture();
    void exportExcel();    
    void deleteHistory(int rowid);
    //void deleteHistoryFromSql( QString id );
    void deleteUnHandlerPicture( );
    void contionQuery();
    void locateCurrent();	//定位
    void deleteAll();		//删除全部
    void setOutExtentFromDrawOnCanvas();
    void extentDrawn( const QgsRectangle &extent );
    void insertPlaneStock();
    void refreshPlaneStock();	//刷新飞机库存
    void tableWidgetPicture();

private:
    void setOutputExtent( const QgsRectangle &r, const QgsCoordinateReferenceSystem &srcCrs, LookDialog::ExtentState state );
    void setValid( bool valid);
    QString buildsql();	//构造sql语句
    void showhistory(QSqlQuery &query);

    Ui::LookDialog *ui;

    QStandardItemModel			*mModelPlane;
    QVector<tag_PlaneMessage>	m_Planes;	//接收到的mavlink协议无人机列表
	QgsMapCanvas				*mCanvas	= nullptr;
    ExtentState mExtentState = OriginalExtent;

    QgsCoordinateReferenceSystem mOutputCrs;
    QgsRectangle mCurrentExtent;
    QgsCoordinateReferenceSystem mCurrentCrs;

    QgsRectangle mOriginalExtent;
    QgsCoordinateReferenceSystem mOriginalCrs;

    bool mIsValid = false;
    bool mHasFixedOutputCrs = false;
    QSize mRatio;
    QgsRectangle extent;
    //QString currentsql = "";
protected:
    void closeEvent( QCloseEvent * ) override;
};

#endif // LOOKDIALOG_H

#ifndef FixPlaneTHREAD_H
#define FixPlaneTHREAD_H

#include <QThread>
#include "comm.h"
#include <QDateTime>
#include <qdebug.h>
#include <qgsfeatureid.h>
#include <Windows.h>

class QgsAnnotationMarkerItem;
class QbmpCar;
//���˻���
class FixPlaneThread:public QThread
{
    Q_OBJECT
public:
	FixPlaneThread();
	int						m_id		= -1;		//无人机id
	QgsFeatureId			m_cachedFid	= -1;		//缓存的feature id，避免每次字符串过滤查询
	DWORD					m_lastUpdateTick = 0;	//上次更新时间，用于限速（50ms=20fps）
	//QgsAnnotationLayer		*m_pAnLayer	=nullptr;	//���˻�ͼ��
	//QgsVectorLayer			*m_pLayer = nullptr;	//���˻�ͼ��
	//tag_PlaneMessage			m_plane;
	//QbmpCar						*m_pCar=nullptr;
	//QgsAnnotationMarkerItem *m_pMarkerItem	= nullptr;
	//QgsMarkerSymbol			*m_markSym		= nullptr;
	//QgsSymbolLayerList		symList;
	//virtual ~DDSThreadHandler();    
signals:
	void FixPlaneMsg(tag_PlaneMessage *);	//�޸����˻�λ���ź�
public:
	void run();

public slots://�����źŲ�,�ص�����
	void FixPlane(tag_PlaneMessage *planeMessage);//�޸�ͼ���зɻ�λ��
};

#endif //





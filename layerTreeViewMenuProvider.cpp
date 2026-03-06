#include "stdafx.h"
#include "layerTreeViewMenuProvider.h"

#include <QMenu>
#include <QModelIndex>
#include <QIcon>
#include <QMessageBox>
#include "mainWindow.h"

// QGis include
#include <qgslayertreeviewdefaultactions.h>
#include <qgslayertreenode.h>
#include <qgslayertreemodel.h>
#include <qgslayertree.h>
#include <qgsrasterlayer.h>

layerTreeViewMenuProvider::layerTreeViewMenuProvider( QgsLayerTreeView *view, QgsMapCanvas* canvas): mView( view )
    , mCanvas( canvas)
{
}

layerTreeViewMenuProvider::~layerTreeViewMenuProvider()
{
}

QMenu* layerTreeViewMenuProvider::createContextMenu()
{
    QMenu* menu = new QMenu;
    QgsLayerTreeViewDefaultActions* actions = mView->defaultActions();
    QModelIndex idx = mView->currentIndex();

    if ( !idx.isValid())
    {
        menu->addAction( actions->actionAddGroup( menu ) );
        menu->addAction(tr( "&全部展开" ), mView, SLOT( expandAll() ) );
        menu->addAction(tr( "&全部折叠" ), mView, SLOT( collapseAll() ) );
    }
    else if ( QgsLayerTreeNode* node = mView->layerTreeModel()->index2node( idx ) )
    {
        // layer or group selected
        if (QgsLayerTree::isGroup( node ) )
        {
            menu->addAction( actions->actionZoomToGroup( mCanvas, menu ) );
            menu->addAction(tr( "&删除" ),this,SLOT(click_remove()));
            menu->addAction( actions->actionRenameGroupOrLayer( menu ) );
            if ( mView->selectedNodes( true ).count() >= 2 )
            {
                menu->addAction( actions->actionGroupSelected( menu ) );
            }
            menu->addAction( actions->actionAddGroup( menu ) );
        }
        else if (QgsLayerTree::isLayer( node ) )
        {
            QgsMapLayer* layer = QgsLayerTree::toLayer( node )->layer();
            menu->addAction( actions->actionZoomToLayer( mCanvas, menu ) );
            menu->addAction( actions->actionShowInOverview( menu ) );
            menu->addAction(tr( "&删除" ),this,SLOT(click_remove()));
            // 如果选择的是栅格图层
            if ( layer && layer->type() == QgsMapLayerType::RasterLayer )
            {
                //menu->addAction(tr( "&删除" ),this,SLOT(click_remove()));
            }
        }
    }
    return menu;
}

void layerTreeViewMenuProvider::click_remove()
{
    qDebug() << stlTargetPoint.size();
    if ( !mView ) {return;}
    foreach( QgsMapLayer* layer, mView->selectedLayers() )
    {
        QgsVectorLayer* veclayer = qobject_cast<QgsVectorLayer*>( layer );
        if ( veclayer && veclayer->isEditable() ) {return;}
        if(veclayer->name() == POINT_LAYER)
        {
            //zjrzjr delete m_PointLayer;
            //m_PointLayer = nullptr;
            auto iter = stlTargetPoint.begin();
            while(iter!=stlTargetPoint.end())
            {
                if(iter->type == QGS_POINT)
                {
                    iter = stlTargetPoint.erase(iter);
                }
                else {
                    iter++;
                }
            }

            auto it = stlAllGridInfo.begin();
            while(it!=stlAllGridInfo.end())
            {
                if(iter->type == QGS_POINT)
                {
                    it = stlAllGridInfo.erase(it);
                }
                else {
                    it++;
                }
            }
            //删除点csv文件
            //zjrzjr QFile fileTemp(pointfile);
            //fileTemp.remove();
        }
        else if(veclayer->name() == LINE_LAYER)
        {
            //delete mpLineLayer;
            //mpLineLayer = nullptr;
            auto iter = stlTargetPoint.begin();
            while(iter!=stlTargetPoint.end())
            {
                if(iter->type == QGS_LINE)
                {
                    iter = stlTargetPoint.erase(iter);
                }
                else {
                    iter++;
                }
            }

            auto it = stlAllGridInfo.begin();
            while(it!=stlAllGridInfo.end())
            {
                if(iter->type == QGS_LINE)
                {
                    it = stlAllGridInfo.erase(it);
                }
                else {
                    it++;
                }
            }
            //删除线csv文件
            //zjrzjr QFile fileTemp(linefile);
            //fileTemp.remove();
        }
        else if(veclayer->name() == POLY_LAYER)
        {
            delete g_pPolygonLayer;
			g_pPolygonLayer = nullptr;
            auto iter = stlTargetPoint.begin();
            while(iter!=stlTargetPoint.end())
            {
                if(iter->type == QGS_POLY)
                {
                    iter = stlTargetPoint.erase(iter);
                }
                else {
                    iter++;
                }
            }

            auto it = stlAllGridInfo.begin();
            while(it!=stlAllGridInfo.end())
            {
                if(iter->type == QGS_POLY)
                {
                    it = stlAllGridInfo.erase(it);
                }
                else {
                    it++;
                }
            }
            //删除面csv文件
            //zjrzjr QFile fileTemp(polyfile);
            //fileTemp.remove();
        }

        //zjrzjr layers.removeOne(layer);
        QgsProject::instance()->removeMapLayer(layer);
    }

    QList<QgsLayerTreeNode*> selectedNodes = mView->selectedNodes( true );

    foreach ( QgsLayerTreeNode* node, selectedNodes )
    {
        QgsLayerTreeGroup* parentGroup = qobject_cast<QgsLayerTreeGroup*>( node->parent() );
        if ( parentGroup )
        {
            parentGroup->removeChildNode( node );
        }
    }
    mCanvas->refresh();
}


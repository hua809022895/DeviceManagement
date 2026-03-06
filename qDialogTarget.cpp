#include "stdafx.h"
#include "qDialogTarget.h"
#include "ui_qdialogtarget.h"
#include <QSettings>
#include <QTextCodec>
#include "comm.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include "qgsmapCanvasAnnotationItem.h"
#include "jpgAnnotation.h"

QDialogTarget::QDialogTarget(QWidget *parent) :QDialog(parent), ui(new Ui::QDialogTarget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("发送任务"));

    //0靶标；1车辆；2坦克；3雷达；4导弹
	QStringList typelist = g_targetType.split("|");
	QStringList taskslist= g_targetTask.split("|");    
    ui->mComboTargettype->addItems( typelist);
    ui->mComboBoxTaskType->addItems(taskslist);

    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    button->setText(QString::fromLocal8Bit("确定"));
    QPushButton *buttoncancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    buttoncancel->setText(QString::fromLocal8Bit("取消"));

    connect( button, &QAbstractButton::clicked, this, &QDialogTarget::sendtoAttacker);

    QStringList staticlist;
    staticlist.append(QString::fromLocal8Bit("待执行"));
    staticlist.append(QString::fromLocal8Bit("锁止"));
    staticlist.append(QString::fromLocal8Bit("结束"));
    staticlist.append(QString::fromLocal8Bit("占据"));
    ui->mComboBoxStatic->addItems(staticlist );
}

QDialogTarget::~QDialogTarget()
{
    delete ui;
}

//如果是修改目标点
void QDialogTarget::setFeature(QgsFeature& feature)
{
    mfeature = feature; //   int messageid =   feature.id();	
	QString objectid	= feature.attribute("objectid").value<QString>();	//目标点ID
	QString dztype		= feature.attribute("tasktype").value<QString>();	//目标动作类型
	QString unitID		= feature.attribute("tasktype").value<QString>();	//目标所属战术单元id
	QString planeid		= feature.attribute("planeid").value<QString>();	//目标所属成员id
	QString pgroupid	= feature.attribute("pgroupid").value<QString>();	//目标所属战斗组id
	QString count		= feature.attribute("planecount").value<QString>();	//目标容量

    //x  =  feature.attribute("x").value<QString>().toInt();
    //y  =  feature.attribute("y").value<QString>().toInt();
	//z  = feature.attribute("z").value<QString>().toShort();
    ui->mLineEditTargetid->setText(objectid);
    ui->mComboTargettype->setCurrentIndex(dztype.toInt() );
    ui->mComboBoxTaskType->setCurrentIndex(unitID.toInt());    
    ui->mLineEditPlaneID->setText( planeid);
    ui->mLineEditPlaneGroupID->setText(pgroupid);
	ui->mLineEditPlaneCount->setText(count);

	QString sx = feature.attribute("x").value<QString>();
	QString sy = feature.attribute("y").value<QString>();
	QString sz = feature.attribute("z").value<QString>();

    ui->mLineEditPosition->setText(sx +QString(",")+sy);
    ui->mLineEditHeight->setText(sz);
}

void QDialogTarget::setPosition()
{
    ui->mLineEditPosition->setText(QString::number(int(pos_x*10000000))+QString(",")+QString::number(int(pos_y*10000000)));
}

void QDialogTarget::setImageTarget(Mappoint *mappoint)
{
    ui->mLineEditTargetid->setText(mappoint->rowid );
    ui->mComboTargettype->setEditText( mappoint->weaponname );
    ui->mComboBoxTaskType->setEditText( mappoint->taskname );
    ui->mLineEditPlaneCount->setText( "" );
    ui->mLineEditPlaneID->setText( "" );
    ui->mLineEditPlaneGroupID->setText( "" );
    x = mappoint->x * 10000000;
    y = mappoint->y * 10000000;
    z = 0;
    ui->mLineEditPosition->setText(QString::number(x)+QString(",")+QString::number(y));
    ui->mLineEditHeight->setText(QString::number(z));
}

void QDialogTarget::setMutiSelect()
{
    QgsFeatureList features = g_pTargetLayer->selectedFeatures();
    QgsFeatureList::iterator it = features.begin();
    for ( ; it != features.end(); ++it )
    {
        ui->mLineEditTargetid->setText(QString::number((*it).attribute("id").toLongLong()));
        ui->mLineEditPosition->setText(QString::number((*it).attribute("x").toString().toInt())+QString(",")
                                       +QString::number((*it).attribute("y").toString().toInt()));
        ui->mLineEditHeight->setText((*it).attribute("z").toString());
        break;
    }
}

void QDialogTarget::sendtoAttacker()
{
    if (userinvoke == "mutiselect")//如果是批量发送
    {
        QgsFeatureList features = g_pTargetLayer->selectedFeatures();
        QgsFeatureList::iterator it = features.begin();
        for (; it != features.end(); ++it )
        {			
			PlaneCtr *pp = new PlaneCtr;
			
			pp->objectID = (*it).attribute("objectid").toLongLong();	//目标id
			pp->type = (*it).attribute("targettype").toInt();	//目标类型
			pp->unitID = (*it).attribute("tasttype").toInt();	//任务类型
			pp->planeID = (*it).attribute("planeid").toInt();	//所属成员id
			pp->groupID = (*it).attribute("pgroupid").toInt();	//所属战斗组id
			pp->y = (*it).attribute("y").toString().toInt();
			pp->x = (*it).attribute("x").toString().toInt();
			pp->z = (*it).attribute("z").toString().toInt(); //
			pp->count = (*it).attribute("planecount").toString().toInt(); //
			pp->status= (*it).attribute("objstatic").toString().toInt(); //(uint8_t)ui->mComboBoxStatic->currentIndex();
			
			sBatchMutex.lock();
			g_BatchControls.append(pp);
			sBatchMutex.unlock();
        }
        return;
    }

    if (userinvoke=="image")
    {
        QList<QGraphicsItem *> gItemList = mCanvas->scene()->selectedItems();
        QList<QGraphicsItem *>::iterator it = gItemList.begin();
        for ( ; it != gItemList.end(); ++it )
        {
          QgsMapCanvasAnnotationItem *aItem = dynamic_cast<QgsMapCanvasAnnotationItem *>( *it );
          if ( aItem )
          {
                JpgAnnotation *pAnnt=  reinterpret_cast<JpgAnnotation*>(aItem->annotation());
				PlaneCtr *p = new PlaneCtr();
                p->objectID = pAnnt->mppoint->rowid.toUInt()+20000;
                p->type = (uint8_t)ui->mComboTargettype->currentIndex();
                p->unitID = (int16_t)(ui->mComboBoxTaskType->currentText().section(':',0,0).toUInt());
                p->planeID = (int16_t)ui->mLineEditPlaneID->text().toInt();
                p->groupID = (int16_t)ui->mLineEditPlaneGroupID->text().toInt();
                p->y = pAnnt->mppoint->x*10000000;
                p->x = pAnnt->mppoint->y*10000000;
                p->z = (int16_t)(ui->mLineEditHeight->text().toShort());
                p->count = (uint16_t)ui->mLineEditPlaneCount->text().toInt();
                p->status= (uint8_t)ui->mComboBoxStatic->currentIndex();
                sBatchMutex.lock();
                
				g_BatchControls.append(p);
                sBatchMutex.unlock();
          }
        }
        return;
    }

    if (mUdpSocketRequestAttack==nullptr)
    {
        mUdpSocketRequestAttack = new QUdpSocket( this);
        mUdpSocketRequestAttack->bind( QHostAddress(localIP), localAttackPort.toInt());
        connect(mUdpSocketRequestAttack,SIGNAL(readyRead()),this,SLOT(read_data()));
    }

    if (userinvoke =="")
    {
		mfeature.setAttribute("id", ui->mLineEditTargetid->text());
        //mfeature.setAttribute("type",QString::number(ui->mComboBoxTaskType->currentIndex()));        
        //mfeature.setAttribute("planeid",ui->mLineEditPlaneID->text());
        //mfeature.setAttribute("pgroupid",ui->mLineEditPlaneGroupID->text());
        //mfeature.setAttribute("planecount",ui->mLineEditPlaneCount->text());
        //mfeature.setAttribute("z",ui->mLineEditHeight->text());
		QString sx = QString("%1").arg(x);
		QString sy = QString("%1").arg(y);

		mfeature.setAttribute("x", ui->mLineEditHeight->text());
		mfeature.setAttribute("y", ui->mLineEditHeight->text());

		sx = ui->mLineEditPosition->text().section(',', 0, 0);
		int32_t sec_x =  ui->mLineEditPosition->text().section(',',0,0).toFloat()/10000000;
		int32_t sec_y =  ui->mLineEditPosition->text().section(',',1,1).toFloat()/10000000;
        QgsGeometry p = QgsGeometry::fromPointXY(QgsPointXY(sec_x,sec_y) );
        mfeature.setGeometry(p);
		g_pTargetLayer->startEditing();
		g_pTargetLayer->updateFeature(mfeature);
		g_pTargetLayer->commitChanges();
        QByteArray msg;
        //msg.append((char*)&planecontrol,sizeof(tag_PlaneControl));
        //mUdpSocketRequestAttack->writeDatagram(msg, QHostAddress(attackerip), attackerport);
    }

	//如果是修改目标点
    if (userinvoke == "modify")
    {
        mfeature.setAttribute("tasktype",	QString::number(ui->mComboBoxTaskType->currentIndex()));
        mfeature.setAttribute("objectid",	ui->mLineEditTargetid->text());
        mfeature.setAttribute("planeid",	ui->mLineEditPlaneID->text());
        mfeature.setAttribute("pgroupid",	ui->mLineEditPlaneGroupID->text());
        mfeature.setAttribute("planecount",	ui->mLineEditPlaneCount->text());
        mfeature.setAttribute("z",ui->mLineEditHeight->text());

        double sec_x =  ui->mLineEditPosition->text().section(',',0,0).toFloat()/10000000;
        double sec_y =  ui->mLineEditPosition->text().section(',',1,1).toFloat()/10000000;
        QgsGeometry pointTarget = QgsGeometry::fromPointXY( QgsPointXY(sec_x,sec_y) );
        mfeature.setGeometry( pointTarget);
        mfeature.setAttribute("y",QString::number(int(sec_y*10000000)));
        mfeature.setAttribute("x",QString::number(int(sec_x*10000000)));

		g_pTargetLayer->startEditing();
		g_pTargetLayer->updateFeature(mfeature);
		g_pTargetLayer->commitChanges();
    }

	//如果是新添加目标
    if (userinvoke == "add")
    {
		//QString sx = QString("%1").arg(x);		
		QString sx = ui->mLineEditPosition->text().section(',', 0,0);
		QString sy = ui->mLineEditPosition->text().section(',', 1,1);
		
		long timeid = QDateTime::currentDateTime().toTime_t();
		mfeature = QgsFeature();
		double sec_x = ui->mLineEditPosition->text().section(',', 0, 0).toFloat() / 10000000;
		double sec_y = ui->mLineEditPosition->text().section(',', 1, 1).toFloat() / 10000000;
		QgsGeometry point = QgsGeometry::fromPointXY(QgsPointXY(sec_x, sec_y));
		//   qDebug()<<pointTarget.asPoint().x()<<","<<pointTarget.asPoint().y()<<"/n";
		mfeature.setGeometry(point);

		int		iCount		=g_pTargetLayer->featureCount() + 1;
		QString objectID	=ui->mLineEditTargetid->text();			//目标id		
		QString dztype		=QString::number(ui->mComboTargettype->currentIndex()); //目标动作类型
		QString sUnitID		=ui->mComboBoxTaskType->currentText().section(':', 0, 0);//目标所属战术单元id
		QString planeID		=ui->mLineEditTargetid->text();			//目标所属成员id
		QString sGroupid	=ui->mLineEditPlaneGroupID->text();		//目标所属战斗组id
		QString sCount		=ui->mLineEditPlaneCount->text();		//目标容量

		mfeature.setAttributes(QgsAttributes()<<iCount<< "" 
			<< dztype	<< sUnitID	<< planeID	<< sGroupid
			<< QString::number(timeid)
			<< QString("")
			<< QString::number(int(sec_y * 10000000))
			<< QString::number(int(sec_x * 10000000))
			<< ui->mLineEditHeight->text()
			<< QString::number(0, 'f', 6)
			<< QString::number(0, 'f', 6)
			<< objectID			////目标id
			<< QString(3)
			<< QString(int(sec_y * 10000000))
			<< ui->mLineEditHeight->text()
			<< ""
			<< QString(int(sec_x * 10000000))
			<< "");
		
		g_pTargetLayer->startEditing();
		g_pTargetLayer->addFeature(mfeature);
		g_pTargetLayer->updateFeature(mfeature);
		g_pTargetLayer->commitChanges();
		g_pTargetLayer->triggerRepaint();
    }
}

void QDialogTarget::read_data()
{
}

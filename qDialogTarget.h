#ifndef QDIALOGTARGET_H
#define QDIALOGTARGET_H

#include <QThread>
#include <QDialog>
#include <qgsMapcanvas.h>
#include <qgsVectorlayer.h>
#include <QUdpSocket>
#include "comm.h"

#include "mavlink/air/mavlink_msg_airinfo.h"

namespace Ui {class QDialogTarget;}

class QDialogTarget : public QDialog
{
    Q_OBJECT
public:
    explicit QDialogTarget(QWidget *parent = nullptr);
    ~QDialogTarget();

    QgsFeature mfeature;
    void setFeature(QgsFeature& feature);
    void setPosition();
    void setImageTarget(Mappoint* mappoint);
    void setMutiSelect();
    QString userinvoke= "";
private:
    Ui::QDialogTarget *ui;
public:
    //qlonglong messageid;
    int x,y;    int16_t  z;
    double pos_x;
    double pos_y;
    double pos_z;
    QgsMapCanvas* mCanvas;
    QUdpSocket *mUdpSocketRequestAttack = nullptr;
public slots:
    void sendtoAttacker();
    void read_data();
};

class BatchSendThread : public QThread
{
    Q_OBJECT
public:
     QUdpSocket *mUdpSocketRequestAttack = nullptr;
public:

    BatchSendThread() 
	{
         if ( mUdpSocketRequestAttack ==nullptr)
         {
             mUdpSocketRequestAttack = new QUdpSocket( this );
             bool b=mUdpSocketRequestAttack->bind( QHostAddress(localIP), localAttackPort.toInt());
             connect(mUdpSocketRequestAttack,SIGNAL(readyRead()),this,SLOT(read_data()));
         }
    }

        void run() override 
		{
            while ( true )
            {
                sBatchMutex.lock();
                if (g_BatchControls.count()>0)
                {
                    //QByteArray msg;
                    //msg.append((char*)g_BatchControls[0], sizeof(PlaneCtr));										
					PlaneCtr *plane = g_BatchControls[0];
					mavlink_airinfo_t air;
					mavlink_message_t msg;

					uint8_t sysID = plane->planeID;
					uint8_t compID =plane->groupID;
					air.objectID= plane->objectID;
					air.type	= plane->type;
					air.unitID = plane->unitID;
					air.planeID = plane->planeID;
					air.groupID = plane->groupID;
					air.y = plane->y;
					air.x = plane->x;
					air.z = plane->z;
					air.count = plane->count;
					air.status = plane->status;
					mavlink_msg_airinfo_encode(sysID, uint8_t(compID), &msg, &air);

					uint8_t buf[BUFFER_LENGTH];
					int len =  mavlink_msg_to_send_buffer(buf, &msg);

					QByteArray ba;
					ba.append((char*)buf, len);

                    mUdpSocketRequestAttack->writeDatagram(ba, QHostAddress(attackerIP), attackerPort.toInt());
                    delete g_BatchControls[0];
					g_BatchControls.remove(0);
                    
					QString result="controls is send";
                    emit resultReady(result);
                }
                sBatchMutex.unlock();
                msleep(50);
            }
        }
    signals:
        void resultReady(const QString &s);
    public slots:
        void read_data(){};
};


#endif // QDIALOGTARGET_H

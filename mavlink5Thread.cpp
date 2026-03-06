#include "stdafx.h"
#include "mavlink5Thread.h"
#include "comm.h"
#include "dataBackDlg.h"

mavlink5Thread::mavlink5Thread()
{
}

void mavlink5Thread::run()
{
	QString mPath = QCoreApplication::applicationDirPath();
	QString connName = "mavlink_replay_thread";
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
		db.setDatabaseName(mPath + "/helloPlane.db");
		db.open();

		QSqlQuery query(db);
		query.exec(m_sql);

		QDateTime prevTime;

		while (!m_bStopped && query.next())
		{
			// Parse record timestamp for real-time replay
			QString timeStr = query.value("time").toString();
			QDateTime curTime = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm:ss.zzz");
			if (!curTime.isValid())
				curTime = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm:ss");

			tag_PlaneMessage *p = new tag_PlaneMessage;
			p->ID          = query.value("AirID").toString();
			p->planeX      = query.value("X").toString();
			p->planeY      = query.value("Y").toString();
			p->hZ          = query.value("Z").toString();
			p->xZ          = query.value("xZ").toString();
			p->Yaw         = query.value("yaw").toString();
			p->Roll        = query.value("roll").toString();
			p->Pitch       = query.value("pitch").toString();
			p->airSpeed    = query.value("airSpeed").toString();
			p->groundSpd   = query.value("groundSpd").toString();
			p->vN          = query.value("vN").toString();
			p->vE          = query.value("vE").toString();
			p->vU          = query.value("vU").toString();
			p->gpsStatus   = query.value("gpsStatus").toString();
			p->value       = query.value("voltage").toString();
			p->gpsNum      = query.value("gpsNum").toString();
			p->status      = query.value("status").toString();
			p->taskType    = query.value("taskType").toString();
			p->gjMode      = query.value("gjMode").toString();
			p->qkRunMode   = query.value("qkRunMode").toString();
			p->qkCmdMode   = query.value("qkCmdMode").toString();
			p->fkRunMode   = query.value("fkRunMode").toString();
			p->fkSysStatus = query.value("fkSysStatus").toString();
			p->jqtbTime    = query.value("jqtbTime").toString();

			emit signal_modifyPlane(p);

			// Pause support: spin while paused, exit if stopped
			while (m_bPaused && !m_bStopped)
				this->msleep(50);

			// Real-time interval: sleep based on actual timestamp difference,
			// scaled by speed factor. Cap at 2000 ms to skip long gaps.
			double spd = m_speedFactor > 0 ? m_speedFactor : 1.0;
			if (prevTime.isValid() && curTime.isValid())
			{
				qint64 deltaMs = prevTime.msecsTo(curTime);
				if (deltaMs > 0 && deltaMs <= 2000)
				{
					unsigned long sleepMs = (unsigned long)(deltaMs / spd);
					this->msleep(sleepMs > 0 ? sleepMs : 1);
				}
				else
				{
					this->msleep((unsigned long)(50 / spd));
				}
			}
			else
			{
				this->msleep((unsigned long)(50 / spd));  // first record
			}

			prevTime = curTime;
		}

		db.close();
	}
	QSqlDatabase::removeDatabase(connName);

	emit signal_setTitle();
	m_pMainDlg->m_bBacking = false;
}

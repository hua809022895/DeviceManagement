#include "stdafx.h"
#include <QThread>
#include <QSettings>
#include <QUdpSocket>
#include <qdebug.h>
#include <string.h>

#include "comm.h"
#include "recMavlinkThread.h"

RecMavlinkThread::RecMavlinkThread()
{
}

// 协议常量
static const double RAD2DEG = 180.0 / 3.14159265358979323846;

void RecMavlinkThread::run()
{
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	int port = settings.value("UDPPort/planePort", 12300).toInt();

	m_pUdp = new QUdpSocket();		// 不设 parent，在本线程中创建和销毁，避免跨线程所有权警告
	bool bindOk = m_pUdp->bind(QHostAddress::AnyIPv4, port,
	                            QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	qDebug() << "[RecMavlinkThread] bind port" << port << (bindOk ? "OK" : "FAILED") << m_pUdp->errorString();

	setPriority(QThread::HighestPriority);

	while (!m_stopped)
	{
		if (m_pDlg == nullptr || m_pDlg->m_bBacking || m_paused)
		{
			this->msleep(5);
			continue;
		}

		if (!m_pUdp->hasPendingDatagrams())
		{
			this->msleep(5);
			continue;
		}

		int recsize = (int)m_pUdp->pendingDatagramSize();
		if (recsize < 5)
		{
			m_pUdp->readDatagram(nullptr, 0);
			continue;
		}

		uint8_t buf[256] = {0};
		// 若数据报超过缓冲区大小，截断读取；需同步限制 recsize，防止 buf[recsize-1] 越界
		if (recsize > (int)sizeof(buf))
			recsize = (int)sizeof(buf);
		m_pUdp->readDatagram((char*)buf, sizeof(buf));

		// 帧头/帧尾校验
		if (buf[0] != 0x55 || buf[1] != 0xAA || buf[recsize - 1] != 0xFF)
			continue;

		uint8_t msgId = buf[3];

		// ===== 载具状态 Message ID = 195 =====
		// 格式：0x55 0xAA length(1) msgId(1) timestamp(8) vehicleId(4)
		//        lat(4) lon(4) altAsl(4) altRel(4)
		//        vN(4) vE(4) vU(4) yaw(4) pitch(4) roll(4) airspd(4) gndspd(4)
		//        gpsNum(1) voltage(4)
		//        originLat(4) originLon(4) originAlt(4)
		//        targetLat(4) targetLon(4) targetAlt(4)
		//        checksum(1)   总计 94 字节
		if (msgId == 195 && recsize >= 94)
		{
			int off = 4;

			uint64_t timestamp = 0;
			memcpy(&timestamp, buf + off, 8); off += 8;

			uint32_t vehicleId = 0;
			memcpy(&vehicleId, buf + off, 4); off += 4;

			int32_t lat = 0, lon = 0, altAsl = 0, altRel = 0;
			memcpy(&lat,    buf + off, 4); off += 4;
			memcpy(&lon,    buf + off, 4); off += 4;
			memcpy(&altAsl, buf + off, 4); off += 4;
			memcpy(&altRel, buf + off, 4); off += 4;

			float vn = 0, ve = 0, vu = 0;
			memcpy(&vn, buf + off, 4); off += 4;
			memcpy(&ve, buf + off, 4); off += 4;
			memcpy(&vu, buf + off, 4); off += 4;

			float yaw = 0, pitch = 0, roll = 0, airspd = 0, gndspd = 0;
			memcpy(&yaw,    buf + off, 4); off += 4;
			memcpy(&pitch,  buf + off, 4); off += 4;
			memcpy(&roll,   buf + off, 4); off += 4;
			memcpy(&airspd, buf + off, 4); off += 4;
			memcpy(&gndspd, buf + off, 4); off += 4;

			uint8_t gpsNum = buf[off]; off += 1;

			float voltage = 0;
			memcpy(&voltage, buf + off, 4); off += 4;
			if (voltage > 1000.0f || voltage < 0.0f)
				voltage = 0.0f;  // 超出合理范围（>1000V）视为无效，清零

			// origin/target 坐标暂不使用，跳过
			// off += 24;

			tag_PlaneMessage pm;
			pm.ID        = QString::number(vehicleId);
			pm.planeY    = QString::number(lat / 1e7, 'f', 7);		// 纬度
			pm.planeX    = QString::number(lon / 1e7, 'f', 7);		// 经度
			pm.hZ        = QString::number(altAsl / 1000.0, 'f', 2);	// 海拔 mm→m
			pm.xZ        = QString::number(altRel / 1000.0, 'f', 2);	// 相对高度 mm→m
			pm.Yaw       = QString::number(yaw   * RAD2DEG, 'f', 1);	// Rad→°
			pm.Pitch     = QString::number(pitch * RAD2DEG, 'f', 1);
			pm.Roll      = QString::number(roll  * RAD2DEG, 'f', 1);
			pm.airSpeed  = QString::number(airspd,  'f', 2);
			pm.groundSpd = QString::number(gndspd,  'f', 2);
			pm.vN        = QString::number(vn,      'f', 2);
			pm.vE        = QString::number(ve,      'f', 2);
			pm.vU        = QString::number(vu,      'f', 2);
			pm.gpsNum    = QString::number(gpsNum);
			pm.value     = QString::number(voltage, 'f', 2);
			// 以下字段由 msg 199 填写，此处置默认值
			pm.status      = "0";
			pm.gpsStatus   = "0";
			pm.taskType    = "0";
			pm.gjMode      = "0";
			pm.qkRunMode   = "0";
			pm.qkCmdMode   = "0";
			pm.fkRunMode   = "0";
			pm.fkSysStatus = "0";
			pm.jqtbTime    = "0";

			g_transitMutex.lock();
			g_transitList.push_back(pm);
			g_transitMutex.unlock();
		}

		// ===== 群控状态信息 Message ID = 199 =====
		// 格式：0x55 0xAA length(1) msgId(1) timestamp(8) vehicleId(4)
		//        cmdMode(1) runMode(1) fkRunMode(1) syncTime(8) checksum(1)
		//        总计 28 字节
		else if (msgId == 199 && recsize >= 28)
		{
			int off = 4;

			uint64_t timestamp = 0;
			memcpy(&timestamp, buf + off, 8); off += 8;

			uint32_t vehicleId = 0;
			memcpy(&vehicleId, buf + off, 4); off += 4;

			uint8_t cmdMode = buf[off++];
			uint8_t runMode = buf[off++];
			uint8_t fkMode  = buf[off++];

			uint64_t syncTime = 0;
			memcpy(&syncTime, buf + off, 8); off += 8;

			// planeX 为空 → registerPlane() 识别为仅更新模式字段
			tag_PlaneMessage pm;
			pm.ID        = QString::number(vehicleId);
			pm.qkCmdMode = QString::number(cmdMode);
			pm.qkRunMode = QString::number(runMode);
			pm.fkRunMode = QString::number(fkMode);
			// 将 syncTime (毫秒) 转为 HH:MM:SS.mmm 可读格式
		quint64 tMs  = syncTime % 1000;
		quint64 tS   = (syncTime / 1000) % 60;
		quint64 tM   = (syncTime / 60000) % 60;
		quint64 tH   = syncTime / 3600000;
		pm.jqtbTime  = QString("%1:%2:%3.%4")
		               .arg(tH,  2, 10, QChar('0'))
		               .arg(tM,  2, 10, QChar('0'))
		               .arg(tS,  2, 10, QChar('0'))
		               .arg(tMs, 3, 10, QChar('0'));

			g_transitMutex.lock();
			g_transitList.push_back(pm);
			g_transitMutex.unlock();
		}

		this->msleep(5);
	}
	// m_stopped=true，线程正常退出：在本线程内关闭并释放 socket，确保端口立即解绑
	m_pUdp->close();
	delete m_pUdp;
	m_pUdp = nullptr;
	qDebug() << "[RecMavlinkThread] stopped, socket closed";
}

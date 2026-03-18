#include "stdafx.h"
#include <QThread>
#include "comm.h"
#include <QDateTime>
#include <qdebug.h>
#include "TransitThread.h"
#include <string>
#include <vector>
#include "global.h"

using namespace std;

TransitThread::TransitThread()
{
}

void TransitThread::run()
{
	setPriority(QThread::HighestPriority);
	while (!m_stopped)
	{
		// 一次性取出整个队列，减少锁竞争次数
		g_transitMutex.lock();
		if (g_transitList.isEmpty())
		{
			g_transitMutex.unlock();
			this->msleep(5);
			continue;
		}
		QVector<tag_PlaneMessage> batch;
		batch.swap(g_transitList);		// O(1) 整批取走
		g_transitMutex.unlock();

		// 对同一 ID 的消息做合并：保留最新 msg195，将 msg199 字段叠加
		// msg195: planeX 非空；msg199: planeX 为空
		QHash<QString, tag_PlaneMessage> latestMap;	// msg195 or merged
		QHash<QString, tag_PlaneMessage> mode199Map;	// standalone msg199（尚未收到 msg195）

		for (const auto &pm : batch)
		{
			if (pm.planeX.isEmpty())
			{
				// msg199：只更新模式/同步字段
				if (latestMap.contains(pm.ID))
				{
					auto &ex = latestMap[pm.ID];
					ex.qkCmdMode = pm.qkCmdMode;
					ex.qkRunMode = pm.qkRunMode;
					ex.fkRunMode = pm.fkRunMode;
					ex.jqtbTime  = pm.jqtbTime;
				}
				else
				{
					// 暂存，等待同批次或后续 msg195 到来
					mode199Map[pm.ID] = pm;
				}
			}
			else
			{
				// msg195：全量更新，吸收已有的 msg199 字段
				tag_PlaneMessage updated = pm;
				if (latestMap.contains(pm.ID))
				{
					const auto &ex = latestMap[pm.ID];
					updated.qkCmdMode = ex.qkCmdMode;
					updated.qkRunMode = ex.qkRunMode;
					updated.fkRunMode = ex.fkRunMode;
					updated.jqtbTime  = ex.jqtbTime;
				}
				else if (mode199Map.contains(pm.ID))
				{
					const auto &ex = mode199Map[pm.ID];
					updated.qkCmdMode = ex.qkCmdMode;
					updated.qkRunMode = ex.qkRunMode;
					updated.fkRunMode = ex.fkRunMode;
					updated.jqtbTime  = ex.jqtbTime;
					mode199Map.remove(pm.ID);
				}
				latestMap[pm.ID] = updated;
			}
		}

		// 每批最多发射 N 个信号（N = 无人机数量），而不是每条包一个信号
		for (auto it = latestMap.begin(); it != latestMap.end(); ++it)
			emit receviePlaneMsg(new tag_PlaneMessage(it.value()));

		// 未匹配 msg195 的 msg199（主线程已注册的飞机需要更新模式字段）
		for (auto it = mode199Map.begin(); it != mode199Map.end(); ++it)
			emit receviePlaneMsg(new tag_PlaneMessage(it.value()));
	}
}

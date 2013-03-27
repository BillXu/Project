#ifndef _TASK__H_
#define _TASK__H_

#include "MissionMgr.h"
#include "../Base/Define.h"
#include <map>
#include "../Share/mutex.h"

class CTask
{
public:
	CTask();
	~CTask();
	int FillMainTaskInfo(unsigned int nMainId, char* buf, unsigned short* count);
	short CopyTaskInfo(unsigned int userId, char* buf, short bufLen);
public:
	void AddTaskInfo(unsigned int userId, stMainMissionConfig* pMainTask);
	void Insert(t_TaskInfo* pTask);
	t_TaskInfo* GetTaskInfo(unsigned short mainId, unsigned short leafId);
	void RemoveTask(unsigned short mainId, unsigned short leafId);

	void SetAllLeafDone()
	{
		m_bAllLeafDone = true;
	}
	void ResetAllLeafDone()
	{
		m_bAllLeafDone = false;
	}
	bool IsAllLeafDone()
	{
		return m_bAllLeafDone;
	}
private:
	typedef std::multimap<unsigned short,t_TaskInfo*>  MAP_TASKS;
	MAP_TASKS m_muTask;
	bool m_bAllLeafDone;
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
	Mutex m_mutex;
};

#endif

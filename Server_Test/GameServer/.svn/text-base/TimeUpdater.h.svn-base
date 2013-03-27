#ifndef   _TIME_UPDATER__H_
#define   _TIME_UPDATER__H_

#include "../Base/ThreadMod.h"
#include "../Share/Common.h"

class CTimeUpdater : public CThreadT
{
public:
	static CTimeUpdater* _instance()
	{
		static CTimeUpdater* __inst = NULL;
		if(!__inst)
		{
			__inst = new CTimeUpdater;
		}
		return __inst;
	}
	CTimeUpdater();
	~CTimeUpdater();
	
public:
	bool IsWeeklyTime(uint32_t tim);
	bool IsDailyTime(uint32_t tim);
	bool IsResetBattleTime(uint32_t tim);

	void SetUserUpdateTime(uint32_t tim);
	void SetUserSaveTime(uint32_t tim);
	void PushSysMsg(int sub);
	int MainLoop();
	void __run();
protected:
	uint64_t m_lastUpdateTime;
	uint32_t m_msUpdateTime;

	uint32_t m_lastDailyTime;
	uint32_t m_lastWeeklyTime;
	uint32_t m_lastResetBattleTime;

	uint64_t m_lastSaveTime;
	uint64_t m_msSaveTime;
	/*void Lock()
	{
		m_mutex.Acquire();
	}
	void UnLock()
	{
		m_mutex.Release();
	}
	Mutex m_mutex;*/

};

#define TimeUpdater CTimeUpdater::_instance()

#endif


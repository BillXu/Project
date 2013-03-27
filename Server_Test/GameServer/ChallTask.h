
#ifndef _CHALLENGE_TASK__H_
#define _CHALLENGE_TASK__H_

#include "../Base/Define.h"
#include "../Base/InternalMsg.h"
#include "../Share/mutex.h"

class CUser;
class CChallSubTask
{
public:
	CChallSubTask(unsigned int roleId, unsigned short subId, unsigned int roleId2);
	CChallSubTask(stChallTaskInfo* pInfo);
	~CChallSubTask();
	
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline bool IsDelete()
	{
		return (changeFlag == 2);
	}
	inline void SetChangeFlag()
	{
		changeFlag = 1;
	}
	inline void SetDelFlag()
	{
		changeFlag = 2;
	}
	inline void SetAddFlag()
	{
		changeFlag = 3;
	}
	inline void ResetFlag()
	{
		changeFlag = 0;
	}
public:
	void Reset();
	t_ChallTask m_info;
protected:
	unsigned char changeFlag;	//3Ôö ; 2 É¾ ;1 ¸Ä
};

class CChallTaskMgr
{
public:
	void Reset();
	unsigned short SubTaskFinish(unsigned int roleId);
	unsigned short GetFinishCount();
	inline void SetTaskFinish()
	{
		m_bFinishFlag = true;
	}
	inline bool Finish()
	{
		return m_bFinishFlag;
	}

	inline bool IsGetReward()
	{
		return m_bGetRewardFlag;
	}
	inline void FinishGetReward()
	{
		m_bGetRewardFlag = true;
	}
	short CopyChallTaskInfo(unsigned int roleId, char* buf, short bufLen);
	int Add(CChallSubTask* pSub);
	int Del(CChallSubTask* pSub);
	CChallSubTask* GetSubChall(unsigned short subId);
	bool HasTask();
	void ResetList(unsigned int userId);
	int OnGetChallTaskList(unsigned int userId, char* buf, unsigned short *cnt);
	int OnFlushChallTaskList(unsigned int userId, char* buf, unsigned short *cnt);

public:
	void SetUser(CUser* pUser)
	{
		assert(pUser);
		m_pUser = pUser;
	}
	CChallTaskMgr();
	~CChallTaskMgr();
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
private:
	Mutex m_mutex;
	CUser* m_pUser;
public:
	bool m_bFinishFlag;
	bool m_bGetRewardFlag;
	typedef std::map<uint16_t, CChallSubTask*> MAP_CHALL_TASK;
	MAP_CHALL_TASK m_mapChallSubTask;
};

#endif

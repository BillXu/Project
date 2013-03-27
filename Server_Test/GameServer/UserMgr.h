#ifndef _USER_MGR__H_
#define _USER_MGR__H_

#include "../Share/Common.h"
#include "../Share/mutex.h"
#include "User.h"
#include <map>
using namespace std;

class CUserMgr
{
public:
	void CalculateAllRole();
	void Update(uint32_t tim);
	void ClearAllWonTimes();
	//void OnDailyReward();
	void OnSaveData();
	void OnChallTaskUpdate();
	void OnBattleReset();
	int AddUser(CUser* pUser);
	int DelUser(uint32_t uid);
	CUser* GetUser(uint32_t uid);
	CUser* GetUser(char* pName);

	unsigned short RandUserList(unsigned int userId, CUser** ppUserList, int count);
	//unsigned short RandChallTaskList(unsigned int userId, CUser** ppUserList, int count);
	int CopyUserList(unsigned int userId, unsigned char type,  unsigned short count, char* buf, unsigned short *cnt);
	int CopyChallTaskList(unsigned int userId, char* buf, unsigned short *cnt);
	void CreateChallTaskList(CUser* pUser);
	
	//int BroadcastOther(uint32_t uid, const char* buf, int bufLen);
public:
	CUserMgr();
	~CUserMgr();
	static CUserMgr* _instance()
	{
		static CUserMgr* __inst = NULL;
		if(!__inst)
		{
			__inst = new CUserMgr();
		}
		return __inst;
	}
private:
	void Lock()
	{
		m_mutex.Acquire();
	}
	void UnLock()
	{
		m_mutex.Release();
	}
private:
	Mutex m_mutex;
	typedef std::map<uint32_t,CUser*> MUP_USER;
	MUP_USER m_UserMap;
	typedef std::multimap<uint16_t, CUser*> MMAP_USER;	//level - CUser*
	MMAP_USER m_mmapUser[2];	//<20 ºÍ>=20¼¶
};

#define g_UserMgr CUserMgr::_instance()

#endif


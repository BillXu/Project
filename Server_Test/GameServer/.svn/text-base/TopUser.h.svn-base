
#ifndef _TOP_INFO__H_
#define _TOP_INFO__H_

#include "User.h"
//#include "../Base/Define.h"
//#include "../Base/InternalMsg.h"
//#include "../Share/mutex.h"

class CTopUser
{
public:
	CTopUser(stTopInfo* pInfo);
	~CTopUser();
	
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline void SetChangeFlag()
	{
		changeFlag = 1;
	}
	inline void ResetFlag()
	{
		changeFlag = 0;
	}
	inline unsigned int GetRoleId()
	{
		return m_info.roleId;
	}
public:
	t_TopInfo m_info;
protected:
	unsigned char changeFlag;	//3Ôö ; 2 É¾ ;1 ¸Ä
};

class CTopUserMgr
{
public:
	int Add(CTopUser* rel);
	int Del(CTopUser* rel);

	int FillTopRoleInfo(char* buf, short bufLen, unsigned short* cnt);
	short CopyTopInfo(char* buf, short bufLen);

	CTopUser* GetTopUser(unsigned int guid);
	bool HasEmptyPos();
	unsigned int GetTopId(unsigned int userId);
	unsigned int CheckTopNo(CUser* pUser);

	//void OnDailyReward();
	void OnWeeklyReward();
public:
	CTopUserMgr();
	~CTopUserMgr();
	static CTopUserMgr* _instance()
	{
		static CTopUserMgr* __inst = NULL;
		if(!__inst)
		{
			__inst = new CTopUserMgr();
		}
		return __inst;
	}
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
public:
	typedef std::map<uint32_t, CTopUser*> MAP_SHOP;
	MAP_SHOP m_mapTopUser;
};

#define g_TopInfoMgr CTopUserMgr::_instance()

#endif


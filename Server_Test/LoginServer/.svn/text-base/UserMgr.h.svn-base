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
	int AddUser(CUser* pUser);
	int DelUser(uint32_t uid);
	CUser *GetUser(uint32_t uid); 
	int BroadcastOther(uint32_t uid, const char* buf, int bufLen);
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
	
	std::map<uint32_t,CUser*> m_UserMap;
};

#define g_UserMgr CUserMgr::_instance()

#endif


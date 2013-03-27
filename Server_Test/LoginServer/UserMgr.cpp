#include "UserMgr.h"

CUserMgr::CUserMgr()
{
	
}

CUserMgr::~CUserMgr()
{
}

int CUserMgr::AddUser(CUser* pUser)
{
	int ret = 0;
	Lock();
	std::map<uint32_t,CUser*>::iterator it = m_UserMap.find(pUser->GetUserId());
	if(it == m_UserMap.end())
	{
		m_UserMap.insert(make_pair(pUser->GetUserId(),pUser));
		ret = m_UserMap.size();
	}
	else
	{
		m_UserMap.erase(it->first);
		m_UserMap.insert(make_pair(pUser->GetUserId(),pUser));
		printf("CUserMgr::AddUser玩家已存在\n");
		ret = -1;
	}
	UnLock();
	
	return ret;
}

int CUserMgr::DelUser(uint32_t uid)
{
	Lock();
	m_UserMap.erase(uid);
	UnLock();
	
	return 0;
}

CUser *CUserMgr::GetUser(uint32_t uid)
{
	std::map<uint32_t,CUser*>::iterator it = m_UserMap.find(uid);
	if(it == m_UserMap.end())
		return NULL;
	else
		return it->second;
}

int CUserMgr::BroadcastOther(uint32_t uid, const char* buf, int bufLen)
{
	/*std::map<uint32_t,CUser*>::iterator it = m_UserMap.begin();
	for(;it!=m_UserMap.end();++it)
	{
		CUser *TmpUser = it->second;		
		if( uid != TmpUser->GetUid() && !TmpUser->IsBroken() )
		{
			printf("大厅中的群聊: uid=%llu\n", TmpUser->GetUid());
			TmpUser->Send(buf, bufLen);
		}
	}*/
	
	return 0;
}





















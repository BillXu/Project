
#include "TopUser.h"
#include "UserMgr.h"
#include "ServerTreator.h"

CTopUser::CTopUser(stTopInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.topId = pInfo->topId;
	m_info.roleId = pInfo->roleId;
	m_info.lastTime = pInfo->lastTime;
	changeFlag = 0;
}

CTopUser::~CTopUser()
{
}

CTopUserMgr::CTopUserMgr()
{
	//
}

CTopUserMgr::~CTopUserMgr()
{
	//
}

int CTopUserMgr::Add(CTopUser* shop)
{
	Lock();
	m_mapTopUser.insert(MAP_SHOP::value_type(shop->m_info.topId, shop));
	UnLock();
	return m_mapTopUser.size();
}

int CTopUserMgr::Del(CTopUser* pTopUser)
{
	Lock();
	MAP_SHOP::iterator it = m_mapTopUser.find(pTopUser->m_info.topId);
	if (it != m_mapTopUser.end())
	{
		m_mapTopUser.erase(it);
	}
	UnLock();
	return m_mapTopUser.size();
}

int CTopUserMgr::FillTopRoleInfo(char* buf, short bufLen, unsigned short* cnt)
{
	assert(buf);
	//char* p = buf;
	short count = 0;
	int len = 0;

	CTopUser* pObj = NULL;
	unsigned int userId = 0;
	char * pName = NULL;
	stTopRoleInfo* pInfo = NULL;
	Lock();
	MAP_SHOP::iterator ipos = m_mapTopUser.begin();
	MAP_SHOP::iterator iend = m_mapTopUser.end();
	while(ipos != iend)
	{
		pObj = (CTopUser*)(ipos->second);
		userId = pObj->GetRoleId(); 
		if( userId > 0 )
		{
			CUser* pUser = g_UserMgr->GetUser(userId);
			if (!pUser)
			{
				printf("user %u not inline long time @@@@@\n", userId);
				ServerTreator->LoginUserId(userId);
				continue;
			}
			++count;
			pInfo = (stTopRoleInfo*)(buf+len);
			pInfo->userId = pObj->m_info.roleId;
			pInfo->topId = pObj->m_info.topId;
			pInfo->godId = pUser->GetFirstGodId();
			pInfo->level = pUser->GetLevel();
			pInfo->ability = pUser->GetAbility();
			pInfo->wonTimes = pUser->GetWonTimes();
			pName = pUser->GetName();
			pInfo->nameLen = strlen(pName);
			pInfo->pName = NULL;
			len += sizeof(stTopRoleInfo);
			memcpy(buf+len, pName, pInfo->nameLen);
			len += pInfo->nameLen;
		}
		++ipos;
	}
	UnLock();
	*cnt = count;
	return len;
}

short CTopUserMgr::CopyTopInfo(char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CTopUser* pObj = NULL;
	stTopInfo* pInfo = NULL;
	Lock();
	MAP_SHOP::iterator ipos = m_mapTopUser.begin();
	MAP_SHOP::iterator iend = m_mapTopUser.end();
	while(ipos != iend)
	{
		pObj = (CTopUser*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stTopInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->topId = pObj->m_info.topId;
			pInfo->roleId = pObj->m_info.roleId;
			pInfo->lastTime = pObj->m_info.lastTime;
			p += sizeof(stTopInfo);
			/*if(pObj->IsDelete())
			{
				Del(pObj);
			}*/
			pObj->ResetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}

CTopUser* CTopUserMgr::GetTopUser(unsigned int roleId)
{
	CTopUser* pRet = NULL;
	Lock();
	int sz = m_mapTopUser.size();
	for (int i=1; i <= sz; i++)
	{
		if (roleId == m_mapTopUser[i]->m_info.roleId)
		{
			pRet = m_mapTopUser[i];
			break;
		}
	}
	UnLock();
	return pRet;
}

bool CTopUserMgr::HasEmptyPos()
{
	bool bRet = false;
	Lock();
	int sz = m_mapTopUser.size();
	for (int i=sz-1; i >= 0; i--)
	{
		if (m_mapTopUser[i]->m_info.roleId > 0)
		{
			bRet = true;
			break;
		}
	}
	UnLock();

	return bRet;
}

unsigned int CTopUserMgr::GetTopId(unsigned int userId)
{
	int sz = m_mapTopUser.size();
	for (int i=1; i <= sz; i++)
	{
		if (m_mapTopUser[i]->m_info.roleId == userId)
		{
			return m_mapTopUser[i]->m_info.topId;
		}
	}
	return 0;
}

unsigned int CTopUserMgr::CheckTopNo(CUser* pUser)
{
	CUser* pTemUser = NULL;
	//CTopUser* pTopUser = NULL;
	Lock();
	int sz = m_mapTopUser.size();
	unsigned int userId = pUser->GetUserId();
	unsigned int newTopId = 0;
	unsigned int oldTopId = GetTopId(userId);
	int i=sz;
	for (;i > 0; --i)
	{
		pTemUser = g_UserMgr->GetUser(m_mapTopUser[i]->m_info.roleId);
		if (!pTemUser)
		{
			continue;
		}
		if (pUser->GetWonTimes() >= pTemUser->GetWonTimes())
		{
			newTopId = m_mapTopUser[i]->m_info.topId; 
		}
		else
		{
			break;
		}
	}
	if (newTopId == oldTopId)
	{
		;
	}
	else if (newTopId > 0 && oldTopId <= 0)
	{//新插入到newTopId名次，pUser后的排名依次后移
		unsigned int i=sz;
		for (;i > newTopId;--i)
		{
			m_mapTopUser[i]->m_info.roleId = m_mapTopUser[i-1]->m_info.roleId;
			m_mapTopUser[i]->SetChangeFlag();
		}
		m_mapTopUser[i]->m_info.roleId = userId;
		m_mapTopUser[i]->SetChangeFlag();
	}
	else if ( oldTopId > 0 && newTopId > oldTopId)
	{//名次后移到newTopId
		unsigned int i = oldTopId;
		for(; i > newTopId; i++)
		{
			m_mapTopUser[i]->m_info.roleId = m_mapTopUser[i+1]->m_info.roleId;
			m_mapTopUser[i]->SetChangeFlag();
		}
		m_mapTopUser[i]->m_info.roleId = userId;
		m_mapTopUser[i]->SetChangeFlag();
	}
	else if (newTopId > 0 && newTopId <= oldTopId)
	{//名次前移到newTopId
		unsigned int i = oldTopId ;
		for(; i > newTopId; i--)
		{
			m_mapTopUser[i]->m_info.roleId = m_mapTopUser[i-1]->m_info.roleId;
			m_mapTopUser[i]->SetChangeFlag();
		}
		m_mapTopUser[i]->m_info.roleId = userId;
		m_mapTopUser[i]->SetChangeFlag();
	}
	else if(oldTopId > 0)
	{//除名则移到最有一名
		int i=oldTopId;
		unsigned int myRoleId = m_mapTopUser[i]->m_info.roleId;
		for(; i < sz; i++)
		{
			m_mapTopUser[i]->m_info.roleId = m_mapTopUser[i+1]->m_info.roleId;
			m_mapTopUser[i]->SetChangeFlag();
		}
		m_mapTopUser[i]->m_info.roleId = myRoleId;
		m_mapTopUser[i]->SetChangeFlag();
	}
	UnLock();
	return newTopId;
}

/*void CTopUserMgr::OnDailyReward()
{
	//
}*/
void CTopUserMgr::OnWeeklyReward()
{
	printf("CTopUserMgr::OnWeeklyReward()\n");
	int sz = m_mapTopUser.size();
	for (int i=1; i <= sz; i++)
	{
		CUser* pUser = g_UserMgr->GetUser(m_mapTopUser[i]->m_info.roleId);
		if ( pUser)
		{
			pUser->ExecHonor();
		}
	}
	//清空所有玩家战功
	g_UserMgr->ClearAllWonTimes();
}



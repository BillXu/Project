#include "RelationMgr.h"
#include "UserMgr.h"
#include "ServerTreator.h"
//#include "../Base/InternalMsg.h"

CRelationMgr::CRelationMgr()
{	
	m_pUser = NULL;
}

CRelationMgr::~CRelationMgr()
{
}

int CRelationMgr::Add(CRelation* rel)
{
	eRelation type = rel->GetType();
	if (eFoe == type)
	{
		return AddFoe(rel);
	} 
	else
	{
		return AddFriend(rel);
	}
}

int CRelationMgr::Del(CRelation* rel)
{
	eRelation type = rel->GetType();
	if (eFoe == type)
	{
		return DelFoe(rel);
	} 
	else
	{
		return DelFrined(rel);
	}
}

int CRelationMgr::AddFoe(CRelation* rel)
{
	m_vFoe.insert(MAP_RELATION::value_type(rel->GetUserId(), rel));
	return m_vFoe.size();
}

CRelation* CRelationMgr::GetFoe(unsigned int userId)
{
	MAP_RELATION::iterator ipos = m_vFoe.find(userId);
	if (ipos != m_vFoe.end())
	{
		return ipos->second;
	}
	return NULL;
}

int CRelationMgr::AddFoe(unsigned int userId)
{
	Lock();
	CRelation* pObj = GetFoe(userId);
	if (!pObj)
	{
		pObj = new CRelation();
		pObj->SetType(eFoe);
		pObj->SetUserId(userId);
		pObj->SetTime(getSecTime());
		pObj->SetAddFlag();
		AddFoe(pObj);
		m_pUser->SaveRelationInfo();
	}
	else
	{
		pObj->SetTime(getSecTime());
		pObj->SetChangeFlag();
	}
	UnLock();
	return 0;
}

int CRelationMgr::DelFoe(CRelation* rel)
{
	Lock();
	MAP_RELATION::iterator it = m_vFoe.find(rel->GetUserId());
	if (it != m_vFoe.end())
	{
		m_vFoe.erase(it);
	}
	UnLock();
	return m_vFoe.size();
}

int CRelationMgr::AddFriend(CRelation* rel)
{
	//Lock();
	m_vFriend.insert(MAP_RELATION::value_type(rel->GetUserId(), rel));
	//UnLock();
	return m_vFriend.size();
}

int CRelationMgr::DelFrined(CRelation* rel)
{
	Lock();
	MAP_RELATION::iterator it = m_vFriend.find(rel->GetUserId());
	if (it != m_vFriend.end())
	{
		m_vFriend.erase(it);
	}
	UnLock();
	return m_vFriend.size();
}

short CRelationMgr::CopyRelationInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;
	MAP_RELATION::iterator ipos = m_vFoe.begin();
	MAP_RELATION::iterator iend = m_vFoe.end();
	CRelation* pObj = NULL;
	stRelationInfo* pInfo = NULL;
	while(ipos != iend)
	{
		pObj = (CRelation*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stRelationInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->roleId = pObj->GetUserId();
			pInfo->type = (unsigned char)(pObj->GetType());
			pInfo->addTime = pObj->GetTime();
			p += sizeof(stRelationInfo);
			if (2 == pObj->GetChangeFlag())
			{
				DelFoe(pObj);
			}
			pObj->ResetFlag();
		}
		++ipos;
	}

	ipos = m_vFriend.begin();
	iend = m_vFriend.end();
	while(ipos != iend)
	{
		pObj = (CRelation*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stRelationInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->roleId = pObj->GetUserId();
			pInfo->type = (unsigned char)(pObj->GetType());
			p += sizeof(stRelationInfo);
			if (2 ==  pObj->GetChangeFlag())
			{
				DelFrined(pObj);
			}
			pObj->ResetFlag();
		}
		++ipos;
	}
	return count;
}

int CRelationMgr::CopyRelationList(unsigned char type, unsigned short count, char* buf, unsigned short* cnt)
{
	assert(1==type || 2==type);
	assert(buf);
	
	MAP_RELATION::iterator ipos = m_vFoe.begin();
	MAP_RELATION::iterator iend = m_vFoe.end();
	CRelation* pObj = NULL;
	//stRelationInfo* pInfo = NULL;
	int len = 0;
	*cnt = 0;
	while(ipos != iend)
	{
		pObj = (CRelation*)(ipos->second);
		CUser* pUser = g_UserMgr->GetUser(pObj->GetUserId());
		if (pUser)
		{
			if( 1==type && eFoe == pObj->GetType() )
			{
				len += pUser->FillSimpInfo(buf+len);
				(*cnt)++;
			}
			else if(2==type && eFriend == pObj->GetType())
			{
				len += pUser->FillSimpInfo(buf+len);
				(*cnt)++;
			}
		} 
		++ipos;
	}

	return len;
}

















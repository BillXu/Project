
#include "Challenge.h"
#include "UserMgr.h"

CChallenge::CChallenge(unsigned int myRoleId, unsigned int roleId)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.myRoleId = myRoleId;
	m_info.roleId = roleId;
	changeFlag = 0;
}

CChallenge::CChallenge(stChallengeInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.myRoleId = pInfo->myRoleId;
	m_info.roleId = pInfo->roleId;
	m_info.times = pInfo->times;
	m_info.type = pInfo->type;
	m_info.lastTime = pInfo->lastTime;
	changeFlag = 0;
}

CChallenge::~CChallenge()
{
}

CChallengeMgr::CChallengeMgr()
{
	m_pUser = NULL;
}

CChallengeMgr::~CChallengeMgr()
{
	//
}

bool CChallengeMgr::CanChallenge(unsigned int roleId)
{
	CChallenge* pHistory = GetChallenge(roleId);
	if (pHistory)
	{
		if (pHistory->m_info.times >= 3)
		{
			return false;
		}
	} 
	return true;
}

void CChallengeMgr::AddHistory(unsigned int myRoleId, unsigned int roleId, unsigned char type)
{
	CChallenge* pChall = GetChallenge(roleId);
	if(pChall)
	{
		pChall->m_info.times++;
		pChall->SetChangeFlag();
	}
	else
	{
		Add(myRoleId, roleId, type);
	}
}

int CChallengeMgr::Add(unsigned int myRoleId, unsigned int roleId, unsigned char type)
{
	CChallenge* pObj = new CChallenge(myRoleId, roleId);
	pObj->m_info.lastTime = getSecTime();
	pObj->SetAddFlag();
	Add(pObj);
	m_pUser->SaveChallengeHistory();
	return 0;
}

int CChallengeMgr::Add(CChallenge* pHistory)
{
	Lock();
	m_mapChallenge.insert(MAP_CHALLENGE::value_type(pHistory->m_info.roleId, pHistory));
	UnLock();
	return m_mapChallenge.size();
}

int CChallengeMgr::Del(CChallenge* pChall)
{
	Lock();
	MAP_CHALLENGE::iterator it = m_mapChallenge.find(pChall->m_info.roleId);
	if (it != m_mapChallenge.end())
	{
		m_mapChallenge.erase(it);
	}
	UnLock();
	return m_mapChallenge.size();
}

CChallenge* CChallengeMgr::GetChallenge(unsigned int roleId)
{
	CChallenge* pRet = NULL;
	Lock();
	MAP_CHALLENGE::iterator ipos = m_mapChallenge.find(roleId);
	if (ipos != m_mapChallenge.end())
	{
		pRet = ipos->second;
	}
	UnLock();
	return pRet;
}

short CChallengeMgr::CopyChallengeInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CChallenge* pObj = NULL;
	stChallengeInfo* pInfo = NULL;
	Lock();
	MAP_CHALLENGE::iterator ipos = m_mapChallenge.begin();
	MAP_CHALLENGE::iterator iend = m_mapChallenge.end();
	while(ipos != iend)
	{
		pObj = (CChallenge*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stChallengeInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->myRoleId = pObj->m_info.myRoleId;
			pInfo->roleId = pObj->m_info.roleId;
			pInfo->times = pObj->m_info.times;
			pInfo->type = pObj->m_info.type;
			pInfo->lastTime = pObj->m_info.lastTime;
			p += sizeof(stChallengeInfo);
			if (pObj->IsDelete())
			{
				Del(pObj);
			}
			pObj->ResetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}



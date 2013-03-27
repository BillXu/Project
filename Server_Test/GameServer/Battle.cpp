#include "Battle.h"
#include "MissionMgr.h"

CBattle::CBattle(unsigned short mainId, unsigned short curLeafId)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.mainId = mainId;
	m_info.curLeafId = curLeafId;
	changeFlag = 0;
}
CBattle::CBattle(stBattleInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.roleId = pInfo->roleId;
	m_info.mainId = pInfo->mainId;
	m_info.curLeafId = pInfo->curLeafId;
	changeFlag = 0;
}

CBattle::~CBattle()
{}

CBattleMgr::CBattleMgr()
{
	m_bIsAllOpen = false;
	m_lastResetTime = 0;
}

CBattleMgr::~CBattleMgr()
{
}

bool CBattleMgr::CheckOpenBattle(unsigned int userId, unsigned short level)
{
	if (m_bIsAllOpen)
	{
		return false;
	}
	CMissionMgr* pMgr = CMissionMgr::SharedMissionMgr();
	int n = pMgr->GetMainMissionCount(eMission_Battle);
	stMainMissionConfig* pMainConfig = NULL;
	for (int i=0; i < n; i++)
	{
		pMainConfig = pMgr->GetMainMissionByIndex(i, eMission_Battle);
		if (level >= pMainConfig->iLevelNeed)
		{
			if( Add(pMainConfig->iMainID, 0) >= n)
			{
				m_bIsAllOpen = true;
			}
		}
	}
	return false;
}

int CBattleMgr::FillBattleInfo(char* buf, unsigned short* count)
{
	/*CMissionMgr* pMgr = CMissionMgr::SharedMissionMgr();
	int n = pMgr->GetMainMissionCount(eMission_Battle);
	stMainMissionConfig* pMainConfig = NULL;
	int offset = 0;
	for (int i=0; i < n; i++)
	{
		pMainConfig = pMgr->GetMainMissionByIndex(i, eMission_Battle);
		memcpy(buf+offset, &(pMainConfig->iMainID), sizeof(unsigned short));
		offset += sizeof(unsigned short);
		memcpy(buf+offset, &(pMainConfig->i), sizeof(unsigned short));
		offset += sizeof(unsigned short);
	}
	return n;*/
	Lock();
	MAP_BATTLE::iterator ipos = m_mapBattle.begin();
	MAP_BATTLE::iterator iend = m_mapBattle.end();
	int offset = 0;
	CBattle* pInfo = NULL;
	int n = 0;
	while(ipos != iend)
	{
		++n;
		pInfo = ipos->second;
		memcpy(buf+offset, &(pInfo->m_info.mainId), sizeof(unsigned short));
		offset += sizeof(unsigned short);
		memcpy(buf+offset, &(pInfo->m_info.curLeafId), sizeof(unsigned short));
		offset += sizeof(unsigned short);
		++ipos;
	}
	UnLock();
	*count = n;
	return offset;
}

int CBattleMgr::Add(unsigned short mainId, unsigned short curId)
{
	CBattle* pBattle = new CBattle(mainId);
	pBattle->m_info.curLeafId = curId;
	m_mapBattle.insert(MAP_BATTLE::value_type(mainId, pBattle));
	pBattle->SetAddFlag();
	return m_mapBattle.size();
}

void CBattleMgr::Add(CBattle* pBattle)
{
	Lock();
	m_mapBattle.insert(MAP_BATTLE::value_type(pBattle->m_info.mainId, pBattle));
	pBattle->SetAddFlag();
	UnLock();
}

void CBattleMgr::Del(CBattle* pBattle)
{
	MAP_BATTLE::iterator ipos = m_mapBattle.find(pBattle->m_info.mainId);
	if (ipos != m_mapBattle.end())
	{
		m_mapBattle.erase(ipos);
	}
}

CBattle* CBattleMgr::Get(unsigned short mainId)
{
	MAP_BATTLE::iterator ipos = m_mapBattle.find(mainId);
	MAP_BATTLE::iterator iend = m_mapBattle.end();
	if (ipos != iend)
	{
		return ipos->second;
	}
	else
	{
		return NULL;
	}
}

bool CBattleMgr::Update(unsigned short mainId, unsigned short curId)
{
	bool bRet = false;
	Lock();
	CBattle* pBattle = Get(mainId);
	if (pBattle)
	{
		pBattle->Update(curId);
	} 
	else
	{
		//Add(mainId, curId);
		CBattle* pBattle = new CBattle(mainId);
		pBattle->m_info.curLeafId = curId;
		m_mapBattle.insert(MAP_BATTLE::value_type(mainId, pBattle));
		pBattle->SetAddFlag();
		bRet = true;
	}
	UnLock();
	return bRet;
}

short CBattleMgr::CopyBattleInfo(char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CBattle* pObj = NULL;
	stBattleInfo* pInfo = NULL;
	Lock();
	MAP_BATTLE::iterator ipos = m_mapBattle.begin();
	MAP_BATTLE::iterator iend = m_mapBattle.end();
	while(ipos != iend)
	{
		pObj = ipos->second;
		if( pObj->GetChangeFlag() > 0)
		{
			++count;
			pInfo = (stBattleInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->roleId = pObj->m_info.roleId;
			pInfo->mainId = pObj->m_info.mainId;
			pInfo->curLeafId = pObj->m_info.curLeafId;
			p += sizeof(stBattleInfo);
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

unsigned int CBattleMgr::GetResetLeftTime()
{
	time_t curTime = getSecTime();
	tm* tmp = localtime(&curTime);
	unsigned int sec = tmp->tm_hour*3600 + tmp->tm_min*60 + tmp->tm_sec;
	unsigned int leftSec = 24*3600 - sec + 3*3600;//每日凌晨3点重置副本
	
	return leftSec;
}

void CBattleMgr::Reset()
{
	CBattle* pObj = NULL;
	Lock();
	MAP_BATTLE::iterator ipos = m_mapBattle.begin();
	MAP_BATTLE::iterator iend = m_mapBattle.end();
	while(ipos != iend)
	{
		pObj = ipos->second;
		if( pObj )
		{
			pObj->m_info.curLeafId = 0;
			pObj->SetChangeFlag();
		}
		++ipos;
	}
	m_lastResetTime = getSecTime();
	UnLock();
}




#include "UserMgr.h"
#include "CompeteRankConfig.h"
#include "ChallTask.h"

CUserMgr::CUserMgr()
{
	
}

CUserMgr::~CUserMgr()
{
}

void CUserMgr::CalculateAllRole()
{
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->CalculateRole();
		++ipos;
	}
}

void CUserMgr::Update(uint32_t tim)
{
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->Update(tim);
		++ipos;
	}
}

void CUserMgr::ClearAllWonTimes()
{
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->ClearWonTimes();
		ipos++;
	}
}

/*void CUserMgr::OnDailyReward()
{
	CCompeteRankConfig* pTmpConf = CCompeteRankConfig::SharedMgr();
	CCompeteRankConfig::stRankClearing* pClear = NULL;

	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	CUser * pUser = NULL;
	while(ipos != iend)
	{
		pUser = ipos->second;
		pClear = pTmpConf->GetRankClearingForRank(pUser->GetOldTopId(), false);
		stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(pClear->nGroupID);
		pUser->OnReward(pRewardGroup, 3);//3 每日排行奖励
		ipos++;
	}
}*/

void CUserMgr::OnSaveData()
{
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->SaveData();
		ipos++;
	}
}

void CUserMgr::OnChallTaskUpdate()
{
	printf("CUserMgr::OnChallTaskUpdate\n");
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	std::map<uint32_t,CUser*>::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->ChallTaskUpdate();
		ipos++;
	}
}

void CUserMgr::OnBattleReset()
{
	printf("CUserMgr::OnBattleReset\n");
	MUP_USER::iterator ipos = m_UserMap.begin();
	MUP_USER::iterator iend = m_UserMap.end();
	while(ipos != iend)
	{
		ipos->second->OnBattleReset();
		ipos++;
	}
}

int CUserMgr::AddUser(CUser* pUser)
{
	//printf("CUserMgr::AddUser( %u)\n", m_UserMap.size());
	Lock();
	std::map<uint32_t,CUser*>::iterator it = m_UserMap.find(pUser->GetUserId());
	if(it == m_UserMap.end())
	{
		m_UserMap.insert(make_pair(pUser->GetUserId(),pUser));
		unsigned short level = pUser->GetLevel();
		if(level < LEVLE_BIG_BEGIN)
		{
			m_mmapUser[0].insert(MMAP_USER::value_type(level, pUser));
		}
		else
		{
			m_mmapUser[1].insert(MMAP_USER::value_type(level, pUser));
		}
	}
	else
	{
		m_UserMap.erase(it->first);
		m_UserMap.insert(make_pair(pUser->GetUserId(),pUser));
		printf("CUserMgr::AddUser玩家已存在");
	}

	UnLock();
	
	return m_UserMap.size();
}

int CUserMgr::DelUser(uint32_t uid)
{
	Lock();
	m_UserMap.erase(uid);
	UnLock();
	
	return 0;
}

CUser *CUserMgr::GetUser(char* pName)
{
	CUser* pUser = NULL;
	Lock();
	std::map<uint32_t,CUser*>::iterator ipos = m_UserMap.begin();
	while(ipos != m_UserMap.end())
	{
		pUser = ipos->second;
		if(!strcmp(pName,pUser->GetName()))
		{
			UnLock();
			return pUser;
		}
		++ipos;
	}
	UnLock();
	return NULL;
}

CUser *CUserMgr::GetUser(uint32_t uid)
{
	Lock();
	std::map<uint32_t,CUser*>::iterator it = m_UserMap.find(uid);
	if(it == m_UserMap.end())
	{
		UnLock();
		return NULL;
	}
	else
	{
		UnLock();
		return it->second;
	}
}

unsigned short CUserMgr::RandUserList(unsigned int userId, CUser** ppUserList, int count)
{
	CUser* pUser = GetUser(userId);
	assert(pUser);
	unsigned short level = pUser->GetLevel();
	int index = 0;
	//Lock();
	if (level < LEVLE_BIG_BEGIN)
	{
		int sz = m_mmapUser[0].size();
		MMAP_USER::iterator ipos = m_mmapUser[0].begin();
		int n = sz > count ? count : sz;
		if(sz > count)
		{
			int indexBegin = rand()%(sz-count);
			for (int j=0; j<indexBegin; j++)
			{
				ipos++;
			}
		}	
		for(int i=0; i < n; i++)
		{
			ipos++;
			if (userId == ipos->second->GetUserId())
			{
				continue;
			}
			if (ipos != m_mmapUser[0].end())
			{
				ppUserList[index++] = ipos->second;
			}
		}
	}
	else
	{
		int sz = m_mmapUser[1].size();
		MMAP_USER::iterator ipos = m_mmapUser[1].begin();
		int n = sz>count ? count:sz;
		if(sz > count)
		{
			int indexBegin = rand()%(sz-count);
			for (int j=0; j<indexBegin; j++)
			{
				ipos++;
			}
		}
		for(int i=0; i < n; i++)
		{
			ipos++;
			if (userId == ipos->second->GetUserId())
			{
				continue;
			}
			if (ipos != m_mmapUser[1].end())
			{
				ppUserList[index++] = ipos->second;
			}
		}
	}
	//UnLock();

	return (unsigned short)index;
}

/*unsigned short CUserMgr::RandChallTaskList(unsigned int userId, CUser** ppUserList, int count)
{
	CUser* pUser = GetUser(userId);
	assert(pUser);
	unsigned short level = pUser->GetLevel();
	unsigned short levelBegin, levelEnd;
	if (level >= LEVLE_BIG_BEGIN)
	{
		if (levle > LEVLE_BIG_BEGIN + 5)
		{
			levelBegin = level -5;
		}
		else
		{
			levelBegin = 20;
		}
		levelEnd = levle+5 > MAX_LEVLE_SIZE ? MAX_LEVLE_SIZE : levle+5;
	}
	else
	{
		levelEnd = levle;
		levelBegin = levelBegin - 5 < 1 ? 1 : levelBegin - 5;
	}

	int sz = 0;
	for (unsigned short i=levelBegin; i<=levleEnd; i++)
	{
		sz += m_mmapUser.count(i);
	}

	Lock();
	int sz = m_UserMap.size();
	MUP_USER::iterator ipos = m_UserMap.begin();
	int n = sz>count ? count:sz;
	if(sz > count)
	{
		int index = rand()%(sz-count);
		for (int j=0; j<index; j++)
		{
			ipos++;
		}
	}
	int index = 0;
	for(int i=0; i < n; i++)
	{
		ipos++;
		if (userId == ipos->first)
		{
			continue;
		}
		if (ipos != m_UserMap.end())
		{
			ppUserList[index++] = ipos->second;
		}
	}
	UnLock();
	return (unsigned short)index;
}*/

int CUserMgr::CopyUserList(unsigned int userId, unsigned char type,  unsigned short count, char* buf, unsigned short *cnt)
{
	CUser* pUserList[MAX_CHALLENGE_SIZE];
	memset(pUserList, 0, sizeof(pUserList));
	int n = count > MAX_CHALLENGE_SIZE ? MAX_CHALLENGE_SIZE : count;
	int len = 0;

	*cnt = RandUserList(userId, pUserList, n+1);// 多取一个 以防取到自己
	*cnt = *cnt > n ? n : *cnt;
	for(int i=0; i < *cnt; i++)
	{
		len += pUserList[i]->FillSimpInfo(buf+len);
	}
	return len;
}

int CUserMgr::CopyChallTaskList(unsigned int userId, char* buf, unsigned short *cnt)
{
	int len = 0;
	CUser* pSelf = GetUser(userId);
	assert(pSelf);

	CUser* pTmpUser = NULL;
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = pSelf->GetSubChall(i);
		assert(pSub);
		pTmpUser = GetUser(pSub->m_info.roleId2);
		assert(pTmpUser);
		len += pTmpUser->FillChallRoleInfo(buf+len, pSub->m_info.flag);
	}
	*cnt = CHALL_TASK_SIZE;
	return len;
}

void CUserMgr::CreateChallTaskList(CUser* pUser)
{
	CUser* pUserList[MAX_CHALLENGE_SIZE];
	memset(pUserList, 0, sizeof(pUserList));
	unsigned short count = RandUserList(pUser->GetUserId(), pUserList, CHALL_TASK_SIZE+1);// 多取一个 以防取到自己
	assert(count >= CHALL_TASK_SIZE);
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = new CChallSubTask(pUser->GetUserId(), i, pUserList[i]->GetUserId());
		pUser->AddChallSubTask(pSub);
	}
}






#include "ChallTask.h"
#include "UserMgr.h"

CChallSubTask::CChallSubTask(unsigned int roleId, unsigned short subId, unsigned int roleId2)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.roleId = roleId;
	m_info.subTaskId = subId;
	m_info.roleId2 = roleId2;
	m_info.flag = 0;
	changeFlag = 0;
}

CChallSubTask::CChallSubTask(stChallTaskInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.roleId = pInfo->roleId;
	m_info.subTaskId = pInfo->subTaskId;
	m_info.roleId2 = pInfo->roleId2;
	m_info.flag = pInfo->flag;
	changeFlag = 0;
}

CChallSubTask::~CChallSubTask()
{
}
void CChallSubTask::Reset()
{
	m_info.roleId2 = 0;
	m_info.flag = 0;
	changeFlag = 1;
}

CChallTaskMgr::CChallTaskMgr()
{
	m_bFinishFlag = false;
	m_bGetRewardFlag = false;
	m_pUser = NULL;
}

CChallTaskMgr::~CChallTaskMgr()
{
	//
}

void CChallTaskMgr::Reset()
{
	Lock();
	MAP_CHALL_TASK::iterator ipos = m_mapChallSubTask.begin();
	MAP_CHALL_TASK::iterator iend = m_mapChallSubTask.end();
	for(;ipos != iend; ++ipos)
	{
		ipos->second->Reset();
		ipos->second->SetChangeFlag();
	}
	m_bFinishFlag = false;
	m_bGetRewardFlag = false;
	UnLock();
}

unsigned short CChallTaskMgr::SubTaskFinish(unsigned int roleId)
{
	Lock();
	MAP_CHALL_TASK::iterator ipos = m_mapChallSubTask.begin();
	MAP_CHALL_TASK::iterator iend = m_mapChallSubTask.end();
	CChallSubTask* pSub = NULL;
	unsigned short nRet = 0;
	for(;ipos != iend; ++ipos)
	{
		pSub = ipos->second;
		if (pSub->m_info.roleId2 == roleId)
		{
			pSub->m_info.flag = 1;
			pSub->SetChangeFlag();
		}
		if (1 == pSub->m_info.flag)
		{
			nRet++;
		}
		
	}
	UnLock();
	return nRet;
}

unsigned short CChallTaskMgr::GetFinishCount()
{
	Lock();
	MAP_CHALL_TASK::iterator ipos = m_mapChallSubTask.begin();
	MAP_CHALL_TASK::iterator iend = m_mapChallSubTask.end();
	unsigned short nRet = 0;
	for(;ipos != iend; ++ipos)
	{
		if (1 == ipos->second->m_info.flag)
		{
			nRet++;
		}
	}
	UnLock();
	return nRet;
}

short CChallTaskMgr::CopyChallTaskInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CChallSubTask* pObj = NULL;
	stChallTaskInfo* pInfo = NULL;
	Lock();
	MAP_CHALL_TASK::iterator ipos = m_mapChallSubTask.begin();
	MAP_CHALL_TASK::iterator iend = m_mapChallSubTask.end();
	while(ipos != iend)
	{
		pObj = (CChallSubTask*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stChallTaskInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->roleId = pObj->m_info.roleId;
			pInfo->subTaskId = pObj->m_info.subTaskId;
			pInfo->roleId2 = pObj->m_info.roleId2;
			pInfo->flag = pObj->m_info.flag;
			p += sizeof(stChallTaskInfo);
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

int CChallTaskMgr::Add(CChallSubTask* pSub)
{
	m_mapChallSubTask.insert(MAP_CHALL_TASK::value_type(pSub->m_info.subTaskId, pSub));
	return m_mapChallSubTask.size();
}

int CChallTaskMgr::Del(CChallSubTask* pSub)
{
	Lock();
	MAP_CHALL_TASK::iterator it = m_mapChallSubTask.find(pSub->m_info.roleId);
	if (it != m_mapChallSubTask.end())
	{
		m_mapChallSubTask.erase(it);
	}
	UnLock();
	return m_mapChallSubTask.size();
}

CChallSubTask* CChallTaskMgr::GetSubChall(unsigned short subId)
{
	CChallSubTask* pRet = NULL;
	//Lock();
	//MAP_CHALL_TASK::iterator ipos = m_mapChallSubTask.find(roleId);
	//if (ipos != m_mapChallSubTask.end())
	{
		pRet = m_mapChallSubTask[subId];
	}
	//UnLock();
	return pRet;
}

bool CChallTaskMgr::HasTask()
{
	unsigned short n = 0;
	//Lock();
	MAP_CHALL_TASK::iterator it = m_mapChallSubTask.begin();
	if (it != m_mapChallSubTask.end())
	{
		if(it->second->m_info.roleId2 > 0)
			n++;
	}
	//UnLock();
	return (n == CHALL_TASK_SIZE);
}

void CChallTaskMgr::ResetList(unsigned int userId)
{
	Lock();
	MAP_CHALL_TASK::iterator it = m_mapChallSubTask.begin();
	if (it != m_mapChallSubTask.end())
	{
		it->second->SetDelFlag();
	}
	UnLock();
	m_pUser->SaveChallTask();//通知DB删除数据
	m_mapChallSubTask.clear();

	CUser* pUserList[MAX_CHALLENGE_SIZE];
	memset(pUserList, 0, sizeof(pUserList));
	unsigned short count = g_UserMgr->RandUserList(userId, pUserList, CHALL_TASK_SIZE+1);// 多取一个 以防取到自己
	assert(count >= CHALL_TASK_SIZE);
	Lock();
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = new CChallSubTask(userId, i, pUserList[i]->GetUserId());
		pSub->SetAddFlag();
		m_mapChallSubTask.insert(MAP_CHALL_TASK::value_type(pSub->m_info.subTaskId, pSub));
	}
	UnLock();
	m_pUser->SaveChallTask();//通知DB 保存
}

int CChallTaskMgr::OnGetChallTaskList(unsigned int userId, char* buf, unsigned short *cnt)
{
	
	int len = 0;
	if (!HasTask())
	{
		ResetList(userId);
	}
	CUser* pSelf = g_UserMgr->GetUser(userId);
	assert(pSelf);
	CUser* pTmpUser = NULL;
	Lock();
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = m_mapChallSubTask[i];
		assert(pSub);
		pTmpUser = g_UserMgr->GetUser(pSub->m_info.roleId2);
		assert(pTmpUser);
		len += pTmpUser->FillChallRoleInfo(buf+len, pSub->m_info.flag);
	}
	UnLock();
	*cnt = CHALL_TASK_SIZE;
	
	return len;
}

int CChallTaskMgr::OnFlushChallTaskList(unsigned int userId, char* buf, unsigned short *cnt)
{
	ResetList(userId);
	/*CUser* pUserList[MAX_CHALLENGE_SIZE];
	memset(pUserList, 0, sizeof(pUserList));
	unsigned short count = g_UserMgr->RandUserList(userId, pUserList, CHALL_TASK_SIZE+1);// 多取一个 以防取到自己
	assert(count >= CHALL_TASK_SIZE);
	
	Lock();
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = new CChallSubTask(userId, i, pUserList[i]->GetUserId());
		pSub->SetAddFlag();
		m_mapChallSubTask.insert(MAP_CHALL_TASK::value_type(pSub->m_info.subTaskId, pSub));
	}*/
	int len = 0;
	CUser* pSelf = g_UserMgr->GetUser(userId);
	assert(pSelf);
	CUser* pTmpUser = NULL;
	for(int i=0; i < CHALL_TASK_SIZE; i++)
	{
		CChallSubTask* pSub = m_mapChallSubTask[i];
		assert(pSub);
		pTmpUser = g_UserMgr->GetUser(pSub->m_info.roleId2);
		assert(pTmpUser);
		len += pTmpUser->FillChallRoleInfo(buf+len, pSub->m_info.flag);
	}
	UnLock();
	*cnt = CHALL_TASK_SIZE;

	return len;
}



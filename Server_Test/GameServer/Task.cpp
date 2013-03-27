#include "Task.h"
#include "../Base/InternalMsg.h"

CTask::CTask()
{
	m_bAllLeafDone = false;
}

CTask::~CTask()
{
}

int CTask::FillMainTaskInfo(unsigned int nMainId, char* buf, unsigned short* count)
{
	Lock();
	MAP_TASKS::iterator ipos = m_muTask.lower_bound(nMainId);
	MAP_TASKS::iterator iend = m_muTask.upper_bound(nMainId);
	int offset = 0;
	t_TaskInfo* pInfo = NULL;
	int n = 0;
	while(ipos != iend)
	{
		++n;
		pInfo = ipos->second;
		memcpy(buf+offset, &(pInfo->leafId), sizeof(unsigned short));
		offset += sizeof(unsigned short);
		memcpy(buf+offset, &(pInfo->leftTimes), sizeof(unsigned char));
		offset += sizeof(unsigned char);
		++ipos;
	}
	UnLock();
	*count = n;
	return offset;
}

short CTask::CopyTaskInfo(unsigned int userId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;
	Lock();
	MAP_TASKS::iterator ipos = m_muTask.begin();
	MAP_TASKS::iterator iend = m_muTask.end();
	t_TaskInfo* pTask = NULL;
	stTaskInfo* pST = NULL;
	while(ipos != iend)
	{
		pTask = ipos->second;
		if( pTask->changeFlag > 0 )//
		{
			++count;
			pST = (stTaskInfo*)p;
			pST->userId = userId;
			pST->mainId = pTask->mainId;
			pST->leafId = pTask->leafId;
			pST->leftTimes = pTask->leftTimes;
			pST->changeFlag = pTask->changeFlag;
			p += sizeof(stTaskInfo);
			pTask->ResetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}

void CTask::AddTaskInfo(unsigned int userId, stMainMissionConfig* pMainTask)
{
	printf("CTask::AddTaskInfo\n");
	int size = pMainTask->GetLeafMissionCount();
	stLeafMissionConfig* pSubTask = NULL;
	Lock();
	for(int i=0; i<size; i++)
	{
		t_TaskInfo* pTask = NULL;
		pSubTask = pMainTask->GetLeafMissionByIndex(i);
		if (pSubTask)
		{
			pTask = new t_TaskInfo;
			pTask->mainId = (unsigned short)(pMainTask->iMainID);
			pTask->leafId = (unsigned short)(pSubTask->iID);
			pTask->leftTimes = (unsigned char)(pSubTask->iMaxfinish);
			pTask->SetFlag(1); //Ôö¼Ó
		}
		Insert(pTask);
	}
	UnLock();
}

void CTask::Insert(t_TaskInfo* pTask)
{
	m_muTask.insert(MAP_TASKS::value_type(pTask->mainId, pTask));
}

void CTask::RemoveTask(unsigned short mainId, unsigned short leafId)
{
	MAP_TASKS::iterator ipos = m_muTask.lower_bound(mainId);
	MAP_TASKS::iterator iend = m_muTask.upper_bound(mainId);
	t_TaskInfo* p = NULL;
	Lock();
	while(ipos != iend)
	{
		p = ipos->second;
		if( p->leafId == leafId )
		{
			m_muTask.erase(ipos);
		}
	}
	UnLock();
}

t_TaskInfo* CTask::GetTaskInfo(unsigned short mainId, unsigned short leafId)
{
	Lock();
	MAP_TASKS::iterator ipos = m_muTask.lower_bound(mainId);
	MAP_TASKS::iterator iend = m_muTask.upper_bound(mainId);
	t_TaskInfo* p = NULL;
	while(ipos != iend)
	{
		p = ipos->second;
		if(p->leafId == leafId)
		{
			break;
		}
		++ipos;
	}
	UnLock();
	return p;
}



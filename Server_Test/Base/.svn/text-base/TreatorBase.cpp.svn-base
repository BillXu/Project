#include "TreatorBase.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

bool ITreatorBase::Get(InternalMsg** ppInMsg)
{
	//printf("ITreatorBase::Get ...");
	Lock();
	if (m_msgQueue.empty())
	{
		UnLock();
		return false;
	}
	*ppInMsg = m_msgQueue.front();
	m_msgQueue.pop();
	/*if( pInMsgQueued )
	{
		memcpy( pInMsg, pInMsgQueued, sizeof( InternalMsg ) );
		delete pInMsgQueued;
	}*/
	UnLock();
	if(*ppInMsg)
	{
		//printf("...Pop msg(%d) %d : %d\n", m_msgQueue.size(), (*ppInMsg)->pMsg->cSysIdentifer, (*ppInMsg)->pMsg->cMsgID);
	}
	
	return (*ppInMsg != NULL);
}

void ITreatorBase::Push(InternalMsg* pInMsg)
{
	//printf("ITreatorBase::Push ...");
	Lock();
	m_msgQueue.push(pInMsg);
	UnLock();
	//printf("...Push msg(%d) %d : %d\n",m_msgQueue.size(), pInMsg->pMsg->cSysIdentifer, pInMsg->pMsg->cMsgID);
	int ret = m_semMsg.Post();
	if (ret < 0)
	{
		printf(" m_semMsg.Post errno = %d\n", errno);
	}
}


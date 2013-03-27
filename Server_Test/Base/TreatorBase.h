#ifndef   _LOGIN_TREATOR_BASE_H_
#define   _LOGIN_TREATOR_BASE_H_

#include <queue>
#include "../Share/mutex.h"
#include "InternalMsg.h"
#include "../Share/Sem.h"

class ITreatorBase
{
public:
	virtual ~ITreatorBase()
	{}
	bool Get(InternalMsg** ppInMsg);
	void Push(InternalMsg* pInMsg);
protected:
	void Lock()
	{
		m_mutex.Acquire();
	}
	void UnLock()
	{
		m_mutex.Release();
	}
	Mutex m_mutex;
	
	std::queue<InternalMsg* > m_msgQueue;
	CSemaphore m_semMsg;
};

#endif


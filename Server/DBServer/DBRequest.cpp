#include "DBRequest.h"
CDBRequestQueue::CDBRequestQueue()
{

}

CDBRequestQueue::~CDBRequestQueue()
{
	ClearAllRequest();
	ClearAllResult();
	ClearAllReserveRequest();
}

void CDBRequestQueue::PushRequest(stDBRequest* request )
{
	mRequestLock.Lock();
	m_vAllRequest.push_back(request) ;
	mRequestLock.Unlock() ;
}

void CDBRequestQueue::PushReserveRequest(stDBRequest* request )
{
	mReserveQuestLock.Lock() ;
	m_vReserveRequest.push_back(request);
	mReserveQuestLock.Unlock();
}
void CDBRequestQueue::PushResult(stDBResult* result )
{
	mResultLock.Lock();
	m_vAllResult.push_back(result);
	mResultLock.Unlock();
}

stDBRequest* CDBRequestQueue::GetReserveRequest()
{
	stDBRequest* pOut = NULL ;
	mReserveQuestLock.Lock();
	VEC_DBREQUEST::iterator iter = m_vReserveRequest.begin() ;
	if ( iter != m_vReserveRequest.end() )
	{
		pOut = *iter ;
		m_vReserveRequest.erase(iter) ;
	}
	mReserveQuestLock.Unlock();
	if ( pOut == NULL )
	{
		pOut = new stDBRequest ;
	}
	memset(pOut,0,sizeof(stDBRequest));
	return pOut ;
}
void CDBRequestQueue::GetAllRequest(VEC_DBREQUEST& vAllRequestOut )
{
	vAllRequestOut.clear();
	mRequestLock.Lock();
	vAllRequestOut.swap(m_vAllRequest);
	mRequestLock.Unlock();
}

void CDBRequestQueue::GetAllResult(VEC_DBRESULT& vAllReslutOut )
{
	vAllReslutOut.clear() ;
	mResultLock.Lock();
	vAllReslutOut.swap(m_vAllResult) ;
	mResultLock.Unlock() ;
}

void CDBRequestQueue::ClearAllRequest()
{
	mRequestLock.Lock();
	VEC_DBREQUEST::iterator iter = m_vAllRequest.begin() ;
	for ( ; iter != m_vAllRequest.end(); ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vAllRequest.clear() ;
	mRequestLock.Unlock();
}

void CDBRequestQueue::ClearAllResult()
{
	mResultLock.Lock() ;
	VEC_DBRESULT::iterator iter = m_vAllResult.begin() ;
	for ( ; iter != m_vAllResult.end(); ++iter)
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vAllResult.clear();
	mResultLock.Unlock() ;
}

void CDBRequestQueue::ClearAllReserveRequest()
{
	mReserveQuestLock.Lock();
	VEC_DBREQUEST::iterator iter = m_vReserveRequest.begin() ;
	for ( ; iter != m_vReserveRequest.end(); ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vReserveRequest.clear() ;
	mReserveQuestLock.Unlock() ;
}

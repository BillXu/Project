#include "LoginPeerMgr.h"
#include "LoginPeer.h"
CLoginPeerMgr::CLoginPeerMgr()
{
	ClearAll();
}

CLoginPeerMgr::~CLoginPeerMgr()
{
	ClearAll();
}

void CLoginPeerMgr::OnMessage(RakNet::Packet* pMsg )
{

}

CLoginPeer* CLoginPeerMgr::GetPeerBySessionID(unsigned int nSessionID )
{
	MAP_LOGIN_PEER::iterator iter = m_vAllPeers.find(nSessionID) ;
	if ( iter != m_vAllPeers.end())
	{
		return iter->second ;
	}
	return NULL ;
}

CLoginPeer* CLoginPeerMgr::GetReserverPeer()
{
	if ( m_vReseverLoginPeers.empty() )
	{
		return NULL ;
	}
	LIST_LOGIN_PEER::iterator iter = m_vReseverLoginPeers.begin() ;
	CLoginPeer* pPeer = *iter ;
	m_vReseverLoginPeers.erase(iter) ;
	return pPeer ;
}

bool CLoginPeerMgr::SendMsgToDB(const char* pBuffer , int nLen )
{
	// net work object 
	return true ;
}

bool CLoginPeerMgr::SendMsgToGate(const char* pBuffer , int nLen )
{
	// net work object
	return true ;
}

void CLoginPeerMgr::ClearAll()
{
	MAP_LOGIN_PEER::iterator iter = m_vAllPeers.begin() ;
	for ( ; iter != m_vAllPeers.end(); ++iter )
	{
		if ( iter->second )
		{
			delete iter->second ;
			iter->second = NULL ;
		}

	}
	m_vAllPeers.clear() ;

	LIST_LOGIN_PEER::iterator iter_list = m_vReseverLoginPeers.begin() ;
	for ( ; iter_list != m_vReseverLoginPeers.end(); ++iter_list )
	{
		if ( *iter_list )
		{
			delete *iter_list ;
		}
	}
	m_vReseverLoginPeers.clear() ;
}
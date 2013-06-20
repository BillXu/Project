#include "LoginPeerMgr.h"
#include "LoginPeer.h"
#include "LoginApp.h"
#include "MessageDefine.h"
#include "LogManager.h"
#include "ServerMessageDefine.h"
CLoginPeerMgr::CLoginPeerMgr(CLoginApp* app )
{
	ClearAll();
	m_pApp = app ;
}

CLoginPeerMgr::~CLoginPeerMgr()
{
	ClearAll();
}

void CLoginPeerMgr::OnMessage(RakNet::Packet* pMsg )
{
	CHECK_MSG_SIZE_VOID(stMsg,pMsg->length);
	
	stMsg* pRet = (stMsg*)pMsg->data;
	if ( pRet->usMsgType = MSG_DISCONNECT ) // a peer disconnected ;
	{
		CHECK_MSG_SIZE_VOID(stMsgPeerDisconnect,pMsg->length);
		stMsgPeerDisconnect* pReal = (stMsgPeerDisconnect*)pRet ;
		MAP_LOGIN_PEER::iterator iter = m_vAllPeers.find(pReal->nSessionID) ;
		if ( iter != m_vAllPeers.end() )
		{
			m_vReseverLoginPeers.push_back(iter->second) ;
		}
		CLogMgr::SharedLogMgr()->PrintLog("A peer disconnected !") ;
		return ;
	}

	if ( pRet->usMsgType == MSG_TRANSER_DATA )
	{
		CHECK_MSG_SIZE_VOID(stMsgTransferData,pMsg->length);
		stMsgTransferData* pReal = (stMsgTransferData*)pRet ;
		CLoginPeer* pPeer = GetPeerBySessionID(pReal->nSessionID);
		if ( !pPeer )
		{
			pPeer = GetReserverPeer();
			if ( !pPeer )
			{
				pPeer = new CLoginPeer(this) ;
				pPeer->Reset(pReal->nSessionID) ;
				m_vAllPeers[pReal->nSessionID] = pPeer ;
			}
			pPeer->Reset(pReal->nSessionID) ;
		}
		pPeer->OnMessage(pMsg) ;
	}
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
	m_pApp->SendMsg(pBuffer,nLen,false);
	return true ;
}

bool CLoginPeerMgr::SendMsgToGate(const char* pBuffer , int nLen )
{
	// net work object
	m_pApp->SendMsg(pBuffer,nLen,true);
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
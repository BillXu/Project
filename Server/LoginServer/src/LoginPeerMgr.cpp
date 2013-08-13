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
	if ( pRet->usMsgType == MSG_TRANSER_DATA )
	{
		CHECK_MSG_SIZE_VOID(stMsgTransferData,pMsg->length);
		stMsgTransferData* pReal = (stMsgTransferData*)pRet ;
		char* pbuffer = (char*)pReal ;
		pbuffer += sizeof(stMsgTransferData) ;
		OnGateMessage((stMsg*)pbuffer,pReal->nSessionID ) ;
		return ;
	}

	if ( pRet->cSysIdentifer == ID_MSG_DB2LOGIN )
	{
		OnDBMessage(pRet) ;
		return ;
	}

	CLogMgr::SharedLogMgr()->PrintLog("unprocessed msg type = %d",pRet->usMsgType ) ;
	return ;
}

void CLoginPeerMgr::OnGateMessage(stMsg* pmsg ,unsigned int nSessionID )
{
	switch ( pmsg->usMsgType )
	{
	case MSG_PLAYER_REGISTER:
		{
			stMsgRegister* pMsg = (stMsgRegister*)pmsg ;

			stMsgLoginRegister msgRegister ;
			int nLen = sizeof(msgRegister) + pMsg->nAccLen + pMsg->nPasswordLen;
			msgRegister.bAutoRegister = pMsg->bAutoQuickEnter ;
			msgRegister.cSex = pMsg->bSex ;
			msgRegister.pAcoundLen = pMsg->nAccLen ;
			msgRegister.ppPasswordLen = pMsg->nPasswordLen ;
			msgRegister.nSessionID = nSessionID ;
			msgRegister.ncharNameLen = pMsg->ncharNameLen ;
			
			int nOffset = 0 ;
			memcpy(m_pMaxBuffer + nOffset , &msgRegister, sizeof(msgRegister) );
			nOffset += sizeof(msgRegister);

			char* pbuffer = (char*)pmsg ;
			pbuffer += sizeof(stMsgRegister);

			memcpy(m_pMaxBuffer + nOffset , pbuffer, msgRegister.pAcoundLen) ;
			nOffset += msgRegister.pAcoundLen ;
			pbuffer += msgRegister.pAcoundLen ;

			memcpy(m_pMaxBuffer + nOffset , pbuffer, msgRegister.ppPasswordLen) ;
			nOffset += msgRegister.ppPasswordLen ;
			pbuffer += msgRegister.ppPasswordLen ;

			memcpy(m_pMaxBuffer + nOffset , pbuffer, msgRegister.ncharNameLen) ;
			nOffset += msgRegister.ncharNameLen ;
			pbuffer += msgRegister.ncharNameLen ;

			SendMsgToDB(m_pMaxBuffer, nOffset ) ;
		}
		break;
	case MSG_PLAYER_CHECK_ACCOUNT:
		{
			stMsgCheckAccount* pMsgCheck = (stMsgCheckAccount*)pmsg ;
			stMsgLoginAccountCheck msg ;
			msg.nSessionID = nSessionID ;
			msg.nAccountLen = pMsgCheck->nAccountLen ;
			msg.nPasswordLen = pMsgCheck->nPasswordlen ;
			char* pbuffer = (char*)pmsg ;
			pbuffer += sizeof(stMsgCheckAccount);
			int nOffset = 0 ;
			memcpy(m_pMaxBuffer + nOffset ,pbuffer, pMsgCheck->nAccountLen );
			nOffset += pMsgCheck->nAccountLen ;
			pbuffer += pMsgCheck->nAccountLen ;

			memcpy(m_pMaxBuffer + nOffset ,pbuffer, pMsgCheck->nPasswordlen );
			nOffset += pMsgCheck->nPasswordlen ;
			pbuffer += pMsgCheck->nPasswordlen ;
			SendMsgToDB(m_pMaxBuffer,nOffset) ;
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog("Unknown message from gate type = %d",pmsg->usMsgType );
			return ;
		}	   

	}
}

void CLoginPeerMgr::OnDBMessage(stMsg* pmsg )
{
	switch ( pmsg->usMsgType )
	{
	case MSG_PLAYER_REGISTER:
		{
			stMsgLoginRegisterRet* pMsgRet = (stMsgLoginRegisterRet*)pmsg ;
			stMsgTransferData msgData ;
			msgData.bBroadCast = false ;
			msgData.cSysIdentifer = ID_MSG_LOGIN2C ;
			msgData.nSessionID = pMsgRet->nSessionID ;
			
			stMsgRegisterRet msgclientRet ;
			msgclientRet.bAutoReigster = pMsgRet->bAuto;
			msgclientRet.nRet = pMsgRet->nRet ;
			msgclientRet.nUserID = pMsgRet->nUserID ;
		
			int nOffset = 0 ;
			memcpy(m_pMaxBuffer+ nOffset , &msgData, sizeof( msgData ) );
			nOffset += sizeof(msgData);
			
			memcpy(m_pMaxBuffer + nOffset, &msgclientRet , sizeof(msgclientRet) );
			nOffset += sizeof(msgclientRet);
			
			SendMsgToGate(m_pMaxBuffer, nOffset );
		}
		break;
	case MSG_PLAYER_CHECK_ACCOUNT:
		{
			stMsgLoginAccountCheckRet* pLoginCheckRet = (stMsgLoginAccountCheckRet*)pmsg ;
			
				
			stMsgCheckAccountRet msgcheckret ;
			msgcheckret.nRet = pLoginCheckRet->nRet ;
			msgcheckret.nUserID = pLoginCheckRet->nUserID ;
			
			stMsgTransferData msgData ;
			msgData.bBroadCast = false ;
			msgData.nSessionID = pLoginCheckRet->nSessionID ;
			msgData.cSysIdentifer = ID_MSG_LOGIN2C ;
			
			int nOffset = 0 ;
			memcpy(m_pMaxBuffer, &msgData, sizeof(msgData));
			nOffset += sizeof(msgData);

			memcpy(m_pMaxBuffer+ nOffset , &msgcheckret,sizeof(msgcheckret));
			nOffset += sizeof(msgcheckret) ;
			SendMsgToGate(m_pMaxBuffer, nOffset) ;
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog("Unknown message from DB type = %d",pmsg->usMsgType );
			return ;
		}	   

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
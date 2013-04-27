#include "Player.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
#include "PlayerManager.h"
CPlayer::CPlayer( unsigned int nUserUID )
{
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		 m_vAllComponents[i] = NULL;
	}
	Reset(nUserUID);
}

CPlayer::~CPlayer()
{
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
			delete p ;
		m_vAllComponents[i] = NULL ;
	}
}

void CPlayer::Reset( unsigned int nUserUID )
{
	m_nUserUID = nUserUID ;
	// inform components;
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->Reset();
		}
	}
}

void CPlayer::OnMessage(stMsg* pMsg )
{
	if ( pMsg->cSysIdentifer == ID_MSG_DB2GM )
	{
		ProcessDBMessage(pMsg) ;
	}
	else if ( pMsg->cSysIdentifer == ID_MSG_C2S )
	{
		ProcessLogicMessage(pMsg) ;
	}
	else 
	{
		CLogMgr::SharedLogMgr()->ErrorLog("GameServer Player can not process unknown SysIdentifer Msg = %d, type = %d",pMsg->cSysIdentifer , pMsg->usMsgType );
	}
}

void CPlayer::OnDisconnect()
{
	// inform components;
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->OnDisconnect();
		}
	}
}

void CPlayer::OnGateServerLost()
{
	// inform components;
	// inform components;
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->OnLostServer(true);
		}
	}
}

void CPlayer::OnDBServerLost()
{
	// inform components;
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->OnLostServer(false);
		}
	}
}

void CPlayer::SendMsgToClient(const char* pBuffer, unsigned short nLen,bool bBrocat )
{
	CPlayerManager::SharedPlayerMgr()->SendMsgToGateServer(GetUserUID(),pBuffer,nLen,bBrocat) ;
}

void CPlayer::SendMsgToDBServer( const char* pBuffer, unsigned short nLen )
{
	CPlayerManager::SharedPlayerMgr()->SendMsgToDBServer(pBuffer,nLen) ;
}

void CPlayer::ProcessLogicMessage(stMsg* pMsg )
{
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->OnMessage(pMsg) ;
		}
	}
}

void CPlayer::ProcessDBMessage(stMsg* pMsg )
{
	switch( pMsg->usMsgType )
	{
	case MSG_PUSH_BASE_DATA:
		{
			stMsgPushBaseDataToGameServer* pRealMsg = (stMsgPushBaseDataToGameServer*)pMsg ;
			memcpy(&m_stBaseData,&(pRealMsg->stData),sizeof(stBaseData));
			CLogMgr::SharedLogMgr()->PrintLog("name = %s",m_stBaseData.strName) ;
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Unprocessed Msg From DBServer id = %d " ,pMsg->usMsgType ) ;
		}
	}
}
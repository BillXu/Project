#include "Player.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
#include "PlayerManager.h"
#include "GameServerApp.h"
#include "PlayerBaseData.h"
#include "RoomPeer.h"
CPlayer::CPlayer( )
{
	m_nUserUID = 0 ;
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

void CPlayer::Init(unsigned int nUserUID, unsigned int nSessionID )
{
	m_nSessionID = nSessionID ;
	m_nUserUID = nUserUID ;
	/// new components ;here ;
	m_vAllComponents[ePlayerComponent_BaseData] = new CPlayerBaseData(this) ;
	m_vAllComponents[ePlayerComponent_RoomPeer] = new CRoomPeer(this) ;

	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->Init();
		}
	}
}

void CPlayer::Reset(unsigned int nUserUID, unsigned int nSessionID )
{
	m_nSessionID = nSessionID ;
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
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			if ( p->OnMessage(pMsg) )
			{
				return ;
			}
		}
	}

	CLogMgr::SharedLogMgr()->SystemLog("Unprocessed msg id = %d",pMsg->usMsgType) ;
}

void CPlayer::OnPlayerDisconnect()
{
	// inform components;
	for ( int i = ePlayerComponent_None; i < ePlayerComponent_Max ; ++i )
	{
		IPlayerComponent* p = m_vAllComponents[i] ;
		if ( p )
		{
			p->OnPlayerDisconnect();
		}
	}
}

void CPlayer::SendMsgToClient(const char* pBuffer, unsigned short nLen,bool bBrocat )
{
	CGameServerApp::SharedGameServerApp()->SendMsgToGateServer(GetSessionID(),pBuffer,nLen,bBrocat) ;
}

void CPlayer::SendMsgToDBServer( const char* pBuffer, unsigned short nLen )
{
	CGameServerApp::SharedGameServerApp()->SendMsgToDBServer(GetSessionID(),pBuffer,nLen) ;
}

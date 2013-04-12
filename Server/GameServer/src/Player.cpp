#include "Player.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
#include "PlayerManager.h"
CPlayer::CPlayer( unsigned int nUserUID )
{
	Reset(nUserUID);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Reset( unsigned int nUserUID )
{
	m_nUserUID = nUserUID ;
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

}

void CPlayer::OnGateServerLost()
{

}

void CPlayer::OnDBServerLost()
{

}

void CPlayer::SendMsgToClient(const char* pBuffer, unsigned short nLen )
{
	CPlayerManager::SharedPlayerMgr()->SendMsgToGateServer(GetUserUID(),pBuffer,nLen) ;
}

void CPlayer::SendMsgToDBServer( const char* pBuffer, unsigned short nLen )
{
	CPlayerManager::SharedPlayerMgr()->SendMsgToDBServer(pBuffer,nLen) ;
}

void CPlayer::ProcessLogicMessage(stMsg* pMsg )
{

}

void CPlayer::ProcessDBMessage(stMsg* pMsg )
{

}
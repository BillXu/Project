#include "RoomPeer.h"
#include "Player.h"
#include "Room.h"
#include "LogManager.h"
CRoomPeer::CRoomPeer(CPlayer* pPlayer )
	:IPlayerComponent(pPlayer)
{
	m_eType = ePlayerComponent_RoomPeer ;
	m_pRoom = NULL ;
	m_nPeerIdx = 0 ;
	Reset();
}

CRoomPeer::~CRoomPeer()
{

}

bool CRoomPeer::OnMessage(stMsg* pMsg )
{
	if ( pMsg->usMsgType > MSG_ROOM_MSG_BEGIN && pMsg->usMsgType < MSG_ROOM_MSG_END )
	{
		if ( m_pRoom )
		{
			m_pRoom->OnRoomMsg(pMsg,this) ;
		}
		else
		{
			CLogMgr::SharedLogMgr()->ErrorLog("Can n't do room action ,befor enter room ; name = %s",GetPlayerBaseData()->strName ) ;
		}
	}
	return true ;
}

void CRoomPeer::OnLostServer( bool bGate ) // gate or db server ;
{

}

void CRoomPeer::OnDisconnect()
{

}

void CRoomPeer::Reset()
{
	m_nTimesMoneyForPK = 1 ;
	m_nBetMoney = 0 ;
	m_eState = eState_Wait ;
	m_bViewdCard = false ;
	m_PeerCard.Reset();
}

stBaseData* CRoomPeer::GetPlayerBaseData()
{
	return GetPlayer()->GetBaseData() ;
}

void CRoomPeer::OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC )
{
	m_PeerCard.SetPeerCardByNumber(nCardA,nCardB,nCardC) ;
}

void CRoomPeer::OnEnterRoom(CRoom* pRoom )
{
	m_pRoom = pRoom ;
}

void CRoomPeer::OnExitRoom(CRoom* pRoom)
{
	m_pRoom = NULL ;
}

bool CRoomPeer::AddBet( int nBetMoney )
{
	m_nBetMoney += nBetMoney ;
	GetPlayerBaseData()->nCoin -= nBetMoney ;
	if ( GetPlayerBaseData()->nCoin >= 0 )
		return true ;
	GetPlayerBaseData()->nCoin = 0 ;
	return false ;
}
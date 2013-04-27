#include "RoomPeer.h"
#include "Player.h"
CRoomPeer::CRoomPeer(CPlayer* pPlayer )
	:IPlayerComponent(pPlayer)
{
	m_eType = ePlayerComponent_RoomPeer ;
	Reset();
}

CRoomPeer::~CRoomPeer()
{

}

bool CRoomPeer::OnMessage(stMsg* pMsg )
{
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
	m_pRoom = NULL ;
}

stBaseData* CRoomPeer::GetPlayerBaseData()
{
	return GetPlayer()->GetBaseData() ;
}

void CRoomPeer::OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC )
{
	m_PeerCard.SetPeerCardByNumber(nCardA,nCardB,nCardC) ;
}
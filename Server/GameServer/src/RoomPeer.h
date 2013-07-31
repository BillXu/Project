#pragma once 
#include "IPlayerComponent.h"
#include "PeerCard.h"
#include "BaseData.h"
#include "CommonDefine.h"
class CRoom ;
class CRoomPeer
	:public IPlayerComponent
{
public:
	CRoomPeer(CPlayer* pPlayer );
	~CRoomPeer();
	virtual bool OnMessage(stMsg* pMsg ) ;
	virtual void OnLostServer( bool bGate ); // gate or db server ;
	virtual void OnDisconnect();
	virtual void Reset();
	stBaseData* GetPlayerBaseData();
	eRoomPeerState GetState(){return m_eState ;}
	void OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC );
	void OnEnterRoom(CRoom* pRoom );
	void OnExitRoom(CRoom* pRoom);
	bool AddBet( int nBetMoney ) ;
protected:
	friend CRoom ;
protected:
	CPeerCard m_PeerCard ;
	unsigned char m_nTimesMoneyForPK ; // ���Ʒ����ı��� 
	unsigned int m_nBetMoney;
	eRoomPeerState m_eState ;
	CRoom* m_pRoom ;
	int m_nPeerIdx ;// index used in room ;
	bool m_bViewdCard ;
};
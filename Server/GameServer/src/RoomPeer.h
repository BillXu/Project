#pragma once 
#include "IPlayerComponent.h"
#include "PeerCard.h"
#include "BaseData.h"
class CRoom ;
class CRoomPeer
	:public IPlayerComponent
{
public:
	enum eRoomPeerState
	{
		eState_None,
		eState_Wait,
		eState_Ready,
		eState_Playing,
		eState_GiveUp,
		eState_Failed,
		eState_Win,
		eState_Max,
	};
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
protected:
	CPeerCard m_PeerCard ;
	unsigned char m_nTimesMoneyForPK ;
	unsigned int m_nBetMoney;
	eRoomPeerState m_eState ;
	CRoom* m_pRoom ;
};
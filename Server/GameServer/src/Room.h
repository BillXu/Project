#pragma once
#include "CommonDefine.h"
#include <string>
#include "MessageDefine.h"
#include "CardPoker.h"
#include "RoomBase.h"
class CRoomPeer ;
class CRoom
	:public CRoomBase
{
public:
	CRoom();
	~CRoom();
	virtual void Init( unsigned int nRoomID , unsigned char nMaxSeat );
	virtual unsigned char GetEmptySeatCount();
	virtual bool CanJoin(CRoomPeer* peer );
	virtual void AddPeer(CRoomPeer* peer ) = 0 ;
	virtual void Update(float fTimeElpas, unsigned int nTimerID );
	virtual bool OnPeerMsg(CRoomPeer* pPeer, stMsg* pmsg );
protected:
	void SwitchToRoomSate( eRoomState eFrom, eRoomState eToDest );
	void NextPlayerAction();
	char GetReadyPeerCount();
protected:
	static unsigned int s_RoomID ;
protected:
	float m_fRoomSateTick[eRoomState_Max] ;
	char m_nCurMainPeerIdx ;
	char m_nCurWaitPeerIdx ;
	short m_nRound ;
	CPoker m_Poker;
};
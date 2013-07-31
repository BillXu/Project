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
	char GetReadyPeerCount();
protected:
	static unsigned int s_RoomID ;
protected:
	float m_fRoomTime[eRoomState_Max] ;
};
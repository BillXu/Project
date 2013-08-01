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
	virtual void OnPlayerDisconnect();
	virtual void Reset();
	stBaseData* GetPlayerBaseData();
	eRoomPeerState GetState(){return m_eState ;}
	void OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC );
	void OnEnterRoom(CRoom* pRoom );
	void OnExitRoom(CRoom* pRoom);
	void OnWaitTimeOut();// please make a message fake ;
	bool IsActive(); // not fail ,not give, not null 
	unsigned int GetSessionID();
	unsigned int GetCoin();
	void GetBrifBaseInfo(stRoomPeerBrifInfo& vInfoOut );
	unsigned char GetPeerIdxInRoom(){ return m_nPeerIdx ; }
protected:
	friend CRoom ;
protected:
	CPeerCard m_PeerCard ;
	unsigned char m_nTimesMoneyForPK ; // ±ÈÅÆ·­±¶µÄ±¶Êý 
	unsigned int m_nBetCoin;
	eRoomPeerState m_eState ;
	CRoom* m_pRoom ;
	char m_nPeerIdx ;// index used in room ;
};
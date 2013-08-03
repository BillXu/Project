#pragma once 
#include "IPlayerComponent.h"
#include "PeerCard.h"
#include "PlayerBaseData.h"
#include "CommonDefine.h"
#include "MessageDefine.h"
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
	CPlayerBaseData* GetPlayerBaseData();
	eRoomPeerState GetState(){return m_eState ;}
	void OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC );
	void LeaveRoom();
	void OnWaitTimeOut();// please make a message fake ;
	bool IsActive(); // not fail ,not give, not null 
	unsigned int GetSessionID();
	unsigned int GetCoin();
	unsigned int  AddBetCoin( unsigned int naddBetCoin );  // return indeeed added betCoin ;
	void GetBrifBaseInfo(stRoomPeerBrifInfo& vInfoOut );
	unsigned char GetPeerIdxInRoom(){ return m_nPeerIdx ; }
	void OnWinCoin(unsigned int nWinCoin );
protected:
	friend CRoom ;
protected:
	CPeerCard m_PeerCard ;
	unsigned int m_nBetCoin;
	eRoomPeerState m_eState ;
	CRoom* m_pRoom ;
	char m_nPeerIdx ;// index used in room ;
};
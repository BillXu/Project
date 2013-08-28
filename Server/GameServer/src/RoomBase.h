#pragma once
#include "CommonDefine.h"
#include "Timer.h"
enum eRoomType
{
	eRoom_None,
	eRoom_Gold,
	eRoom_Max ,
};

class CRoomPeer ;
struct stMsg ;
class CRoomBase
	:public CTimerDelegate
{
public:
	CRoomBase();
	virtual ~CRoomBase();
	virtual void Init( unsigned int nRoomID , unsigned char nMaxSeat );
	void SendMsgRoomPeers(stMsg*pMsg ,unsigned short nLen );
	CRoomPeer* GetRoomPeerBySessionID( unsigned int nSessionID );
	virtual void OnPeerLeave( CRoomPeer* peer );
	int GetRoomPeerCount();
	unsigned char GetEmptySeatCount(){ return m_nMaxSeat - GetRoomPeerCount() ; }
	unsigned char GetMaxSeat(){ return m_nMaxSeat ; }
	void SetRoomID(unsigned int nRoomID ){ m_nRoomID = nRoomID ; }
	unsigned int GetRoomID(){ return m_nRoomID ; }
	eRoomType GetRoomType(){ return m_eRoomType ;}
	eRoomState GetRoomState(){ return m_eRoomState ;}

	virtual bool CanJoin(CRoomPeer* peer );
	virtual void AddPeer(CRoomPeer* peer ) = 0 ;
	virtual void Update(float fTimeElpas, unsigned int nTimerID );
	virtual bool OnPeerMsg(CRoomPeer* pPeer, stMsg* pmsg );

protected:
	char GetRoomPeerIdx(CRoomPeer* pPeer );
protected:
	eRoomType m_eRoomType ;
	unsigned int m_nRoomID ;
	CRoomPeer**m_vRoomPeer;
	eRoomState m_eRoomState ;
private:
	unsigned char m_nMaxSeat ;
};
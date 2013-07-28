#pragma once
#include <list>
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
	typedef std::list<CRoomPeer*> LIST_ROOM_PEER ;
public:
	CRoomBase();
	virtual ~CRoomBase(){}
	virtual void Init( unsigned int nRoomID , unsigned char nMaxSeat );
	void SendMsgRoomPeers(stMsg*pMsg ,unsigned short nLen );
	CRoomPeer* GetRoomPeerBySessionID( unsigned int nSessionID );
	void OnPeerLeave( CRoomPeer* peer );
	int GetRoomPeerCount(){ return m_vRoomPeer.size() ;}
	unsigned char GetEmptySeatCount(){ return m_nMaxSeat - GetRoomPeerCount() ; }
	unsigned char GetMaxSeat(){ return m_nMaxSeat ; }
	void SetMaxSeat( unsigned char nSeat ) { m_nMaxSeat = nSeat ; }
	void SetRoomID(unsigned int nRoomID ){ m_nRoomID = nRoomID ; }
	unsigned int GetRoomID(){ return m_nRoomID ; }
	eRoomType GetRoomType(){ return m_eRoomType ;}

	virtual bool CanJoin(CRoomPeer* peer );
	virtual void AddPeer(CRoomPeer* peer ) = 0 ;
	virtual void Update(float fTimeElpas, unsigned int nTimerID );
	virtual bool OnPeerMsg(CRoomPeer* pPeer, stMsg* pmsg );
protected:
	eRoomType m_eRoomType ;
	unsigned int m_nRoomID ;
	LIST_ROOM_PEER m_vRoomPeer ;
	unsigned char m_nMaxSeat ;
};
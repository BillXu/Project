#pragma once
#include "CommonDefine.h"
#include <string>
#include "MessageDefine.h"
#include "Timer.h"
class CRoomPeer ;
class CTimer ;
class CRoom
	:public CTimerDelegate
{
public:
	CRoom( unsigned int nRoomID );
	~CRoom();
	void Init();
	bool HasFlag(eRoomFlag eFlag );
	void SetFlag(eRoomFlag eFlag );
	void OnRoomMsg(stMsg* pMsg , CRoomPeer* pPeer );
	void SendMsgToRoomPeers( char* pbuffer ,unsigned short nLen ,CRoomPeer* pExcept );
	void TimerFunc(float fTimeElaps,unsigned int nTimerID );
	unsigned char RoomPeerCount();
	unsigned int GetRoomID();
	void SetRoomID(unsigned int nRoomId );
	void OnPlayerEnter( CRoomPeer* pPeerToEnter );
	void OnPlayerExit( CRoomPeer* pPeerToEnter );
protected:
	unsigned int m_nRoomID ;
	std::string m_strRoomName ;
	int m_nflag ;
	
	CRoomPeer* m_pAllPeers[MAX_ROOM_PEER] ;
	CTimer* m_pTimerToReady ;   // 等待所有玩家准备 .
	CTimer* m_pTimerToAction ;  // 等待某个玩家执行操作. 

	char m_nCurActionPeerIndex;
	char m_nMainPeerIndex ; // 本局庄家的索引.
	
	unsigned int m_nAllBetMoney ;
	char m_nRound ;
	unsigned int m_nSingleBetMoney ; 
};
#pragma once
#include "CommonDefine.h"
#include <string>
#include "MessageDefine.h"
#include "Timer.h"
#include "CardPoker.h"
class CRoomPeer ;
class CTimer ;
class CRoom
	:public CTimerDelegate
{
public:
	CRoom(int nBaseMoney);
	~CRoom();
	void Init();
	bool HasFlag(eRoomFlag eFlag );
	void SetFlag(eRoomFlag eFlag );
	void SetFinalFlag( int nFlag ){ m_nflag = nFlag ;}
	void OnRoomMsg(stMsg* pMsg , CRoomPeer* pPeer );
	void SendMsgToRoomPeers( char* pbuffer ,unsigned short nLen ,CRoomPeer* pExcept );
	void TimerFunc(float fTimeElaps,unsigned int nTimerID );
	unsigned char GetRoomPeerCount();
	unsigned int GetRoomID();
	void OnPlayerEnter( CRoomPeer* pPeerToEnter );
	void OnPlayerExit( CRoomPeer* pPeerToEnter );
	void OnProcessPlayerAction(stMsgActionCmd* pActMsg, CRoomPeer* pPeer );
	eRoomState GetRoomState(){ return m_eState ;}
	void SetRoomName(const char* pName ){ m_strRoomName = pName ;}
protected:
	unsigned char GetReadyPeerCount();
	void DistributeCard();
	bool CheckAllPlayerReady();
	bool NextActionPeerIdx();
	bool CanPlayerDoThisAction(eRoomPeerAction eAction , CRoomPeer* pPeer );
	void NextTurn();
	void FinishThisMatch(CRoomPeer* peerWin );
	void OnRestarMatch();
	void UpdateMainPlayerIndex();
protected:
	static unsigned int s_RoomID ;
protected:
	unsigned int m_nRoomID ;
	std::string m_strRoomName ;
	int m_nflag ;
	int m_nBaseBetMoney ;
	CPoker m_Poker ;
	
	CRoomPeer* m_pAllPeers[MAX_ROOM_PEER] ;
	CTimer* m_pTimerToReady ;   // 等待所有玩家准备 .
	CTimer* m_pTimerToAction ;  // 等待某个玩家执行操作. 
	CTimer* m_pTimerWaitDistribute; // 等待发牌动画结束；
	CTimer* m_pTimerWaitPK ; //   等待PK画面结束；
	CTimer* m_pTimerWaitFinish; // 等待结束画面；

	char m_nCurActionPeerIndex;
	char m_nMainPeerIndex ; // 本局庄家的索引.
	eRoomState m_eState ;
	
	unsigned int m_nAllBetMoney ;
	char m_nRound ;
	unsigned int m_nSingleBetMoney ; 
};
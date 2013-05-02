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
	CTimer* m_pTimerToReady ;   // �ȴ��������׼�� .
	CTimer* m_pTimerToAction ;  // �ȴ�ĳ�����ִ�в���. 

	char m_nCurActionPeerIndex;
	char m_nMainPeerIndex ; // ����ׯ�ҵ�����.
	
	unsigned int m_nAllBetMoney ;
	char m_nRound ;
	unsigned int m_nSingleBetMoney ; 
};
#pragma once 
#include "BaseData.h"
#include "IPlayerComponent.h"
struct stMsg ;
class CPlayer
{
public:
	CPlayer( unsigned int nUserUID );
	~CPlayer();
	void Reset( unsigned int nUserUID ) ; // for reuse the object ;
	void OnMessage(stMsg* pMsg );
	void OnDisconnect();
	void OnGateServerLost();
	void OnDBServerLost();
	void SendMsgToClient(const char* pBuffer, unsigned short nLen,bool bBrocat = false );
	void SendMsgToDBServer(const char* pBuffer, unsigned short nLen);
	unsigned int GetUserUID(){ return m_nUserUID ;}
	IPlayerComponent* GetComponent(ePlayerComponentType eType );
	stBaseData* GetBaseData(){ return &m_stBaseData ;}
protected:
	void ProcessLogicMessage(stMsg* pMsg );
	void ProcessDBMessage(stMsg* pMsg );
protected:
	unsigned int m_nUserUID ;
	stBaseData m_stBaseData ;
	IPlayerComponent* m_vAllComponents[ePlayerComponent_Max] ;
};
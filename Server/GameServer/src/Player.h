#pragma once 
#include "IPlayerComponent.h"
class CPlayerBaseData ;
struct stMsg ;
class CPlayer
{
public:
	CPlayer();
	~CPlayer();
	void Init( unsigned int nUserUID,unsigned int nSessionID );
	void Reset(unsigned int nUserUID,unsigned int nSessionID ) ; // for reuse the object ;
	void OnMessage(stMsg* pMsg );
	void OnPlayerDisconnect();
	void SendMsgToClient(const char* pBuffer, unsigned short nLen,bool bBrocat = false );
	void SendMsgToDBServer(const char* pBuffer, unsigned short nLen);
	unsigned int GetUserUID(){ return m_nUserUID ;}
	unsigned int GetSessionID(){ return m_nSessionID ;}
	IPlayerComponent* GetComponent(ePlayerComponentType eType ){ return m_vAllComponents[eType];}
	CPlayerBaseData* GetBaseData(){ return (CPlayerBaseData*)GetComponent(ePlayerComponent_BaseData);}
protected:
	unsigned int m_nUserUID ;
	unsigned int m_nSessionID ;  // comunicate with the client ;
	IPlayerComponent* m_vAllComponents[ePlayerComponent_Max] ;
};
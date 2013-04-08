#pragma once 
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
	void SendMsgToClient(const char* pBuffer, unsigned short nLen );
	void SendMsgToDBServer(const char* pBuffer, unsigned short nLen);
	unsigned int GetUserUID(){ return m_nUserUID ;}
protected:
	void ProcessLogicMessage(stMsg* pMsg );
	void ProcessDBMessage(stMsg* pMsg );
protected:
	unsigned int m_nUserUID ;
};
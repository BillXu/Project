#pragma once
struct stMsg ;
class CPlayer ;

enum ePlayerComponentType
{
	ePlayerComponent_None ,
	ePlayerComponent_BaseData,
	ePlayerComponent_RoomPeer,
	ePlayerComponent_Max,
};

class IPlayerComponent
{
public:
	IPlayerComponent(CPlayer* pPlayer );
	virtual ~IPlayerComponent();
	void SendMsgToClient(const char* pbuffer , unsigned short nLen , bool bBrocast = false );
	void SendMsgToDB(const char* pbuffer , unsigned short nLen);
	ePlayerComponentType GetComponentType(){ return m_eType ;}
	CPlayer* GetPlayer(){ return m_pPlayer ;}
	virtual bool OnMessage(stMsg* pMsg ) = 0 ;
	virtual void OnPlayerDisconnect(){}
	virtual void Reset(){}
	virtual void Init(){}
protected:
	CPlayer* m_pPlayer ;
	ePlayerComponentType m_eType ;	
};
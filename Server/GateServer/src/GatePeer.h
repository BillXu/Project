#pragma once
#include "RakNetTypes.h"
class CGatePeer
{
public:
	enum eGatePeerType
	{
		eGatePeer_None,
		eGatePeer_GameServer,
		eGatePeer_LoginServer,
		eGatePeer_Client,
		eGatePeer_Max,
	};
public:
	CGatePeer();
	virtual ~CGatePeer();
	virtual void Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	virtual void Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	virtual void OnMessage( RakNet::Packet* pData ) = 0 ;
	virtual void OnDisconnected();
	RakNet::RakNetGUID& GetSelfNetGUID(){ return m_nSelfNetGUID ; }
	unsigned int GetSessionID(){ return m_nSessionID ;}
	eGatePeerType GetPeerType(){ return m_ePeerType ;}
	bool IsRemove(){ return m_bRemove ;}
protected:
	unsigned int m_nSessionID ;
	RakNet::RakNetGUID m_nSelfNetGUID ;
	eGatePeerType m_ePeerType ;
	bool m_bRemove;
public:
	static char* s_pBuffer ;
};
#pragma once
#include "RakNetTypes.h"
class CLoginPeer
{
public:
	CLoginPeer();
	~CLoginPeer();
	void OnMessage(RakNet::Packet* packet );
	void Reset(unsigned int nSessionID );
	void 
protected:
	unsigned int m_nSessionID ;
};
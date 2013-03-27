#ifndef _HANDLER__H_
#define _HANDLER__H_

#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include <stdio.h>
#include <cstring>
#include "TreatorBase.h"
#include "IUserBase.h"

class CListenThread;
class CHandler
{
public:
	CHandler(RakNet::SystemAddress& peerID, RakNet::RakNetGUID guid);
	~CHandler();
public:
	void SetIOServer(RakNet::RakPeerInterface* pServer);
	void RegisterTreator(ITreatorBase* pTreator);
	void OnHandle(unsigned char* pData, unsigned int length);
	void OnDisconnect();
	void SetListenThread(CListenThread* const pListenThread);
	void SetUser(IUserBase* pUser);
	IUserBase* GetUser();
	CListenThread* GetListenThread()
	{ return m_pListenThread; }
	RakNet::SystemAddress GetClientID()
	{ return m_peerSystemAddress; }
	uint64_t GetGuid()
	{ return m_peerGuid.g; }
	uint32_t Send(const char *msg, const int length);
	//uint32_t Send(const char *msg);
private:
	RakNet::SystemAddress 		m_peerSystemAddress;
	RakNet::RakNetGUID			m_peerGuid;
	RakNet::RakPeerInterface* 	m_pIOServer;
	ITreatorBase*				m_pTreatorBase;
	CListenThread*				m_pListenThread;
	IUserBase*					m_pUserBase;
};
#endif


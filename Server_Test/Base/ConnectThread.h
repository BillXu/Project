#ifndef _CONNECT_THREAD__H_
#define _CONNECT_THREAD__H_

#include "ThreadMod.h"
#include "ListenThreadBase.h"
#include "Handler.h"
#include <map>

#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "RakNetTypes.h"

#include <unistd.h> // usleep

#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif

//#define MS_CONNECT_TIME_OUT		30000
//#define MS_UNRELIABLE_TIME_OUT	10000
//#define MS_BEFORE_SHUT_DOWN		300
//#define	MAX_CONNECTIONS			10000//

//template<class T>
class CConnectThread: public CThreadT//, public IListenThreadBase
{
public:
	/*static CConnectThread* _instance()
	{
		static CConnectThread* __inst = NULL;
		if(!__inst)
		{
			__inst = new CConnectThread;
		}

		return __inst;
	}*/
	CConnectThread();
	~CConnectThread();
public:
	void 	RegisterTreator(ITreatorBase* pTreator);
	void	SetServerAddr(const char* ip, int port);
	void 	SetClientPort(int port);
	bool 	Init();
	// We copy this from Multiplayer.cpp to keep things all in one file for this example
	unsigned char GetPacketIdentifier(RakNet::Packet *p);
	void 	__run();
	
	
	int OnRecv(RakNet::Packet* p);
	int OnConnect(RakNet::SystemAddress serverID, RakNet::RakNetGUID guid);
	CHandler* GetPeerHandler();
	CHandler* m_peerHandler;
private:
	ITreatorBase* m_pTreator;
	int		m_ServerPort;
	char	m_ServerIP[16];
	int 	m_ClientPort;
	bool	m_bRun;
	//std::map<RakNet::SystemAddress,CHandler*> m_mapHandler;
	
	RakNet::RakPeerInterface* 	m_peer;
	// GetPacketIdentifier returns this
	unsigned char 				m_packetIdentifier;
};

//#define ConnectThread ( CConnectThread::_instance() )

#endif


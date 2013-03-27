#ifndef _LISTEN_THREAD__H_
#define _LISTEN_THREAD__H_

#include "ThreadMod.h"
#include "ListenThreadBase.h"
#include "Handler.h"
#include <map>

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
#include "../Share/mutex.h"
//#include <cstring>
//#include <stdlib.h>
//#include "Kbhit.h"
//#include <stdio.h>
//#include <string.h>
//#include "Gets.h"

#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif

#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif

#ifdef _CONSOLE_2
_CONSOLE_2_SetSystemProcessParams
#endif

#define MS_CONNECT_TIME_OUT		30000*100
#define MS_UNRELIABLE_TIME_OUT	10000*100
#define MS_BEFORE_SHUT_DOWN		300
#define	MAX_CONNECTIONS			10000//

class CListenThread: public CThreadT//, public IListenThreadBase
{
public:
	/*static CListenThread* _instance()
	{
		static CListenThread* __inst = NULL;
		if(!__inst)
		{
			__inst = new CListenThread;
		}

		return __inst;
	}*/
	CListenThread();
	~CListenThread();
public:
	int	SetListenPort(int port)
	{
		assert(port > 1000);
		m_nListenPort = port;
		return m_nListenPort;
	}
	bool 	Init();
	// We copy this from Multiplayer.cpp to keep things all in one file for this example
	unsigned char GetPacketIdentifier(RakNet::Packet *p);
	void 	__run();
	
	void RegisterTreator(ITreatorBase* pTreator);
	CHandler* GetPeerHandler();
	
	int OnRecv(RakNet::Packet* p);
	int OnDisconnect(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid);
	int OnAccept(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid);
	CHandler* GetHandler(RakNet::SystemAddress& clientID);
	CHandler* GetHandler(uint64_t g);
	int RemoveHandler(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid);
private:
	int 	m_nListenPort;
	bool	m_bRun;
	std::map<RakNet::SystemAddress,CHandler*> m_mapHandler;
	std::map<RakNet::RakNetGUID,CHandler*> m_mapHandler2;
	ITreatorBase* m_pTreator;
	
	RakNet::RakPeerInterface* 	m_pServer;
	// GetPacketIdentifier returns this
	unsigned char 				m_packetIdentifier;
	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
	RakNet::SocketDescriptor m_socketDescriptors[2];
private:
	void Lock()
	{
		m_mutex.Acquire();
	}
	void UnLock()
	{
		m_mutex.Release();
	}
	Mutex m_mutex;
};

//#define ListenThread ( CListenThread::_instance() )

#endif


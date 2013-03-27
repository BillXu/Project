#include "ListenThread.h"

CListenThread::CListenThread()
{
	m_bRun = true;
	m_nListenPort = 0;
	m_pServer 	= NULL;
	m_pTreator = NULL;
}

CListenThread::~CListenThread()
{
}

bool CListenThread::Init()
{
	m_pServer=RakNet::RakPeerInterface::GetInstance();
	m_pServer->SetTimeoutTime(MS_CONNECT_TIME_OUT,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	
	m_socketDescriptors[0].port=m_nListenPort;
	m_socketDescriptors[0].socketFamily=AF_INET; // Test out IPV4
	//m_socketDescriptors[1].port=m_nListenPort;
	//m_socketDescriptors[1].socketFamily=AF_INET6; // Test out IPV6
	
	return true;
}

void CListenThread::__run()
{
	assert( Init() );
	
	bool b = m_pServer->Startup(MAX_CONNECTIONS, m_socketDescriptors, 1 )==RakNet::RAKNET_STARTED;
	m_pServer->SetMaximumIncomingConnections(MAX_CONNECTIONS);
	if (!b)
	{
		//printf("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

		// Try again, but leave out IPV6
		//b = m_pServer->Startup(MAX_CONNECTIONS, m_socketDescriptors, 1 )==RakNet::RAKNET_STARTED;
		//if (!b)
		{
			puts("Server failed to start.  Terminating.");
			assert(0);
		}
	}
	m_pServer->SetOccasionalPing(true);
	m_pServer->SetUnreliableTimeout(MS_UNRELIABLE_TIME_OUT);
	
	//DataStructures::List<RakNet::RakNetSmartPtr < RakNet::RakNetSocket> > sockets;
	//m_pServer->GetSockets(sockets);
	
	//char message[2048];
	RakNet::Packet* p = NULL;
	//RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		
	while( m_bRun )
	{
		// This sleep keeps RakNet responsive
		RakSleep(30);
		
		for (p=m_pServer->Receive(); p; m_pServer->DeallocatePacket(p), p=m_pServer->Receive())
		{
			// We got a packet, get the identifier with our handy function
			m_packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			//printf("m_packetIdentifier =%d", m_packetIdentifier);
			switch (m_packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));
				OnDisconnect(p->systemAddress, p->guid);
				break;
			case ID_NEW_INCOMING_CONNECTION:
				// Somebody connected.  We have their IP now
				printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				//clientID=; // Record the player ID of the client
				OnAccept(p->systemAddress, p->guid);
				break;

			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;

			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));
				OnDisconnect(p->systemAddress, p->guid);
				break;

			default:
				// The server knows the static data of all clients, so we can prefix the message
				// With the name data
				/*printf("%s\n", p->data);

				sprintf(message, "%s", p->data);
				m_pServer->Send(message, (const int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);*/
				OnRecv(p);
				break;
			}

		}
	}
	m_pServer->Shutdown(MS_BEFORE_SHUT_DOWN);
	RakNet::RakPeerInterface::DestroyInstance(m_pServer);
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char CListenThread::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char) p->data[0];
}

int CListenThread::OnRecv(RakNet::Packet* p)
{
	//printf("CListenThread::OnRecv\n");
	CHandler* handler = GetHandler(p->systemAddress);
	assert(handler);
	//handler->RegisterTreator(m_pTreator);
	handler->OnHandle(p->data, p->length);
	return 0;
}

int CListenThread::OnDisconnect(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid)
{
	printf("CListenThread::OnDisconnect\n");
	CHandler* handler = GetHandler(clientID);
	assert(handler);
	handler->OnDisconnect();
	RemoveHandler(clientID, guid);
	return 0;
}

int CListenThread::OnAccept(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid)
{
	CHandler* pHandler = GetHandler(clientID);
	if(!pHandler)
	{
		pHandler = new CHandler(clientID, guid);
		pHandler->SetIOServer(m_pServer);
		pHandler->SetListenThread(this);
		pHandler->RegisterTreator(m_pTreator);
		Lock();
		m_mapHandler[clientID] = pHandler;
		m_mapHandler2[guid] = pHandler;
		UnLock();
	}
	else
	{
		printf("info: CListenThread::OnAccept pHandler!=NULL\n");
	}
	
	return m_mapHandler.size();
}

CHandler* CListenThread::GetHandler(RakNet::SystemAddress& clientID)
{
	Lock();
	std::map<RakNet::SystemAddress,CHandler*>::iterator it = m_mapHandler.find(clientID);
	if(it == m_mapHandler.end())
	{
		UnLock();
		return NULL;
	}
	else
	{
		UnLock();
		return it->second;
	}
}

CHandler* CListenThread::GetHandler(uint64_t g)
{
	Lock();
	std::map<RakNet::RakNetGUID,CHandler*>::iterator it = m_mapHandler2.find(RakNet::RakNetGUID(g));
	if(it == m_mapHandler2.end())
	{
		UnLock();
		return NULL;
	}
	else
	{
		UnLock();
		return it->second;
	}
}

int CListenThread::RemoveHandler(RakNet::SystemAddress clientID, RakNet::RakNetGUID guid)
{
	Lock();
	/*std::map<RakNet::SystemAddress,CHandler*>::iterator it = m_mapHandler.find(clientID);
	if(it != m_mapHandler.end())
	{
		m_mapHandler.erase(it);
	}
	
	std::map<RakNet::RakNetGUID,CHandler*>::iterator it2 = m_mapHandler2.find(guid);
	if(it2 != m_mapHandler2.end())
	{
		CHandler* pHandler = it2->second;
		m_mapHandler2.erase(it2);
	
		if(pHandler) 
		{
			delete pHandler;
		}
	}*/
	UnLock();
	return m_mapHandler2.size();
}

void CListenThread::RegisterTreator(ITreatorBase* pTreator)
{
	assert(pTreator);
	m_pTreator = pTreator;
}




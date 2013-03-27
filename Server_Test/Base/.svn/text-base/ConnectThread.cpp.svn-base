#include "ConnectThread.h"

CConnectThread::CConnectThread()
{
	m_bRun = true;
	m_ServerPort = 0;
	memset(m_ServerIP, 0, sizeof(m_ServerIP));
	m_ClientPort = 0;
	m_pTreator = NULL;
	m_peer 	= NULL;
	m_peerHandler = NULL;
}

CConnectThread::~CConnectThread()
{
}

void CConnectThread::SetServerAddr(const char* ip, int port)
{
	assert(ip && port>0);
	strcpy(m_ServerIP, ip);
	m_ServerPort = port;
}

void CConnectThread::SetClientPort(int port)
{
	assert(port>0);
	m_ClientPort = port;
}

bool CConnectThread::Init()
{
	m_peer=RakNet::RakPeerInterface::GetInstance();
	m_peer->AllowConnectionResponseIPMigration(false);
	
	RakNet::SocketDescriptor socketDescriptor(m_ClientPort,0);
	socketDescriptor.socketFamily=AF_INET;
	m_peer->Startup(1,&socketDescriptor, 1);
	m_peer->SetOccasionalPing(true);
	
	RakNet::ConnectionAttemptResult car = m_peer->Connect(m_ServerIP, m_ServerPort, NULL, 0);
	RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);
	printf("CConnectThread::Init CONNECTION_ATTEMPT_STARTED\n ");
	return true;
}

void CConnectThread::__run()
{
	assert( Init() );
	
	RakNet::Packet* p = NULL;
	//RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;		
	while( m_bRun )
	{
		// This sleep keeps RakNet responsive
		usleep(30*1000);
		
		for (p=m_peer->Receive(); p; m_peer->DeallocatePacket(p), p=m_peer->Receive())
		{
			// We got a packet, get the identifier with our handy function
			m_packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (m_packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %llu\n", p->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n"); 
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				// This tells the client they have connected
				//printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				//printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
				OnConnect(p->systemAddress, p->guid);
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;
			default:
				// It's a client, so just show the message
				//printf("recv %d bytes\n", p->length);
				OnRecv(p);
				break;
			}

		}
	}
	// Be nice and let the server know we quit.
	m_peer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_peer);
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char CConnectThread::GetPacketIdentifier(RakNet::Packet *p)
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

int CConnectThread::OnRecv(RakNet::Packet* p)
{
	//printf("CConnectThread::OnRecv\n");
	//m_peerHandler->RegisterTreator(m_pTreator);
	m_peerHandler->OnHandle(p->data, p->length);
	return 0;
}

int CConnectThread::OnConnect(RakNet::SystemAddress serverID, RakNet::RakNetGUID guid)
{
	m_peerHandler = new CHandler(serverID, guid);
	m_peerHandler->SetIOServer(m_peer);
	m_peerHandler->RegisterTreator(m_pTreator);
	return 1;
}

CHandler* CConnectThread::GetPeerHandler()
{
	return m_peerHandler;
}

void CConnectThread::RegisterTreator(ITreatorBase* pTreator)
{
	assert(pTreator);
	m_pTreator = pTreator;
}





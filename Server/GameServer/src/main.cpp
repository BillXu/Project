#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include <iostream>
int main()
{
	RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port = 0;
	client->Startup(1, &socketDescriptor, 1 );
	bool rs = (client->Connect("127.0.0.1", 3000, NULL, 0, 0)== RakNet::CONNECTION_ATTEMPT_STARTED);

	getchar();
	static unsigned int count = 1 ;
	while ( 1 )
	{
		unsigned char pbuffer[5];
		pbuffer[0] = ID_USER_PACKET_ENUM ;
		memcpy(&pbuffer[1],&count,sizeof(int)); 
		client->Send((const char*)pbuffer,sizeof(pbuffer),IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		++count ;
		Sleep(10);
	}
	client->Shutdown(1);
	getchar();
	return 0 ;
}
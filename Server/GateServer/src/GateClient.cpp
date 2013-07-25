#include "GateClient.h"
stGateClient::stGateClient()
{
	nSessionId = 0 ;
	nNetWorkID = RakNet::UNASSIGNED_RAKNET_GUID ;
	pTimerForReconnect = NULL ;
}
void stGateClient::Reset(unsigned int nSessionID , RakNet::RakNetGUID& nNetWorkID )
{
	this->nSessionId = nSessionID ;
	this->nNetWorkID = nNetWorkID ;
	if ( pTimerForReconnect )
	{
		pTimerForReconnect->Stop();
	}
}

void stGateClient::TimeUpForReconnect(float fTimeElaps,unsigned int nTimerID)
{

}

void stGateClient::StartWaitForReconnect()
{

}

void stGateClient::SetNewWorkID( RakNet::RakNetGUID& nNetWorkID)
{
	this->nNetWorkID = nNetWorkID ;
}
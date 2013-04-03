#include "DBPlayer.h"
CDBPlayer::CDBPlayer(RakNet::RakNetGUID& nFromGameServerGUID )
{

}

CDBPlayer::~CDBPlayer()
{

}

void CDBPlayer::SetFromServerGUID(RakNet::RakNetGUID& nFromGameServerGUID)
{

}

RakNet::RakNetGUID& CDBPlayer::GetFromGameServerGUID()
{
	return m_nFromGUID;
}

void CDBPlayer::OnDBResultAndProcessMsg(stDBResult* pResult )
{

}

void CDBPlayer::OnDisconnected()
{

}

void CDBPlayer::OnConnected()
{

}

unsigned int CDBPlayer::GetUserUID()
{
	return 0 ;
}
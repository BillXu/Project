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

void CDBPlayer::OnDBResult(stDBResult* pResult )
{

}

void CDBPlayer::OnDisconnected()
{

}

void CDBPlayer::OnConnected()
{

}

void CDBPlayer::OnPassAcountCheck( unsigned int nUserUID)
{

}

unsigned int CDBPlayer::GetUserUID()
{
	return 0 ;
}
#include "DBPlayer.h"
#include <assert.h>
CDBPlayer::CDBPlayer(RakNet::RakNetGUID& nFromGameServerGUID )
	:m_eState(ePlayerState_None),m_nFromGUID(nFromGameServerGUID),m_nUserUID(0)
{
	
}

CDBPlayer::~CDBPlayer()
{

}

void CDBPlayer::SetFromServerGUID(RakNet::RakNetGUID& nFromGameServerGUID)
{
	m_nFromGUID = nFromGameServerGUID ;
}

RakNet::RakNetGUID& CDBPlayer::GetFromGameServerGUID()
{
	return m_nFromGUID;
}

void CDBPlayer::OnDBResult(stDBResult* pResult )
{

}

void CDBPlayer::OnMessage(stMsg* pMsg )
{
	switch ( pMsg->usMsgType )
	{
	default:
		break; ;
	}
}

void CDBPlayer::OnDisconnected()
{
	m_eState = ePlayerState_Resever ;
	// save to db all data ;
	SaveAllToDB();
	// start Timer to delete self ;
}

void CDBPlayer::OnConnected()
{

}

void CDBPlayer::OnPassAcountCheck(unsigned int nUserUID, unsigned int nTempUID )
{
	m_nUserUID = nUserUID ;
	m_nTempUID = nTempUID ;
	if ( m_eState = ePlayerState_Resever )
	{
		m_eState = ePlayerState_Active ;
		SendBaseInfo();
	}
	else
	{
		ReadAllFromDB();
	}
}

unsigned int CDBPlayer::GetUserUID()
{
	return m_nUserUID ;
}

void CDBPlayer::ReadAllFromDB()
{
	m_eState = ePlayerState_ReadingData;
}

void CDBPlayer::SaveAllToDB()
{

}

void CDBPlayer::SendBaseInfo()
{
	assert(m_eState == ePlayerState_Active) ;
	// send message to gameserver ;
}

#include "DBPlayerManager.h"
#include "DBPlayer.h"
#include "DBRequest.h"
CDBPlayerManager::CDBPlayerManager()
{
	ClearAll();
}

CDBPlayerManager::~CDBPlayerManager()
{
	ClearAll();
}

bool CDBPlayerManager::OnMessage( RakNet::Packet* pData )
{
	return false ;
}

void CDBPlayerManager::OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
{

}

void CDBPlayerManager::OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
{

}

void CDBPlayerManager::ProcessDBResults()
{
	CDBRequestQueue::VEC_DBRESULT vResultOut ;
	CDBRequestQueue::SharedDBRequestQueue()->GetAllResult(vResultOut) ;
	CDBRequestQueue::VEC_DBRESULT::iterator iter = vResultOut.begin() ;
	for ( ; iter != vResultOut.end(); ++iter )
	{
		stDBResult* pRet = *iter ;
		OnProcessDBResult(pRet);
		delete pRet ;
	}
	vResultOut.clear();
}

CDBPlayer* CDBPlayerManager::GetPlayer( unsigned int nUID , ePlayersType eType )
{
	if ( eType < ePlayerType_None || eType >= ePlayerType_Max )
		return NULL ;
	LIST_DBPLAYER& vlist = m_vPlayers[eType] ;
	LIST_DBPLAYER::iterator iter = vlist.begin();
	CDBPlayer* pPlayer = NULL ;
	for ( ; iter != vlist.end(); ++iter )
	{
		pPlayer = *iter ;
		if ( pPlayer && pPlayer->GetUserUID() == nUID )
			return pPlayer ;
	}
	return NULL ;
}

void CDBPlayerManager::OnProcessDBResult(stDBResult* pResult )
{

}

void CDBPlayerManager::RemoveDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer )
{
	if ( !pPlayer )
		return ;
	LIST_DBPLAYER::iterator iter = vPlayers.begin();
	for ( ; iter != vPlayers.end(); ++iter )
	{
		if ( *iter == pPlayer )
		{
			vPlayers.erase(iter) ;
			return ;
		}
	}
}

void CDBPlayerManager::DeleteDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer)
{
	LIST_DBPLAYER::iterator iter = vPlayers.begin() ;
	for ( ; iter != vPlayers.end(); ++iter )
	{
		if ( pPlayer == NULL )
		{
			delete *iter ;
			*iter = NULL ;
			continue; 
		}
		
		if ( *iter == pPlayer )
		{
			delete *iter ;
			*iter = NULL ;
			vPlayers.erase(iter) ;
			return ;
		}
	}

	if ( pPlayer == NULL )
		vPlayers.clear() ;
}

void CDBPlayerManager::ClearAll()
{
	for ( int i = ePlayerType_None ; i < ePlayerType_Max ; ++i )
	{
		DeleteDBPlayer(m_vPlayers[i],NULL ) ;
	}
}


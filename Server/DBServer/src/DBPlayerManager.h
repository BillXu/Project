#pragma once
#include "ServerNetwork.h"
#include <list>
class stDBResult;
class CDBPlayer;
class CDBPlayerManager
:public CServerNetworkDelegate
{
public:
	enum ePlayersType
	{
		ePlayerType_None,
		ePlayerType_Active = ePlayerType_None,
		ePlayerType_Reserve,
		ePlayerType_WaitForCheck,
		ePlayerType_Max,
	};
	typedef std::list<CDBPlayer*> LIST_DBPLAYER ;
public:
	CDBPlayerManager();
	~CDBPlayerManager();

	// network delegate ;
	virtual bool OnMessage( RakNet::Packet* pData );
	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData );
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData );

	void ProcessDBResults();

	CDBPlayer* GetPlayer( unsigned int nUID , ePlayersType eType );
protected:
	void OnProcessDBResult(stDBResult* pResult );
	void RemoveDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer );
	void DeleteDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer);
	void ClearAll();
protected:
	LIST_DBPLAYER m_vPlayers[ePlayerType_Max] ;
};
#pragma once
#include "ServerNetwork.h"
#include <map>
class stDBResult;
class CDBPlayer;
class stMsg ;
class CDBPlayerManager
:public CServerNetworkDelegate
{
public:
	//enum ePlayersType
	//{
	//	ePlayerType_None,
	//	ePlayerType_Active = ePlayerType_None,
	//	ePlayerType_Reserve,
	//	ePlayerType_WaitForCheck,
	//	ePlayerType_Max,
	//};
	struct stAccountCheckAndRegister 
	{
		RakNet::RakNetGUID nFromServerID ;
		unsigned int nTempUsrUID ;
		std::string strAccount ;
		std::string strPassword ;
		std::string strCharacterName ;
		bool bCheck ;
	};

	typedef std::map<unsigned int,CDBPlayer*> MAP_DBPLAYER ;
	typedef std::map<unsigned int ,stAccountCheckAndRegister*> MAP_ACCOUNT_CHECK_REGISTER ;
public:
	CDBPlayerManager();
	~CDBPlayerManager();

	// network delegate ;
	virtual bool OnMessage( RakNet::Packet* pData );
	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData );
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData );

	void ProcessDBResults();

	CDBPlayer* GetPlayer( unsigned int nUID);
protected:
	void ProcessTransferedMsg( stMsg* pMsg ,unsigned int nTargetUserUID , RakNet::RakNetGUID& nFromNetUID );
	void OnProcessAccountCheckResult(stDBResult* pResult);
	void OnProcessRegisterResult(stDBResult* pResult);
	void OnProcessDBResult(stDBResult* pResult );
	//void RemoveDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer );
	//void DeleteDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer);
	void ClearAllPlayers();
	void ClearAccountCheck();
public:
	static char* s_gBuffer ;
protected:
	MAP_DBPLAYER m_vPlayers ;
	MAP_ACCOUNT_CHECK_REGISTER m_vAccountChecks ;
};
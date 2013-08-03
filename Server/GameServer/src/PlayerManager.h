#pragma once 
#include "NetWorkManager.h"
#include <map>
#include <list>
class CPlayer ;
struct stMsg ;
class CPlayerManager
{
public:
	typedef std::map<unsigned int, CPlayer*> MAP_PLAYERS ;  // nsessionID , player ;
	typedef std::list<CPlayer*> LIST_PLAYERS ;
public:
	CPlayerManager();
	~CPlayerManager();
	bool OnMessage( RakNet::Packet* pMsg );
	CPlayer* GetPlayerByUserUID( unsigned int nUserUID );
	CPlayer* GetPlayerBySessionID(unsigned int nSessionID );
protected:
	bool PreProcessLogicMessage( CPlayer*pPlayer ,stMsg* pmsg , unsigned int nSessionID );
	void PushReserverPlayers( CPlayer* ) ;
	CPlayer* GetReserverPlayer();
	void RemovePlayer(CPlayer*);
	void AddPlayer(CPlayer*);
protected:
	// logic data ;
	MAP_PLAYERS m_vAllActivePlayers ;
	LIST_PLAYERS m_vAllReservePlayers ;
};
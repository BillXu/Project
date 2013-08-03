#include "RoomManager.h"
#include "Player.h"
#include "Room.h"
#include "LogManager.h"
CBigRoom::CBigRoom()
{

}

CBigRoom::~CBigRoom()
{
	for ( int i = 0 ; i < MAX_ROOM_LEVEL ; ++i )
	{
		MAP_ROOM& vMapRoom = m_vAllRoom[i] ;
		MAP_ROOM::iterator iter = vMapRoom.begin() ;
		for ( ; iter != vMapRoom.end(); ++iter )
		{
			delete iter->second ;
			iter->second = NULL ;
		}
		vMapRoom.clear() ;
	}
}

void CBigRoom::Init( stRoomArgument* pArg, int nFlag )
{
	m_nFlag = nFlag ;
	memcpy(m_vRoomArg,pArg,sizeof(stRoomArgument) * MAX_ROOM_LEVEL);

	for ( int i = 0 ; i < MAX_ROOM_LEVEL ; ++i )
	{
		int nCount = 10 ;
		while ( nCount--)
		{
			CRoom* room = CreateRoom(m_vRoomArg[i],m_nFlag) ;
			m_vAllRoom[i].insert(std::make_pair(room->GetRoomID(),room)) ;
		}
	}
}

bool CBigRoom::CanEnterRoomByLevel( unsigned char nLevel, CPlayer* pPlayer )
{
	if ( pPlayer == NULL )
		return false ;
	if ( nLevel < 0 || nLevel >= MAX_ROOM_LEVEL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("illegal Room Level: CanEnterRoomByLevel");
		return false ;
	}
	stRoomArgument& pArg = m_vRoomArg[nLevel] ;
	if ((unsigned int)pArg.nDiamondRequiredToEnter > pPlayer->GetBaseData()->nDiamoned )
		return false ;
	if ( pArg.nMoneyRequiredToEnter > pPlayer->GetBaseData()->nCoin )
		return false ;
	if ( pArg.nTitleRequiredToEnter > pPlayer->GetBaseData()->nTitle )
		return false ;
	return true ;
}

CRoom* CBigRoom::GetRoomForEnter(unsigned char nLevel )
{
	if ( nLevel < 0 || nLevel >= MAX_ROOM_LEVEL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("illegal Room Level: GetRoomForEnter");
		return NULL ;
	}
	MAP_ROOM& vMapRoom = m_vAllRoom[nLevel] ;
	MAP_ROOM::iterator iter = vMapRoom.begin();
	for ( ; iter != vMapRoom.end() ; ++iter )
	{
		if ( iter->second->GetRoomPeerCount() < MAX_ROOM_PEER )
			return iter->second ;
	}
	
	CRoom* pRoom = CreateRoom(m_vRoomArg[nLevel],m_nFlag) ;
	vMapRoom.insert(std::make_pair(pRoom->GetRoomID(),pRoom)) ;
	return pRoom ;
}

CRoom* CBigRoom::GetRoomByID(int nRoomID )
{
	for ( int i = 0 ; i < MAX_ROOM_LEVEL ; ++i )
	{
		MAP_ROOM& vMapRoom = m_vAllRoom[i] ;
		MAP_ROOM::iterator iter = vMapRoom.begin() ;
		for ( ; iter != vMapRoom.end(); ++iter )
		{
			  if ( nRoomID == iter->second->GetRoomID())
				  return iter->second ;
		}
	}
	return NULL ;
}

int CBigRoom::GetActivePlayerCount(unsigned char nLevel )
{
	if ( nLevel < 0 || nLevel >= MAX_ROOM_LEVEL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("illegal Room Level: GetActivePlayerCount");
		return 0 ;
	}
	MAP_ROOM& vMapRoom = m_vAllRoom[nLevel] ;
	MAP_ROOM::iterator iter = vMapRoom.begin() ;
	int nCount = 0 ;
	for ( ; iter != vMapRoom.end(); ++iter )
	{
		if ( iter->second )
			nCount += iter->second->GetRoomPeerCount();
	}
	return nCount ;
}

int CBigRoom::GetAllActivePlayerCount()
{
	int nCount = 0 ;
	for ( int i = 0 ; i < MAX_ROOM_LEVEL ; ++i )
	{
		nCount += GetActivePlayerCount(i) ;
	}
	return nCount ;
}

CRoom* CBigRoom::CreateRoom(stRoomArgument& pArg, int nFlag )
{

}

// room manager 
CRoomManager::CRoomManager()
{

}

CRoomManager::~CRoomManager()
{
	CBigRoom::MAP_ROOM::iterator iter = m_vVipRooms.begin() ;
	for ( ; iter != m_vVipRooms.end(); ++iter )
	{
		delete iter->second ;
		iter->second ;
	}
	m_vVipRooms.clear();
}

void CRoomManager::Init()
{
	for ( int i = eRoomType_None; i < eRoomType_Max; ++i )
	{
		CBigRoom::stRoomArgument vArg[MAX_ROOM_LEVEL] ;
		for ( int j = 0 ; j < MAX_ROOM_LEVEL ; ++i )
		{
			vArg[j].nBaseBet = 1000 ;
			vArg[j].nDiamondRequiredToEnter = 0 ;
			vArg[j].nMoneyRequiredToEnter = 2000 ;
			vArg[j].nTitleRequiredToEnter = 0 ;
		}
		int nFlag = (1<<eRoomFlag_ShowCard)|(1<<eRoomFlag_TimesPK) | ( 1 << eRoomFlag_ChangeCard ) ;
		m_vAllBigRoom[i].Init(vArg,nFlag );
	}
	m_vVipRooms.clear() ;
}

CRoom* CRoomManager::GetRoomByID( int nRoomID, eBigRoomType eType )
{
	if ( eType < eRoomType_Max )
	{
		return m_vAllBigRoom[eType].GetRoomByID(nRoomID);
	}
	else
	{
		for ( int i = eRoomType_None; i < eRoomType_Max; ++i )
		{
			CRoom* pRoom = m_vAllBigRoom[i].GetRoomByID(nRoomID);
			if ( pRoom )
				return pRoom ;
		}
	}
	return NULL ;
}

CRoom* CRoomManager::GetVipRoom(int nRoomID )
{
	CBigRoom::MAP_ROOM::iterator iter = m_vVipRooms.begin() ;
	for ( ; iter != m_vVipRooms.end(); ++iter )
	{
		if ( iter->second && iter->second->GetRoomID() == nRoomID )
			return iter->second ;
	}
	return NULL ;
}

bool CRoomManager::CanEnterRoom(eBigRoomType eRoomType , unsigned char nLevel , CPlayer* pPlayer )
{
	if ( eRoomType >= eRoomType_Max )
		return false ;
	if ( nLevel >= MAX_ROOM_LEVEL )
		return false  ;
	return m_vAllBigRoom[eRoomType].CanEnterRoomByLevel(nLevel,pPlayer) ;
}

CRoom* CRoomManager::GetRoomToEnter(eBigRoomType eRoomType , unsigned char nLevel )
{
	if ( eRoomType >= eRoomType_Max )
		return NULL ;
	if ( nLevel >= MAX_ROOM_LEVEL )
		return NULL  ;
	return m_vAllBigRoom[eRoomType].GetRoomForEnter(nLevel) ;
}

CRoom* CRoomManager::CreateVipRoom(const char* pName ,int nBaseMoney )
{
	int nFlag = (1<<eRoomFlag_ShowCard)|(1<<eRoomFlag_TimesPK) | ( 1 << eRoomFlag_ChangeCard ) ;
	CRoom* pRoom = new CRoom(nBaseMoney);
	pRoom->Init();
	pRoom->SetFinalFlag(nFlag);
	pRoom->SetRoomName(pName) ;
	return pRoom ;
}

int CRoomManager::GetAllActivePlayerCount()
{
	int nCount = 0 ;
	for ( int i = 0 ; i < eRoomType_Max ; ++i )
	{
		nCount += GetActivePlayerCountByRoomType((eBigRoomType)i);
	}
	return nCount ;
}

int CRoomManager::GetActivePlayerCountByRoomType(eBigRoomType eType )
{
	if ( eType >= eRoomType_Max )
		return 0 ;
	return m_vAllBigRoom[eType].GetAllActivePlayerCount();
}

int CRoomManager::GetAcitvePlayerCountBySingleRoom(eBigRoomType eType , unsigned char nLevel )
{
	if ( eType >= eRoomType_Max )
		return 0 ;
	if ( nLevel >= MAX_ROOM_LEVEL)
		return 0 ;
	return m_vAllBigRoom[eType].GetActivePlayerCount(nLevel);
}

int CRoomManager::GetPlayerCountInVipRoom()
{
	int nCount = 0 ;
	CBigRoom::MAP_ROOM::iterator iter = m_vVipRooms.begin() ;
	for ( ; iter != m_vVipRooms.end(); ++iter )
	{
		nCount += iter->second->GetRoomPeerCount() ;
	}
	return nCount ;
}
#pragma once
#include "CommonDefine.h"
#include <map>
class CPlayer ;
class CRoom;
class CBigRoom
{
public:
	typedef std::map<int,CRoom*> MAP_ROOM ;
	struct stRoomArgument
	{
		int nBaseBet;
		int nMoneyRequiredToEnter ;
		int nDiamondRequiredToEnter ;
		unsigned char nTitleRequiredToEnter ;
	};
public:
	CBigRoom();
	~CBigRoom();
	void Init( stRoomArgument* pArg, int nFlag );
	bool CanEnterRoomByLevel( unsigned char nLevel, CPlayer* pPlayer );
	CRoom* GetRoomForEnter(unsigned char nLevel );
	CRoom* GetRoomByID(int nRoomID );

	int GetActivePlayerCount(unsigned char nLevel );
	int GetAllActivePlayerCount();
protected:
	CRoom* CreateRoom(stRoomArgument& pArg, int nFlag );
protected:
	int m_nFlag ;
	MAP_ROOM m_vAllRoom[MAX_ROOM_LEVEL] ;
	stRoomArgument m_vRoomArg[MAX_ROOM_LEVEL];
};

class CRoomManager
{
public:
	CRoomManager();
	~CRoomManager();
	void Init();
	CRoom* GetRoomByID( int nRoomID, eBigRoomType eType = eRoomType_Max);
	CRoom* GetVipRoom(int nRoomID );
	bool CanEnterRoom(eBigRoomType eRoomType , unsigned char nLevel , CPlayer* pPlayer );
	CRoom* GetRoomToEnter(eBigRoomType eRoomType , unsigned char nLevel );
	CRoom* CreateVipRoom(const char* pName ,int nBaseMoney );

	int GetAllActivePlayerCount();
	int GetActivePlayerCountByRoomType(eBigRoomType eType );
	int GetAcitvePlayerCountBySingleRoom(eBigRoomType eType , unsigned char nLevel );
	int GetPlayerCountInVipRoom();
protected:
	CBigRoom m_vAllBigRoom[eRoomType_Max] ;
	CBigRoom::MAP_ROOM m_vVipRooms ;
};
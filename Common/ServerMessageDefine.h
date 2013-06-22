#pragma once
#pragma pack(push)
#pragma pack(1)
// Define message , used between Servers ;mainly DBServer and GameServer 
#include "MessageDefine.h"
#include "BaseData.h"
#define DBServer_PORT 5001
// WARNNING:变长字符串，我们不包括终结符 \0 ;

//--------------------------------------------
// message between DB and Gamesever ;
struct stMsgGM2DB
:public stMsg
{
	stMsgGM2DB(){ cSysIdentifer = ID_MSG_GM2DB, usMsgType = MSG_NONE ; }
public:
	unsigned int nTargetUserUID;
};

struct stMsgDB2GM
:public stMsg
{
	stMsgDB2GM(){ cSysIdentifer = ID_MSG_DB2GM, usMsgType = MSG_NONE ; }
	unsigned int nTargetUserUID;
};

struct stMsgPushBaseDataToGameServer
	:stMsgDB2GM
{
public:
	stMsgPushBaseDataToGameServer(){usMsgType = MSG_PUSH_BASE_DATA ;}
	stBaseData stData ;
};

//-----------------------------------------------
// message between GateServer and Gamesever ;
struct stMsgPeerDisconnect
	:public stMsg
{
	stMsgPeerDisconnect()
	{
		cSysIdentifer = ID_MSG_GA2GM ;
		usMsgType = MSG_DISCONNECT;
	}
	unsigned int nSessionID ;
	
};

struct stMsgTransferData
	:public stMsg
{
	stMsgTransferData()
	{
		cSysIdentifer = ID_MSG_GA2GM ;
		usMsgType = MSG_TRANSER_DATA;
		bBroadCast = false ;
		pData = 0 ;
	}
	unsigned int nSessionID ;
	bool bBroadCast ;
	char* pData ;
};

#pragma pack(pop)//

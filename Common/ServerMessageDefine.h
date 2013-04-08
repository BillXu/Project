#pragma once
// Define message , used between Servers ;mainly DBServer and GameServer 
#include "MessageDefine.h"
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

struct stMsgGM2DBLoginCheck
:public stMsgGM2DB
{
	char* pAccount ;
	char* pPassword ;
	unsigned short nAccountLen ;
	unsigned short nPasswordLen ;
public:
	stMsgGM2DBLoginCheck(){ usMsgType = MSG_LOGIN_CHECK ; }
};

struct stMsgDB2GMLoginCheck
:public stMsgDB2GM
{
	short nRetFlag ;  // 0 success , 1 account don't exsit , 2 password error ;
public:
	stMsgDB2GMLoginCheck(){ usMsgType = MSG_LOGIN_CHECK ; }
};

//-----------------------------------------------
// message between GateServer and Gamesever ;
struct stMsg2GMPeerDisconnect
	:public stMsg
{
	stMsg2GMPeerDisconnect()
	{
		cSysIdentifer = ID_MSG_GA2GM ;
		usMsgType = MSG_DISCONNECT;
	}
	unsigned int nPeerUID ;
	
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
	unsigned int nTargetPeerUID ;
	bool bBroadCast ;
	char* pData ;
};

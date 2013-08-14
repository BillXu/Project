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
	unsigned int nSessionID;
};

struct stMsgDB2GM
	:public stMsg
{
	stMsgDB2GM(){ cSysIdentifer = ID_MSG_DB2GM, usMsgType = MSG_NONE ; }
	unsigned int nSessionID;
};
//
//struct stMsgPushBaseDataToGameServer
//	:stMsgDB2GM
//{
//public:
//	stMsgPushBaseDataToGameServer(){usMsgType = MSG_PUSH_BASE_DATA ;}
//	stBaseData stData ;
//};

//-----------------------------------------------
// message between GateServer and Gamesever ;
//struct stMsgPeerDisconnect
//	:public stMsg
//{
//	stMsgPeerDisconnect()
//	{
//		cSysIdentifer = ID_MSG_GA2GM ;
//		usMsgType = MSG_DISCONNECT;
//	}
//	unsigned int nSessionID ;
//	
//};

struct stMsgLoginRegister
	:public stMsg
{
	stMsgLoginRegister(){ cSysIdentifer = ID_MSG_LOGIN2DB; usMsgType = MSG_PLAYER_REGISTER ; }
	unsigned int nSessionID ;
	bool bAutoRegister ;
	unsigned char cSex ;
	char* pAccound ;
	unsigned char pAcoundLen ;
	char* pPassword ;
	unsigned char ppPasswordLen ;
	char* pcharactername ;
	unsigned char ncharNameLen ;
};

struct stMsgLoginRegisterRet
	:public stMsg 
{
	stMsgLoginRegisterRet(){ cSysIdentifer = ID_MSG_DB2LOGIN ; usMsgType = MSG_PLAYER_REGISTER ; }
	unsigned char nRet ;  // 0 means ok ;
	unsigned int nSessionID ;
	bool bAuto ;
	unsigned int nUserID ;
};


struct stMsgLoginAccountCheck
	:public stMsg
{
public:
	stMsgLoginAccountCheck(){ cSysIdentifer = ID_MSG_LOGIN2DB ; usMsgType = MSG_PLAYER_CHECK_ACCOUNT ; }
	unsigned int nSessionID ;
	char* pAccount ;
	unsigned char nAccountLen ;
	char * pPassword ;
	unsigned char nPasswordLen ;
};

struct stMsgLoginAccountCheckRet
	:public stMsg
{
public:
	stMsgLoginAccountCheckRet(){ cSysIdentifer = ID_MSG_DB2LOGIN ; usMsgType = MSG_PLAYER_CHECK_ACCOUNT ; }
	unsigned char nRet ;  // 0 means ok  1 .account error , 2 password error ;
	unsigned int nSessionID ;
	unsigned int nUserID ;
};

// game and db 
struct stMsgGameServerGetBaseData
	:public stMsgGM2DB
{
	stMsgGameServerGetBaseData(){cSysIdentifer = ID_MSG_GM2DB ; usMsgType = MSG_PLAYER_BASE_DATA ; }
	unsigned int nUserUID ;
};

struct stMsgGameServerGetBaseDataRet
	:public stMsgDB2GM
{
	stMsgGameServerGetBaseDataRet(){cSysIdentifer = ID_MSG_DB2GM ; usMsgType = MSG_PLAYER_BASE_DATA ; }
	int nCoin ;
	unsigned int nDiamoned ;
	unsigned char nTitle ;
	unsigned char nVipLevel ; 
	unsigned char nSex ;
	unsigned short nDefaulPhotoID ;
	int nUserDefinePhotoID ;
	int nYeastodayWinCoin ;
	int nSingleWinMost;
	unsigned short nWinTimes;
	unsigned short nLoseTimes ;
	int nQQNumber ;
	char* strName;
	unsigned char nNameLen ;
	char* strSigure;
	unsigned char nSigureLen ;
};

struct stMsgClientDisconnect
	:public stMsg
{
public:
	stMsgClientDisconnect(){ usMsgType = MSG_DISCONNECT_CLIENT ; }
	unsigned int nSeesionID ;
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

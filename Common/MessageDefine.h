#pragma once 
#pragma pack(push)
#pragma pack(1)
// define message struct , used between Server and Client ;
#include "MessageIdentifer.h"
#include "CommonDefine.h"
// WARNNING:变长字符串，我们不包括终结符 \0 ;
struct stMsg
{
	unsigned char cSysIdentifer ;
	unsigned short usMsgType ;
public:
	stMsg():cSysIdentifer( ID_USER_PACKET_ENUM  ),usMsgType(MSG_NONE){}
};

struct stMsgRegister
	:public stMsg
{
	stMsgRegister(){cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_REGISTE ;}
	char* pAccount;
	char* pPassword;
	char* pCharacterName;
	unsigned char nAccountLen ;
	unsigned char nPaswordLen ;
	unsigned char nCharacterNameLen ;
	unsigned char nAccountType ;  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .   
} ;

struct stMsgRegisterRet
	:public stMsg
{
	stMsgRegisterRet(){ cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_REGISTE ; }
	unsigned char nAccountType ;  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .   
	bool bSuccess ; // 0 , success 
	char nErrCode ; // 1 , accound repeated ;
	char* pAccount;  // used when visitor register
	//char* pPasswrold; // used when visitor register
	unsigned char nAccountLen ; // used when visitor register
	//unsigned char nPaswordLen ; // used when visitor register  // Password = pAccount ;
};

struct stMsgConnectRet
	:public stMsg
{
public:
	stMsgConnectRet(){ cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_CONNECT_RET ; }
	bool bOk ;
	unsigned char nErr ; // 1 ; no proper server ;
};

struct stMsgLogin
	:public stMsg
{
	stMsgLogin(){ cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_LOGIN; }
	char* pAccount;
	char* pPassword;
	unsigned char nAccountLen ;
	unsigned char nPaswordLen ;
};

struct stMsgLoginRet
	:public stMsg
{
public:
	stMsgLoginRet()
	{
		cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_LOGIN;
	}
	bool bOk ; 
	unsigned char nRetFlag ;  // 1 account don't exsit , 2 password error ;
};

// room message ;
struct stMsgRoomEnter
	:public stMsg 
{
public:
	stMsgRoomEnter(){cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_ROOM_ENTER ;}
};

struct stMsgRoomEnterRet
	:public stMsg 
{
	stMsgRoomEnterRet(){cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ROOM_ENTER ;}
	unsigned char nRet ; // 0 ; success ; 1 condition do not meet ;
};

struct stRoomPeerBrifData
{
	unsigned int nTempUID ;
	char pName[MAX_LEN_CHARACTER_NAME] ;
	unsigned int nAllMoney ;
	unsigned int nBetMoney ;
	unsigned short nTitle; 
	unsigned char nPeerState ; // eRoomPeerState
	unsigned char nIdx ;
	unsigned char nTimesMoneyForPK ; // 比牌翻倍的倍数
	unsigned char nShowCard[2] ;
	unsigned char nSwitchedCard[3] ;
};

struct stMsgRoomInfo
	:public stMsg 
{
public:
	stMsgRoomInfo(){ cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ROOM_INFO ; }
	unsigned int nRoomID ;
	unsigned int nAllBetMoney ;
	unsigned int nSingleBetMoney ;
	unsigned char m_nRound ;
	unsigned char nRoomState ;  // eRoomState ;
	unsigned char nMainPlayerIdx ;
	unsigned char nCurPlayerIdx;
	unsigned int n_RunWaitSecond ;
	stRoomPeerBrifData* pAllPeer ;
	unsigned char peerCount ;
};

struct stMsgRoomActionCmd
	:public stMsg
{
public:
	stMsgRoomActionCmd(){cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_ROOM_ACTION ;}
	unsigned char nActionType ;  // eRoomPeerAction 
};

struct stMsgRoomActionRet
	:public stMsg
{
public:
	stMsgRoomActionRet(){cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_ROOM_ACTION_RET ; }
	unsigned char nErr ; 
};


#pragma pack(pop)
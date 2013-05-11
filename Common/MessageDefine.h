#pragma once 
#pragma pack(push)
#pragma pack(1)
// define message struct , used between Server and Client ;
#include "MessageIdentifer.h"
#include "CommonDefine.h"
// WARNNING:�䳤�ַ��������ǲ������ս�� \0 ;
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
	stMsgRoomEnter(){cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_ENTER ;}
	bool bVip ;
	unsigned char nBigRoomType ;  // eBigRoomType 
	unsigned char nRoomLevel ;
	int nVipRoomID ;
};

struct stMsgRoomEnterRet
	:public stMsg 
{
	stMsgRoomEnterRet(){cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ENTER ;}
	unsigned char nRet ; // 0 ; success ; 1 condition do not meet ; 2 room don't exsit ; 3 room is full ;
};

struct stRoomPeerBrifData
{
	unsigned int nUserUID ;
	char pName[MAX_LEN_CHARACTER_NAME] ;
	unsigned int nAllMoney ;
	unsigned int nBetMoney ;
	unsigned short nTitle; 
	unsigned char nPeerState ; // eRoomPeerState
	unsigned char nIdx ;
	unsigned char nTimesMoneyForPK ; // ���Ʒ����ı���
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
	unsigned char nRound ;
	unsigned char nRoomState ;  // eRoomState ;
	unsigned char nMainPlayerIdx ;
	unsigned char nCurPlayerIdx;
	unsigned int nRunWaitSecond ;
	stRoomPeerBrifData* pAllPeer ;
	unsigned char peerCount ;
};

struct stMsgRoomPlayerEnter
	:public stMsg
{
public:
	stMsgRoomPlayerEnter(){cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ROOM_PLAYER_ENTER  ;}
	unsigned int pPlayerUID ;
	unsigned char nPeerIdx ;
	unsigned int nAllMoney ;
	unsigned int nTitle ;
};

struct stMsgRoomPlayerExit
	:public stMsg
{
public:
	stMsgRoomPlayerExit(){ cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ROOM_PLAYER_EXIT ;}
	unsigned nRoomIdx ;
};

struct stMsgActionCmd
	:public stMsg
{
public:
	stMsgActionCmd(){cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_ACTION ;}
	unsigned char nActionType ;  // eRoomPeerAction 
	unsigned int nArgument ; // meanning depond on ActionType ;
};

struct stMsgActionRet
	:public stMsg
{
public:
	stMsgActionRet(){cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_ACTION_RET ; }
	unsigned char nErr ;  // 1 room state is not suiteable ; 2  it is not your turn , can not do this action ; 3 . argument error, 4 .cuurent room have no that flag ;
};

struct stMsgShowCard
	:public stMsgActionCmd
{
	unsigned char vShowCard[PEER_CARD_COUNT-1] ;
	stMsgShowCard()
	{
		for( int i = 0 ; i < PEER_CARD_COUNT -1 ; ++i )
		{
			vShowCard[i] = 0 ;
		}
	}
};

struct stMsgActionSpeak
	:public stMsgActionCmd
{
	bool bDefault ; // default or user input;
	bool bText ; // default text id  or face id ;  id stored in nArgument ;
	char* pContent ; // len stored in nArgument 
};

struct stMsgRoomPlayerCmd
	:public stMsg
{
public:
	stMsgRoomPlayerCmd(){cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_ROOM_PLAYER_ACTION ; }
	unsigned char nPeerIdx ;
	unsigned char nActionType ;  // eRoomPeerAction 
};

struct stMsgRoomPlayerFollow
	:public stMsgRoomPlayerCmd
{
	unsigned int nRoomAllBetMoney ;
	unsigned int nPlayerLeftMoney;
	unsigned int nPlayerAllBetMoney ;
};

struct stMsgRoomPlayerAdd
	:public stMsgRoomPlayerFollow
{
	unsigned int nRoomSingleBetMoney ;
};

struct stMsgRoomPlayerPK
	:public stMsgRoomPlayerFollow
{
	unsigned char uTargetIdx ;
	bool bWin ;
};

struct stMsgRoomPlayerShowCard
	:public stMsgRoomPlayerCmd
{
	unsigned char vShowCard[PEER_CARD_COUNT-1] ;
	stMsgRoomPlayerShowCard()
	{
		for ( int i = 0 ; i < PEER_CARD_COUNT -1 ; ++i )
		{
			vShowCard[i] = 0 ;
		}
	}
};

struct stMsgRoomPlayerTimesPk
	:public stMsgRoomPlayerCmd
{
	unsigned char nTimesPK ;
};

struct stMsgRoomPlayerSpeakDefault
	:public stMsgRoomPlayerCmd
{
	bool bText ; // default text id  or face id ;
	unsigned short nDefaultID  ; // text id or face id ;
};

struct stMsgRoomPlayerSpeakText
	:public stMsgRoomPlayerCmd
{
	unsigned short nTextLen ;
	char* pContent ;
};

struct stMsgRoomResult
	:public stMsg
{
public:
	stMsgRoomResult(){cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_ROOM_RESULT ;}
	unsigned char nWinPlayerIdx ;
	unsigned int nWinMoney ;
};

struct stGetCardPeer
	:public stMsg
{
public:
	stGetCardPeer(){cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_ROOM_DISTRIBUTE_CARD ;}
	unsigned char nPeerIdx ;
	unsigned char vCard[PEER_CARD_COUNT];
};

struct stMsgRoomPlayerActionTurn
	:public stMsg
{
public:
	stMsgRoomPlayerActionTurn(){cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_ROOM_PLAYER_ACTION_TURN ;}
	unsigned char nPlayerIdx ;
};
struct stMsgRoomDistributeCard
	:public stMsg
{
public:
	stMsgRoomDistributeCard()
	{
		cSysIdentifer = ID_MSG_S2C ;
		usMsgType = MSG_ROOM_DISTRIBUTE_CARD ;
	}
	unsigned char nMainPlayerIndex ;
	unsigned char nCount ;
	stGetCardPeer* pGetCarPeer ;
};


#pragma pack(pop)
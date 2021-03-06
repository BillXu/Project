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
	stMsg():cSysIdentifer( ID_MSG_C2S  ),usMsgType(MSG_NONE){}
};

// register an login ;
struct stMsgRegister
	:public stMsg
{
	stMsgRegister(){cSysIdentifer = ID_MSG_C2LOGIN ; usMsgType = MSG_PLAYER_REGISTER ; }
	bool bAutoQuickEnter ; 
	char* paccount ;
	unsigned char nAccLen ;
	char* pPassword ;
	unsigned char nPasswordLen ;
	unsigned char bSex ; // eSex ;
	char* pcharactername ;
	unsigned char ncharNameLen ;
};

struct stMsgRegisterRet
	:public stMsg
{
	stMsgRegisterRet()
	{
		cSysIdentifer = ID_MSG_LOGIN2C ;
		usMsgType = MSG_PLAYER_REGISTER ;
	}
	char nRet ; // 0 success ;  1 . account have exsit ;
	bool bAutoReigster; 
	unsigned int nUserID ;
};

struct stMsgCheckAccount
	:public stMsg 
{
	stMsgCheckAccount(){ cSysIdentifer = ID_MSG_C2LOGIN ; usMsgType = MSG_PLAYER_CHECK_ACCOUNT ; }
	char*pAccount ;
	unsigned char nAccountLen ;
	char *pPassword ;
	unsigned char nPasswordlen ;
};

struct stMsgCheckAccountRet
	:public stMsg 
{
	stMsgCheckAccountRet(){ cSysIdentifer = ID_MSG_LOGIN2C; usMsgType = MSG_PLAYER_CHECK_ACCOUNT ; }
	unsigned char nRet ; // 0 ; success ; 1 account error , 2 password error ;
	unsigned int nUserID ;
};

// client reconnect ;
struct stMsgReconnect
	:public stMsg
{
public:
	stMsgReconnect(){cSysIdentifer = ID_MSG_C2GATE ; usMsgType = MSG_RECONNECT ; }
public:
	unsigned int nSessionID ;
};

struct stMsgReconnectRet
	:public stMsg
{
public:
	stMsgReconnectRet(){cSysIdentifer = ID_MSG_GATE2C ; usMsgType = MSG_RECONNECT ; }
public:
	char nRet; // 0 : success , 1 failed ;
};

struct stMsgServerDisconnect
	:public stMsg
{
public:
	stMsgServerDisconnect(){ cSysIdentifer = ID_MSG_GATE2C; usMsgType = MSG_DISCONNECT_SERVER  ;}
	unsigned char nServerType ; // eServerType ;
};

struct stMsgPlayerEnterGame
	:public stMsg
{
	stMsgPlayerEnterGame(){cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_PLAYER_ENTER_GAME ;}
	unsigned int nUserUID ;
};

struct stMsgPlayerBaseData
	:public stMsg
{
	stMsgPlayerBaseData(){ cSysIdentifer = ID_MSG_GAME2C; usMsgType = MSG_PLAYER_BASE_DATA ; }

	unsigned int nSessionID ; 
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

struct stMsgRoomPlayerLeave
	:public stMsg
{
public:
	stMsgRoomPlayerLeave(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_LEAVE ;}
	unsigned int nSessionID ; // the leaved player's seessionID ;
};

struct stMsgRoomEnter
	:public stMsg
{
	stMsgRoomEnter(){ cSysIdentifer = ID_MSG_C2GAME; usMsgType = MSG_ROOM_ENTER ; }
	unsigned char nRoomType ;
	unsigned char nRoomLevel ;
};

struct stMsgRoomEnterRet
	:public stMsg 
{
	stMsgRoomEnterRet(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_ENTER ; }
	unsigned char nRet ; // 0 success ; other value failed ;
};

struct stRoomPeerBrifInfo
{
	unsigned  int nSessionID ;
	unsigned char nIdx ; // index in the room ;
    unsigned char ePeerState ; // peer state ;  eRoomPeerState
	char pName[MAX_LEN_CHARACTER_NAME] ;
	unsigned char nTitle ;
	unsigned int nCoin ;
	unsigned int nBetCoin ;
	char nDefaulPhotoID ;     
	unsigned int nUserDefinePhotoID ;
};

struct stMsgRoomCurInfo
	:public stMsg 
{
	 stMsgRoomCurInfo(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_CURRENT_INFO ; }
public:
     char nSelfIdx ; // server roomidx ;
    
     unsigned int nRoomID ;  // used it to get curRoomInfo ;
	 int nSingleBetCoin;
	 int nTotalBetCoin; 
	 unsigned short nRound ;
	 unsigned char eRoomSate ;  // eRoomState 
	 unsigned char nPlayerCount ;
	 stRoomPeerBrifInfo* pInfos ;
};

struct stMsgRoomPlayerEnter
	:public stMsg
{
public:
	stMsgRoomPlayerEnter(){cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_ENTER ; }
	stRoomPeerBrifInfo nEnterPlayerInfo ;
};

struct stMsgRoomReady
	:public stMsg
{
public:
	stMsgRoomReady(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_READY ; }
};

struct stMsgRoomRet
	:public stMsg 
{
	stMsgRoomRet(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_RET ; }
	unsigned char nRet ; // 0 means success , 1 room state not fitable , 2 add money should greate than crrent ; 3 , unlegal pk target ;
};

struct stMsgRoomPlayerReady
	:public stMsg
{
	stMsgRoomPlayerReady(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_READY  ; }
	unsigned nReadyPlayerSessionID ; 
};

struct stMsgDistributeCard
:public stMsg
{
    stMsgDistributeCard(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_DISTRIBUTE_CARD ; }
	char nCurMainIdx ;
} ;

struct stMsgRoomWaitPlayerAction
:public stMsg
{
    stMsgRoomWaitPlayerAction(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_WAIT_PLAYER_ACTION ; }
    unsigned int nSessionID ;
    unsigned short nRound ;
};

struct stMsgRoomFollow
:public stMsg
{
    stMsgRoomFollow(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_FOLLOW ; }
} ;

struct stMsgRoomPlayerFollow
:public stMsg
{
    stMsgRoomPlayerFollow(){cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_FOLLOW ;}
    unsigned int nSessionID ;
} ;

struct stMsgRoomAdd
:public stMsg
{
    stMsgRoomAdd(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_ADD ; }
    int nAddMoney ; //  0 means double ;
};


struct stMsgRoomPlayerAdd
:public stMsg
{
    stMsgRoomPlayerAdd(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_ADD ;}
    unsigned int nSessionID ;
    int nNewSingle ;
    int nBetCoin ;
};

struct stMsgRoomLook
:public stMsg
{
    stMsgRoomLook(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_LOOK ; }
} ;

struct stMsgRoomPlayerLook
:public stMsg
{
    stMsgRoomPlayerLook(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_LOOK ; }
    unsigned int nSessionID ;
	char vCard[3] ;
};

struct stMsgRoomPK
:public stMsg
{
    stMsgRoomPK(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_PK ; }
    unsigned int nPKWithSessionID ;
};

struct stMsgRoomPlayerPK
:public stMsg
{
    stMsgRoomPlayerPK(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_PK ; }
    unsigned int nPKInvokeSessionID ;
    unsigned int nPKWithSessionID ;
    int nConsumCoin ;// pk will comsum Invoker's coin ;
    bool bWin ;
};

struct stMsgRoomGiveUp
:public stMsg
{
    stMsgRoomGiveUp(){ cSysIdentifer = ID_MSG_C2GAME ; usMsgType = MSG_ROOM_GIVEUP ; }
};

struct stMsgRoomPlayerGiveUp
:public stMsg
{
    stMsgRoomPlayerGiveUp(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_PLAYER_GIVEUP ; }
    char nIdx ;
} ;

struct stResultData
{
    char idx ;
    char vCard[3] ;
    int nResultCoin ; // 0 < loss , > 0 win ;
};

struct stMsgRoomResult
:public stMsg
{
    stMsgRoomResult(){ cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_RESULT ;}
    char nCount ;
    stResultData* pResultData ;
};

//----UPLOW IS NEW
//struct stMsgRegister
//	:public stMsg
//{
//	stMsgRegister(){cSysIdentifer = ID_MSG_C2LOGIN ; usMsgType = MSG_REGISTE ;}
//	char* pAccount;
//	char* pPassword;
//	char* pCharacterName;
//	unsigned char nAccountLen ;
//	unsigned char nPaswordLen ;
//	unsigned char nCharacterNameLen ;
//	unsigned char nAccountType ;  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .   
//} ;
//
//struct stMsgRegisterRet
//	:public stMsg
//{
//	stMsgRegisterRet(){ cSysIdentifer = ID_MSG_LOGIN2C; usMsgType = MSG_REGISTE ; }
//	char nResultCode ; // 0 , success  1 , accound repeated ; 2 ,name repeate ;
//	unsigned int nUserUID ; // player Unique ID ;
//};

//struct stMsgQuickEnterRegister
//	:public stMsg
//{
//	stMsgQuickEnterRegister(){ cSysIdentifer = ID_MSG_C2LOGIN; usMsgType = MSG_QUICK_ENTER_REGISTER ; }
//};
//
//struct stMsgQuickEnterRegisterRet
//	:public stMsg
//{
//	stMsgQuickEnterRegisterRet(){ cSysIdentifer = ID_MSG_LOGIN2C; usMsgType = MSG_QUICK_ENTER_REGISTER ; }
//	unsigned int nUserUID ; // player UniqueID ;
//};
//struct stMsgConnectRet
//	:public stMsg
//{
//public:
//	stMsgConnectRet(){ cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_CONNECT_RET ; }
//	bool bOk ;
//	unsigned char nErr ; // 1 ; no proper server ;
//};

//struct stMsgLogin
//	:public stMsg
//{
//	stMsgLogin(){ cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_LOGIN; }
//	char* pAccount;
//	char* pPassword;
//	unsigned char nAccountLen ;
//	unsigned char nPaswordLen ;
//};
//
//struct stMsgLoginRet
//	:public stMsg
//{
//public:
//	stMsgLoginRet()
//	{
//		cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_LOGIN;
//		nPlayerUID = 0 ;
//	}
//	bool bOk ; 
//	unsigned char nRetFlag ;  // 1 account don't exsit , 2 password error ;
//	unsigned int nPlayerUID ; 
//};
//
//struct stMsgUIDLogin
//	:public stMsg
//{
//	stMsgUIDLogin()
//	{
//		cSysIdentifer = ID_MSG_C2S; usMsgType = MSG_UID_LOGIN ;
//	}
//	unsigned int nPlayerUID ;
//};
//
//struct stMsgUIDLoginRet
//	:public stMsg
//{
//public:
//	stMsgUIDLoginRet()
//	{
//		cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_UID_LOGIN ;
//	}
//	unsigned char nRet ;  // zero success ; 1 , don't have active peer , 2 don't have idle server ;
//};
//
//struct stMsgReconnect
//	:public stMsg
//{
//	stMsgReconnect()
//	{
//		cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_RECONNECT ;
//	}
//	unsigned int nPlayerUID ;
//};
//
//struct stMsgReconnectRet
//	:public stMsg
//{
//	stMsgReconnectRet()
//	{
//		cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_RECONNECT ;
//	}
//
//	bool bSuccess ;
//};
// room message ;

struct stRoomPeerBrifData
{
	unsigned int nUserUID ;
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
	stMsgRoomInfo(){ cSysIdentifer = ID_MSG_GAME2C; usMsgType = MSG_ROOM_INFO ; }
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
	stMsgActionRet(){cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ACTION_RET ; }
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
	stMsgRoomPlayerCmd(){cSysIdentifer = ID_MSG_C2GAME; usMsgType = MSG_ROOM_PLAYER_ACTION ; }
	unsigned char nPeerIdx ;
	unsigned char nActionType ;  // eRoomPeerAction 
};

//struct stMsgRoomPlayerFollow
//	:public stMsgRoomPlayerCmd
//{
//	unsigned int nRoomAllBetMoney ;
//	unsigned int nPlayerLeftMoney;
//	unsigned int nPlayerAllBetMoney ;
//};
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


struct stGetCardPeer
	:public stMsg
{
public:
	stGetCardPeer(){cSysIdentifer = ID_MSG_GAME2C ; usMsgType = MSG_ROOM_DISTRIBUTE_CARD ;}
	unsigned char nPeerIdx ;
	unsigned char vCard[PEER_CARD_COUNT];
};

#pragma pack(pop)
#pragma once
#define MAX_LEN_ACCOUNT 50   // can not big then unsigned char max = 255
#define  MAX_LEN_PASSWORD 50 // can not big then unsigned char max = 255
#define MAX_LEN_CHARACTER_NAME 50 // can not big then unsigned char  max = 255
#define MAX_MSG_BUFFER_LEN 1024
#define RESEVER_GAME_SERVER_PLAYERS 100 
#define PEER_CARD_COUNT 3
#define MAX_ROOM_PEER 5
#define TIMER_WAIT_TO_READY 10
#define TIMER_WAIT_ACTION 30
#define TIMER_WAIT_DISTRIBUTE 3
#define TIMER_WAIT_PK 5
#define TIMER_WAIT_FINISH 6
#define MAX_ROOM_LEVEL 3 
#define GATE_SERVER_PORT 5000
#define DB_PORT 6000

enum eServerType
{
	eServer_None,
	eServer_Game = eServer_None ,
	eServer_Login,
	eServer_Max,
};
enum eRoomPeerAction
{
	eRoomPeerAction_None,
	eRoomPeerAction_EnterRoom,
	eRoomPeerAction_Ready,
	eRoomPeerAction_Follow,
	eRoomPeerAction_Add,
	eRoomPeerAction_PK,
	eRoomPeerAction_GiveUp,
	eRoomPeerAction_ShowCard,
	eRoomPeerAction_ViewCard,
	eRoomPeerAction_TimesMoneyPk,
	eRoomPeerAction_LeaveRoom,
	eRoomPeerAction_Speak_Default,
	eRoomPeerAction_Speak_Text,
	eRoomPeerAction_Max
};

enum eRoomFlag
{
	eRoomFlag_None ,
	eRoomFlag_ShowCard  ,
	eRoomFlag_TimesPK ,
	eRoomFlag_ChangeCard,
	eRoomFlag_Max,
};

enum eRoomState
{
	eRoomState_Wait,
	eRoomState_Playing,
	eRoomState_End,
	eRoomState_Max,
};

enum eBigRoomType
{
	eRoomType_None,
	eRoomType_Common = eRoomType_None,
	eRoomType_Max,
};

#define CHECK_MSG_SIZE(CHECK_MSG,nLen) \
{\
	if (sizeof(CHECK_MSG) > (nLen) ) \
	{\
		CLogMgr::SharedLogMgr()->ErrorLog("Msg Size Unlegal msg") ;	\
		return false; \
	}\
}

#define CHECK_MSG_SIZE_VOID(CHECK_MSG,nLen) \
{\
	if (sizeof(CHECK_MSG) > (nLen) ) \
{\
	CLogMgr::SharedLogMgr()->ErrorLog("Msg Size Unlegal msg") ;	\
	return; \
	}\
}
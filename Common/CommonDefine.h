#pragma once
#define MAX_LEN_ACCOUNT 50   // can not big then unsigned char max = 255
#define  MAX_LEN_PASSWORD 50 // can not big then unsigned char max = 255
#define MAX_LEN_CHARACTER_NAME 50 // can not big then unsigned char  max = 255
#define MAX_MSG_BUFFER_LEN 1024
#define RESEVER_GAME_SERVER_PLAYERS 100 
#define PEER_CARD_COUNT 3
#define MAX_ROOM_PEER 5
#define MAX_ROOM_LEVEL 3 
#define GATE_SERVER_PORT 5000
#define DB_PORT 6000


enum eRoomState
{
	eRoomState_None,
	eRoomState_WaitPeerToJoin,
	eRoomState_WaitPeerToGetReady,
	eRoomState_DistributeCard,
	eRoomState_WaitPeerAction,
	eRoomState_PKing,
	eRoomState_ShowingResult,
	eRoomState_Max,
};
// ROOM TIME BY SECOND 
#define TIME_ROOM_WAIT_READY 5
#define TIME_ROOM_DISTRIBUTE 5
#define TIME_ROOM_WAIT_PEER_ACTION 30
#define TIME_ROOM_PK_DURATION 5
#define TIME_ROOM_SHOW_RESULT 5

// player State 
enum eRoomPeerState
{
	eRoomPeer_None,
	eRoomPeer_Ready,
    eRoomPeer_Look,
	eRoomPeer_Unlook,
	eRoomPeer_GiveUp,
	eRoomPeer_Failed,
	eRoomPeer_Max,
};

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
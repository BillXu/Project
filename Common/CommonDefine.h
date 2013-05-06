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
	eRoom_Wait,
	eRoom_Playing,
	eRoom_End,
	eRoom_Max,
};
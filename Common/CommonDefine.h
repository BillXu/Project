#pragma once
#define MAX_LEN_ACCOUNT 50   // can not big then unsigned char max = 255
#define  MAX_LEN_PASSWORD 50 // can not big then unsigned char max = 255
#define MAX_LEN_CHARACTER_NAME 50 // can not big then unsigned char  max = 255
#define MAX_MSG_BUFFER_LEN 1024
#define RESEVER_GAME_SERVER_PLAYERS 100 
#define PEER_CARD_COUNT 3

enum eRoomPeerAction
{
	eRoomPeerAction_None,
	eRoomPeerAction_EnterRoom,
	eRoomPeerAction_Ready,
	eRoomPeerAction_Follow,
	eRoomPeerAction_Add,
	eRoomPeerAction_PK,
	eRooPeerAction_GiveUp,
	eRoomPeerAction_ShowCard,
	eRoomPeerAction_TimesMoneyPk,
	eRoomPeerAction_LevelRoom,
	eRoomPeerAction_SpeakDefault,
	eRoomPeerAction_SpeakWord,
	eRoomPeerAction_Max
};
#pragma once 
#include "MessageIdentifiers.h"
enum 
{
	ID_MSG_C2S = ID_USER_PACKET_ENUM + 1 ,
	ID_MSG_S2C,
	ID_MSG_GM2GA,  // GM = game server , GA = gate server ;
	ID_MSG_GA2GM,
	ID_MSG_DB2GM,
	ID_MSG_GM2DB,
	ID_MSG_VERIFY,
};

enum eMsgType 
{
	MSG_NONE,
	// the msg title used between servers 
	MSG_SERVERS_USE,
	MSG_VERIFY_GMS,  // verify that is game server ;
	MSG_VERIFY_CLIENT, // verify that is client ;
	MSG_VERIFY_GA, // verify that is gate server 
	MSG_VERIFY_DB,  // verify that is DBserver ;
	MSG_TRANSER_DATA, // tranfer data between servers ;
	MSG_DISCONNECT,
	MSG_SERVERS_USE_END,
	MSG_PUSH_BASE_DATA, // PUSH DATA FROM DB SERVER TO GAMER SERVER ;
	// msg title used between server and client ;
	MSG_REGISTE,
	MSG_CONNECT_RET,
	MSG_GAME_SERVER_LOST,
	MSG_LOGIN,
	MSG_REQUEST_BASE_DATA,

	// all room msg here ;
	MSG_ROOM_MSG_BEGIN,
	MSG_ENTER,
	MSG_EXIT,
	MSG_ROOM_PLAYER_ENTER,
	MSG_ROOM_PLAYER_EXIT,
	MSG_ROOM_INFO,
	MSG_ACTION,
	MSG_ACTION_RET,
	MSG_ROOM_PLAYER_ACTION,
	MSG_ROOM_DISTRIBUTE_CARD,
	MSG_ROOM_PLAYER_ACTION_TURN,
	MSG_ROOM_RESULT,
	MSG_ROOM_MSG_END,
	// all room msg above ,
};
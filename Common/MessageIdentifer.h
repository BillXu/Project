#pragma once 
#include "MessageIdentifiers.h"
enum 
{
	ID_MSG_C2S = ID_USER_PACKET_ENUM + 1 , // client to game server 
	ID_MSG_C2LOGIN,
	ID_MSG_C2GATE,
	ID_MSG_GATE2C,
	ID_MSG_C2GAME,
	ID_MSG_LOGIN2C,
	ID_MSG_GAME2C,
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
	MSG_VERIFY_GAME,  // verify that is game server ;
	MSG_VERIFY_LOGIN, // verify login server ;
	MSG_VERIFY_CLIENT, // verify that is client ;
	MSG_VERIFY_GATE, // verify that is gate server 
	MSG_VERIFY_DB,  // verify that is DBserver ;
	MSG_TRANSER_DATA, // tranfer data between servers ;
	MSG_DISCONNECT_SERVER, 
	MSG_DISCONNECT_CLIENT,
	// msg title used between server and client ;
	MSG_RECONNECT,   // client with gate 
	// all room msg here ;
	MSG_ROOM_MSG_BEGIN,
	MSG_ROOM_ENTER,
	MSG_ROOM_PLAYER_ENTER,  // MSG_ROOM_PLAYER_x means other player actions 
	MSG_ROOM_CURRENT_INFO, // when player enter room , will receive the room's current info ;

	MSG_ROOM_LEAVE,
	MSG_ROOM_PLAYER_LEAVE,

	MSG_ROOM_READY,
	MSG_ROOM_PLAYER_READY,

	MSG_ROOM_DISTRIBUTE_CARD,
	MSG_ROOM_WAIT_PLAYER_ACTION,

	MSG_ROOM_FOLLOW,
	MSG_ROOM_PLAYER_FOLLOW,

	MSG_ROOM_ADD,
	MSG_ROOM_PLAYER_ADD,

	MSG_ROOM_PK,
	MSG_ROOM_PLAYER_PK,

	MSG_ROOM_GIVEUP,
	MSG_ROOM_PLAYER_GIVEUP,
	
	MSG_ROOM_RESULT,
	/// UP LOAD IS OK 
	MSG_ENTER,
	MSG_EXIT,
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
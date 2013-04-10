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
	// msg title used between server and client ;
	MSG_REGISTE,
	MSG_LOGIN,
	MSG_REQUEST_BASE_DATA,
};
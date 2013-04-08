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
	MSG_LOGIN_CHECK,  // LOGIN WITH ACCOUNT AND PASSWORD 
	MSG_LOGIN_DIRECT,  // LOGIN WITH USERUID 
	MSG_VERIFY_GMS,  // verify that is game server ;
	MSG_VERIFY_CLIENT, // verify that is client ;
	MSG_VERIFY_GA, // verify that is gate server 
	MSG_VERIFY_DB,  // verify that is DBserver ;
	MSG_TRANSER_DATA, 
	MSG_DISCONNECT,
	MSG_REQUEST_BASE_DATA,
};
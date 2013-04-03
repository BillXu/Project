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
	MSG_LOGIN_CHECK,
	MSG_REQUEST_BASE_DATA,
};
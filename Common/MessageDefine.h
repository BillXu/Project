#pragma once 
// define message struct , used between Server and Client ;
#include "MessageIdentifer.h"
// WARNNING:�䳤�ַ��������ǲ������ս�� \0 ;
struct stMsg
{
	unsigned char cSysIdentifer ;
	unsigned short usMsgType ;
public:
	stMsg():cSysIdentifer( ID_USER_PACKET_ENUM  ),usMsgType(MSG_NONE){}
	virtual ~stMsg(){}
};
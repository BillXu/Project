#pragma once 
// define message struct , used between Server and Client ;
#include "MessageIdentifer.h"
// WARNNING:变长字符串，我们不包括终结符 \0 ;
struct stMsg
{
	unsigned char cSysIdentifer ;
	unsigned short usMsgType ;
public:
	stMsg():cSysIdentifer( ID_USER_PACKET_ENUM  ),usMsgType(MSG_NONE){}
	virtual ~stMsg(){}
};

struct stMsgRegister
	:public stMsg
{
	stMsgRegister(){cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_REGISTE ;}
	char* pAccount;
	char* pPassword;
	char* pCharacterName;
	unsigned char nAccountLen ;
	unsigned char nPaswordLen ;
	unsigned char nCharacterNameLen ;
};

struct stMsgRegisterRet
	:public stMsg
{
	stMsgRegisterRet(){ cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_REGISTE ; }
	bool bSuccess ; // 0 , success 
	char nErrCode ; // 1 , accound repeated ;
};
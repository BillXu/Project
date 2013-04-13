#pragma once 
#pragma pack(push)
#pragma pack(1)
// define message struct , used between Server and Client ;
#include "MessageIdentifer.h"
// WARNNING:变长字符串，我们不包括终结符 \0 ;
struct stMsg
{
	unsigned char cSysIdentifer ;
	unsigned short usMsgType ;
public:
	stMsg():cSysIdentifer( ID_USER_PACKET_ENUM  ),usMsgType(MSG_NONE){}
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
	unsigned char nAccountType ;  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .   
} ;

struct stMsgRegisterRet
	:public stMsg
{
	stMsgRegisterRet(){ cSysIdentifer = ID_MSG_S2C; usMsgType = MSG_REGISTE ; }
	unsigned char nAccountType ;  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .   
	bool bSuccess ; // 0 , success 
	char nErrCode ; // 1 , accound repeated ;
	char* pAccount;  // used when visitor register
	//char* pPasswrold; // used when visitor register
	unsigned char nAccountLen ; // used when visitor register
	//unsigned char nPaswordLen ; // used when visitor register  // Password = pAccount ;
};

struct stMsgConnectRet
	:public stMsg
{
public:
	stMsgConnectRet(){ cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_CONNECT_RET ; }
	bool bOk ;
	unsigned char nErr ; // 1 ; no proper server ;
};
struct stMsgLogin
	:public stMsg
{
	stMsgLogin(){ cSysIdentifer = ID_MSG_C2S ; usMsgType = MSG_LOGIN; }
	char* pAccount;
	char* pPassword;
	unsigned char nAccountLen ;
	unsigned char nPaswordLen ;
};

struct stMsgLoginRet
	:public stMsg
{
public:
	stMsgLoginRet()
	{
		cSysIdentifer = ID_MSG_S2C ; usMsgType = MSG_LOGIN;
	}
	bool bOk ; 
	unsigned char nRetFlag ;  // 1 account don't exsit , 2 password error ;
};


#pragma pack(pop)
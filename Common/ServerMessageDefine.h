#pragma once
// Define message , used between Servers ;mainly DBServer and GameServer 
#include "MessageDefine.h"

struct stMsg2DB
:public stMsg
{
	stMsg2DB(){ cSysIdentifer = ID_MSG_GM2DB, usMsgType = MSG_NONE ; }
};

struct stMsg2DBRet
:public stMsg
{
	stMsg2DB(){ cSysIdentifer = ID_MSG_DB2GM, usMsgType = MSG_NONE ; }
};

struct stMsg2DBLoginCheck
:public stMsg2DB
{
	char* pAccount ;
	char* pPassword ;
	unsigned short nAccountLen ;
	unsigned short nPasswordLen ;
	unsigned int nTempUID ;
public:
	stMsg2DBLoginCheck(){ usMsgType = MSG_LOGIN_CHECK ; }
};

struct stMsg2DBLoginCheckRet
:public stMsg2DBRet
{
	unsigned int nTempUID ;
	short nRetFlag ;  // 0 success , 1 account don't exsit , 2 password error ;
	unsigned int nUserUID ;
public:
	stMsg2DBLoginCheckRet(){ usMsgType = MSG_LOGIN_CHECK ; }
};

struct stMsg2DBRequestBaseData
:public stMsg2DBRet
{
	unsigned int nUserUID ;
public:
	stMsg2DBRequestBaseData()
	{
		usMsgType = MSG_REQUEST_BASE_DATA ;
	}
};

struct stMsg2DBRequestBaseDataRet
:public stMsg2DBRet
{
public:
	stMsg2DBRequestBaseDataRet(){ usMsgType = MSG_REQUEST_BASE_DATA ; }
public:
	// datas ;
};


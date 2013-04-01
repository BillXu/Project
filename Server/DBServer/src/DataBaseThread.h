#pragma once 
#include "ThreadMod.h"
#include "my_global.h"
#include "mysql.h"
class CDataBaseThread
:public  CThreadT
{
public:
	bool InitDataBase( const char* pIP,unsigned pPort , const char* pUserName,const char* pPassword, const char* pDBName );
	virtual void __run();
	void StopWork();
protected:
	bool ProcessRequest();
protected:
	MYSQL* m_pMySql ;
	bool m_bRunning ;
};
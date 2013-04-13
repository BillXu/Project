#pragma once
#include "ThreadMod.h"
#include "MessageDelegate.h"
#include "NetWorkManager.h"
class CClient
	:public CThreadT
{
public:
	void Init();
	virtual void __run(){ Run();delete this ;}
	void Run();
protected:
	CMessageDelegate m_pDelegate ;
	CNetWorkMgr m_pNetWork ;
};
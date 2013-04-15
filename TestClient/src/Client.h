#pragma once
#include "ThreadMod.h"
#include "NetWorkManager.h"
#include "IScene.h"
#include "PlayerData.h"
class CClient
	:public CThreadT
{
public:
	CClient();
	~CClient();
	void Init();
	virtual void __run(){ Run();delete this ;}
	void Run();
	CPlayerData* GetPlayerData(){return m_pPlayerData ;}
	IScene* GetCurrentScene(){ return m_pCurentScene ;}
protected:
	CNetWorkMgr m_pNetWork ;
	CPlayerData* m_pPlayerData ;
	IScene* m_pCurentScene ;
};
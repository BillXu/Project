#pragma once
#include "NetWorkManager.h"
enum eSceneType
{
	eScene_None,
	eScene_Login,
	eScene_Lab,
	eScene_Room,
	eScene_Max,
};

class IScene
	:public CNetMessageDelegate
{
public:
	IScene(CNetWorkMgr* pNetWork):m_eSceneType(eScene_None),m_pNetWork(pNetWork),m_bServerConnected(false),m_bRunning(false){}
	virtual ~IScene(){}
	virtual void OnEnterScene();
	virtual void OnEixtScene();
	virtual bool OnMessage( RakNet::Packet* pPacket ) ;
	virtual bool OnLostSever(RakNet::Packet* pPacket );
	virtual void OnUpdate(float fDeltaTime ){};
	eSceneType GetSceneType(){return m_eSceneType ; }
	bool OnConnectStateChanged( eConnectState eSate );
	virtual void OnVerifyed() {};
	bool IsRunning(){ return m_bRunning ;}
protected:
	eSceneType m_eSceneType ;
	CNetWorkMgr* m_pNetWork ;
	bool m_bServerConnected ;
	bool m_bRunning ;
};
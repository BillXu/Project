#pragma once 
#include "IScene.h"
#include <string>
class CLoginScene
	:public IScene
{
public:
	CLoginScene(CNetWorkMgr* pNetWork ):IScene(pNetWork){ m_eSceneType = eScene_Login ;}
	virtual bool OnMessage( RakNet::Packet* pPacket ) ;
	virtual void OnVerifyed();
	void Login( const char* pAccound , const char* pPassword );
	void Register( const char* pName ,const char* pAccound , const char* pPassword , int nType );  // 0 visitor login register , 1 ordinary register ,2 SINA WEIBO login register  3 QQ login register .
};
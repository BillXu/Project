//
//  ClientApp.h
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#ifndef __Gold__ClientApp__
#define __Gold__ClientApp__
#include "cocos2d.h"
#include "NetWorkManager.h"
#include "PlayerData.h"
class CClientApp
:cocos2d::CCObject
,public CNetMessageDelegate
{
public:
    static CClientApp* SharedClientApp();
    ~CClientApp();
    void StartApp();
    void SendMsg(stMsg* pmsg , short nLen );
    bool OnLostSever(RakNet::Packet* pMsg) ;
    bool OnMessage( RakNet::Packet* pMsg );
    bool OnConnectStateChanged( eConnectState eSate, RakNet::Packet* pMsg);
    void OnExitApp();
    void update(float dt) ;
    unsigned int GetSessionID();
    CPlayerData* GetPlayerData(){ return &m_PlayerData ; }
protected:
    static CClientApp* s_pClient ;
    CNetWorkMgr* m_pNetWorkMgr ;
    bool m_bConnected ;
    CPlayerData m_PlayerData ;
    unsigned int m_nSessionID ; // used when reconnecting;
};

#endif /* defined(__Gold__ClientApp__) */

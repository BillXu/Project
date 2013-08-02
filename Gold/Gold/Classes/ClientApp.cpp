//
//  ClientApp.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#include "ClientApp.h"
#include "CommonDefine.h"
#include "MessageDefine.h"
CClientApp* CClientApp::s_pClient = NULL ;
CClientApp* CClientApp::SharedClientApp()
{
    return s_pClient ;
}

CClientApp::~CClientApp()
{
    m_pNetWorkMgr->ShutDown() ;
    delete m_pNetWorkMgr ;
    m_pNetWorkMgr = NULL ;
}

void CClientApp::StartApp()
{
    // set up net work 
    m_pNetWorkMgr = new CNetWorkMgr ;
    m_pNetWorkMgr->SetupNetwork();
    m_pNetWorkMgr->ConnectToServer("", GATE_SERVER_PORT ) ;
    m_pNetWorkMgr->AddMessageDelegate(this) ;
    
    // meber var
    m_bConnected = false ;
    m_nSessionID = 0 ;
    
    // invoke update
    cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false) ;
    
    // player data ;
    m_PlayerData.Init() ;
}

void CClientApp::SendMsg(stMsg *pmsg, short nLen)
{
    if ( m_bConnected )
    {
        m_pNetWorkMgr->SendMsg((char*)pmsg, nLen) ;
    }
    else
    {
        cocos2d::CCMessageBox("Serve disconnect , reconnectting !", "TIP") ;
    }
}

bool CClientApp::OnLostSever(RakNet::Packet *pMsg)
{
    m_bConnected = false ;
    // will recnnected ;
    return true ;
}

void CClientApp::OnExitApp()
{
    m_pNetWorkMgr->ShutDown() ;
}

void CClientApp::update(float dt)
{
    m_pNetWorkMgr->ReciveMessage() ;
}

bool CClientApp::OnConnectStateChanged( eConnectState eSate, RakNet::Packet* pMsg)
{
    if ( eSate == eConnect_Accepted )
    {
        m_bConnected = true ;
        stMsg msg ;
        msg.cSysIdentifer = ID_MSG_VERIFY ;
        msg.usMsgType = MSG_VERIFY_CLIENT ;
        SendMsg(&msg, sizeof(msg)) ;
    }
    else if ( eConnect_Failed == eSate )
    {
        cocos2d::CCMessageBox("Connect Failed", "TIP") ;
    }
    else if ( eConnect_SeverFull == eSate )
    {
        cocos2d::CCMessageBox("Server is Full", "TIP") ;
    }
    return true ;
}

bool CClientApp::OnMessage( RakNet::Packet* pRakMsg )
{
    stMsg* pMsg = (stMsg*)pRakMsg->data ;
    if ( m_PlayerData.OnMessage(pMsg) )
    {
        return true ;
    }
    return false ;
}

//
//  NetWorkManager.cpp
//  God
//
//  Created by Xu BILL on 12-10-30.
//
//
#include "NetWorkManager.h"
#include "RakPeerInterface.h"
#include "LogManager.h"
#include "MessageIdentifiers.h"
#include "MessageDefine.h"
#include "WaitingLayer.h"
int CNetWorkMgr::s_nCurrentDataSize = 0 ;
CNetWorkMgr::CNetWorkMgr()
{
    m_pNetPeer = NULL ;
    m_eConnectType = eConnectType_None;
}

CNetWorkMgr::~CNetWorkMgr()
{
    if ( m_pNetPeer )
    {
        RakNet::RakPeerInterface::DestroyInstance(m_pNetPeer) ;
        m_pNetPeer = NULL ;
    }
    RemoveAllDelegate() ;
}

void CNetWorkMgr::ShutDown()
{
    if ( m_pNetPeer )
    {
        m_eConnectType = eConnectType_None;
        m_pNetPeer->Shutdown(1);
        RakNet::RakPeerInterface::DestroyInstance(m_pNetPeer) ;
        m_pNetPeer = NULL ;
    }
}

CNetWorkMgr* CNetWorkMgr::SharedNetWorkMgr()
{
    static CNetWorkMgr s_gNetWork ;
    return &s_gNetWork ;
}

bool CNetWorkMgr::ConnectToServer(const char *pSeverIP, unsigned short nPort)
{
    DisconnectCurServer();
    if ( !m_pNetPeer )
    {
        m_pNetPeer = RakNet::RakPeerInterface::GetInstance() ;
        RakNet::SocketDescriptor sDesc (0,0) ;
        m_pNetPeer->Startup(2, &sDesc, 1);
        m_pNetPeer->SetMaximumIncomingConnections(0);
    }
    RakNet::ConnectionAttemptResult cReslt = m_pNetPeer->Connect(pSeverIP, nPort, NULL ,0) ;
    m_eConnectType = eConnectType_Connecting ;
    switch (cReslt)
    {
        case RakNet::CONNECTION_ATTEMPT_STARTED:
        {
            return true ;
        }
            break;
        case RakNet::INVALID_PARAMETER:
        {
            CLogMgr::SharedLogMgr()->ErrorLog("Connect to ServerFail :INVALID_PARAMETER ");
        }
            break ;
        case RakNet::CANNOT_RESOLVE_DOMAIN_NAME:
        {
            CLogMgr::SharedLogMgr()->ErrorLog("Connect to ServerFail :CANNOT_RESOLVE_DOMAIN_NAME ");
        }
            break ;
        case RakNet::ALREADY_CONNECTED_TO_ENDPOINT:
        {
            CLogMgr::SharedLogMgr()->ErrorLog("Connect to ServerFail :ALREADY_CONNECTED_TO_ENDPOINT ");
        }
            break ;
        case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
        {
            CLogMgr::SharedLogMgr()->ErrorLog("Connect to ServerFail :CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS ");
        }
            break ;
        case RakNet::SECURITY_INITIALIZATION_FAILED:
        {
            CLogMgr::SharedLogMgr()->ErrorLog("Connect to ServerFail :SECURITY_INITIALIZATION_FAILED ");
        }
            break ;
        default:
            break;
    }
    return false ;
}

void CNetWorkMgr::ReciveMessage()
{
    if ( m_pNetPeer == NULL )
        return ;
    RakNet::Packet* packet = NULL ;
    while ( (packet = m_pNetPeer->Receive()))
    {
        unsigned char nMessageID = packet->data[0] ;
        s_nCurrentDataSize = packet->length ;
        switch (nMessageID)
        {
            case ID_USER_PACKET_ENUM:
            case MSG_LOGIN_S2C:
            case MSG_GAME_S2C:
            {
                stMsg* pMsg = (stMsg*)(&(packet->data[0]));
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnReciveLogicMessage),pMsg) ;
                CWaitingLayer::SharedWaitingLayer()->EndWaiting() ;
            }
            break;
            case ID_DISCONNECTION_NOTIFICATION:
            case ID_CONNECTION_LOST:
            {
                if ( m_eConnectType == eConnectType_Connected )
                {
                    m_eConnectType = eConnectType_Disconnectd ;
                }
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnLostServer),0) ;
            }
                break ;
            case ID_CONNECTION_REQUEST_ACCEPTED:
            {
                m_nCurrentServer = packet->guid ;
                CLogMgr::SharedLogMgr()->PrintLog("Connected To Server ");
                m_eConnectType = eConnectType_Connected;
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnConnectSateChanged),&nMessageID) ;
            }
                break ;
            case ID_CONNECTION_ATTEMPT_FAILED:
            {
                m_eConnectType = eConnectType_None ;
                CLogMgr::SharedLogMgr()->ErrorLog(" Cann't Connect Server ");
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnConnectSateChanged),&nMessageID) ;
            }
                break ;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
            {
                CLogMgr::SharedLogMgr()->ErrorLog("Server is full and busy !");
                m_eConnectType = eConnectType_None ;
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnConnectSateChanged),&nMessageID) ;
            }
                break ;
            case ID_CONNECTION_BANNED:
            {
                CLogMgr::SharedLogMgr()->ErrorLog("BANNED By targeted Server");
                m_eConnectType = eConnectType_None ;
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnConnectSateChanged),&nMessageID) ;
            }
                break ;
            default:
                break;
        }
        
        m_pNetPeer->DeallocatePacket(packet) ;

    }
}

bool CNetWorkMgr::SendMsg(const char *pbuffer, int iSize)
{
    if ( IsConnected() == false || m_pNetPeer == NULL )
        return false ;
    CWaitingLayer::SharedWaitingLayer()->StartWaiting() ;  // waiting layer ;
    return m_pNetPeer->Send(pbuffer, iSize, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) ;
}

void CNetWorkMgr::AddMessageDelegate(CNetMessageDelegate *pDelegate)
{
    RemoveMessageDelegate(pDelegate) ;
    m_vAllDelegate.push_back(pDelegate) ;
}

void CNetWorkMgr::RemoveAllDelegate()
{
    m_vAllDelegate.clear() ;
}

void CNetWorkMgr::RemoveMessageDelegate(CNetMessageDelegate *pDelegate)
{
    LIST_DELEGATE::iterator iter = m_vAllDelegate.begin() ;
    for ( ; iter != m_vAllDelegate.end() ; ++iter )
    {
        if ( *iter == pDelegate )
        {
            m_vAllDelegate.erase(iter) ;
            return ;
        }
    }
}

bool CNetWorkMgr::OnLostServer( CNetMessageDelegate* pDeleate,void* pData )
{
    return pDeleate->OnLostSever() ;
}

bool CNetWorkMgr::OnReciveLogicMessage( CNetMessageDelegate* pDeleate,void* pData )
{
    return pDeleate->OnMessage((stMsg*)pData);
}

void CNetWorkMgr::EnumDeleagte( CNetWorkMgr* pTarget, lpfunc pFunc, void* pData )
{
    LIST_DELEGATE::iterator iter = m_vAllDelegate.begin() ;
    for ( ; iter != m_vAllDelegate.end(); ++iter )
    {
        if ( (pTarget->*pFunc)(*iter,pData))
            return ;
    }
}

void CNetWorkMgr::DisconnectCurServer()
{
    if ( IsConnected() )
    {
        m_pNetPeer->CloseConnection(m_nCurrentServer, true) ;
    }
    m_eConnectType = eConnectType_None ;
}

bool CNetWorkMgr::OnConnectSateChanged( CNetMessageDelegate* pDeleate,void* pData )
{
    unsigned char nMessageID = *((unsigned char*)pData);
    CNetMessageDelegate::eConnectState eSate = CNetMessageDelegate::eConnect_Accepted;
    switch (nMessageID)
    {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            eSate = CNetMessageDelegate::eConnect_Accepted;
        }
            break ;
        case ID_CONNECTION_ATTEMPT_FAILED:
        {
            eSate = CNetMessageDelegate::eConnect_Failed;
        }
            break ;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
        {
            eSate = CNetMessageDelegate::eConnect_SeverFull;
        }
            break ;
        case ID_CONNECTION_BANNED:
        {
            eSate = CNetMessageDelegate::eConnect_Banned;
        }
            break ;
        default:
            return true ;
    }
    return pDeleate->OnConnectStateChanged(eSate) ;
}


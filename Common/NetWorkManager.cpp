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
int CNetWorkMgr::s_nCurrentDataSize = 0 ;
CNetWorkMgr::CNetWorkMgr()
{
    m_pNetPeer = NULL ;
    m_eConnectType = eConnectType_None;
	m_nMaxConnectTo = 0;
	m_nConnectedTo = 0 ;
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

void CNetWorkMgr::SetupNetwork( int nIntendServerCount )
{
	if ( !m_pNetPeer )
	{
		m_nMaxConnectTo = nIntendServerCount ;
		m_pNetPeer = RakNet::RakPeerInterface::GetInstance() ;
		RakNet::SocketDescriptor sDesc (0,0) ;
		m_pNetPeer->Startup(nIntendServerCount, &sDesc, 1);
		m_pNetPeer->SetMaximumIncomingConnections(0);
	}
}

bool CNetWorkMgr::ConnectToServer(const char *pSeverIP, unsigned short nPort)
{
	assert(m_pNetPeer && "Pls SetupNetwork() first! " );
    if ( !m_pNetPeer )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("m_pNetPeer is null , please setup network first ");
		return false ;
	}

	assert(m_nConnectedTo < m_nMaxConnectTo && "no more slot for new coming server" );
	if ( m_nMaxConnectTo <= m_nConnectedTo )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("no more slot for new coming server, so can not connected to the server: %s , port: %d",pSeverIP, nPort );
		return false ;
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
            case ID_DISCONNECTION_NOTIFICATION:
            case ID_CONNECTION_LOST:
            {
                if ( m_eConnectType == eConnectType_Connected )
                {
                    m_eConnectType = eConnectType_Disconnectd ;
                }
                EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnLostServer),packet) ;
            }
                break ;
            case ID_CONNECTION_REQUEST_ACCEPTED:
            {
				++m_nConnectedTo;
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
				{
					if ( nMessageID >= ID_USER_PACKET_ENUM && nMessageID <= 138 )
					{
						printf("this is: %s \n",packet->guid.ToString()) ;
						EnumDeleagte(this, (lpfunc)(&CNetWorkMgr::OnReciveLogicMessage),packet) ;
					}
				}
                break;
        }
        
        m_pNetPeer->DeallocatePacket(packet) ;

    }
}

bool CNetWorkMgr::SendMsg(const char *pbuffer, int iSize)
{
    if ( m_pNetPeer == NULL )
        return false ;
    return m_pNetPeer->Send(pbuffer, iSize, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_nCurrentServer, false) ;
}

bool CNetWorkMgr::SendMsg( const char* pbuffer , int iSize,RakNet::RakNetGUID& nServerNetUID )
{
	if ( m_pNetPeer == NULL )
		return false ;
	return m_pNetPeer->Send(pbuffer, iSize, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, nServerNetUID, false) ;
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
    return pDeleate->OnLostSever((RakNet::Packet*)pData) ;
}

bool CNetWorkMgr::OnReciveLogicMessage( CNetMessageDelegate* pDeleate,void* pData )
{
    return pDeleate->OnMessage((RakNet::Packet*)pData);
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

void CNetWorkMgr::DisconnectServer( RakNet::RakNetGUID& nServerNetUID )
{
    //if ( IsConnected() )
    {
        m_pNetPeer->CloseConnection(nServerNetUID, true) ;
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


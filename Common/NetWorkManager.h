//
//  NetWorkManager.h
//  God
//
//  Created by Xu BILL on 12-10-30.
//
//

#ifndef God_NetWorkManager_h
#define God_NetWorkManager_h
#include <list>
#include "RakNetTypes.h"
struct stMsg ;
namespace RakNet
{
    class RakPeerInterface ;
}

class CNetMessageDelegate
{
public:
    enum eConnectState
    {
        eConnect_SeverFull,
        eConnect_Accepted,
        eConnect_Failed,
        eConnect_Banned,
        eConnect_Max,
    };
public:
    CNetMessageDelegate(){};
    virtual ~CNetMessageDelegate(){} ;
    virtual bool OnMessage( stMsg* pMsg ) = 0 ;
    virtual bool OnLostSever(){ return  false; };
    virtual bool OnConnectStateChanged( eConnectState eSate ){ return false ;} ;
};

class CNetWorkMgr
{
public:
    enum eConnectType
    {
        eConnectType_None ,
        eConnectType_Connecting,
        eConnectType_Connected,
        eConnectType_Disconnectd ,
        eConnectType_Max,
    };
public:
    typedef std::list<CNetMessageDelegate*> LIST_DELEGATE ;
    typedef bool (CNetWorkMgr::*lpfunc)(CNetMessageDelegate* pDeleate ,void* pData );
public:
    CNetWorkMgr();
    ~CNetWorkMgr();
    static CNetWorkMgr* SharedNetWorkMgr();
    
    bool ConnectToServer( const char* pSeverIP, unsigned short nPort );
    void ReciveMessage();
    bool SendMsg( const char* pbuffer , int iSize );
    
    void AddMessageDelegate(CNetMessageDelegate * pDelegate );
    void RemoveMessageDelegate(CNetMessageDelegate* pDelegate);
    void RemoveAllDelegate();
    eConnectType GetCurrentConnectType(){ return m_eConnectType ;}
    bool IsConnected(){ return m_eConnectType == eConnectType_Connected ;}
    void EnumDeleagte( CNetWorkMgr* pTarget, lpfunc pFunc, void* pData );
    void DisconnectCurServer();
    
    void ShutDown();
public:
    static int s_nCurrentDataSize ;
protected:
    bool OnLostServer( CNetMessageDelegate* pDeleate,void* pData );
    bool OnReciveLogicMessage( CNetMessageDelegate* pDeleate,void* pData );
    bool OnConnectSateChanged( CNetMessageDelegate* pDeleate,void* pData );
protected:
    LIST_DELEGATE m_vAllDelegate;
    RakNet::RakPeerInterface* m_pNetPeer;
    RakNet::RakNetGUID m_nCurrentServer ;
    eConnectType m_eConnectType ;
};

#endif

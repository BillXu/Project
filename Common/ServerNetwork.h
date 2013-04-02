#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include <list>
class CServerNetworkDelegate
{
public:
	CServerNetworkDelegate():m_nPriority(0){}
	virtual ~CServerNetworkDelegate(){}
	virtual bool OnMessage( void* pData ) = 0;
	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, void* pData ){}
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, void* pData ){}
	void SetPriority(unsigned int nPriority );
protected:
	unsigned int m_nPriority ;
};

class CServerNetwork
{
public:
	typedef std::list<CServerNetworkDelegate*> LIST_DELEGATE ;
public:
	static CServerNetwork* SharedNetwork();
	CServerNetwork();
	~CServerNetwork();
	bool StartupNetwork( unsigned short nPort , int nMaxInComming );
	void ShutDown();
	void RecieveMsg();
	void SendMsg(const char* pData , int nLength , RakNet::RakNetGUID& nSendToOrExcpet ,bool bBroadcast );
	void ClosePeerConnection(RakNet::RakNetGUID& nPeerToClose);
	void AddDelegate(CServerNetworkDelegate* pDelegate , unsigned int nPriority = 0 );
	void RemoveDelegate(CServerNetworkDelegate* pDelegate );
protected:
	RakNet::RakPeerInterface* m_pNetPeer ;
	LIST_DELEGATE m_vAllDelegates ;
};
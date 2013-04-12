#include "MessageDelegate.h"
#include "MessageDefine.h"
bool bWaitRet = false ;
bool CMessageDelegate::OnMessage( RakNet::Packet* pMsgPacket )
{
	stMsg* pMsg = (stMsg*)pMsgPacket->data ;
	if ( pMsg->usMsgType == MSG_TRANSER_DATA )
	{
		
	}
	switch ( pMsg->usMsgType )
	{
		
	}
	return true ;
}

bool CMessageDelegate::OnConnectStateChanged( eConnectState eSate )
{
	if ( eConnect_Accepted == eSate )
	{
		printf( "Connected Server!" );
		stMsg msgVerify ;
		msgVerify.cSysIdentifer = ID_MSG_VERIFY ;
		msgVerify.usMsgType = MSG_VERIFY_CLIENT ;
		CNetWorkMgr::SharedNetWorkMgr()->SendMsg((char*)&msgVerify,sizeof(msgVerify)) ;
		bWaitRet = true ;
	}
	return true ;
}
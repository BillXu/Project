#include "MessageDelegate.h"
#include "MessageDefine.h"
#include "CommonDefine.h"
#include "LogManager.h"
bool CMessageDelegate::OnMessage( RakNet::Packet* pMsgPacket )
{
	stMsg* pMsg = (stMsg*)pMsgPacket->data ;
	switch ( pMsg->usMsgType )
	{
	case MSG_REGISTE:
		{
			stMsgRegisterRet* pRet = (stMsgRegisterRet*)pMsg ;
			if ( pRet->bSuccess )
			{
				if ( pRet->nAccountType == 0 )
				{
					char pAccount [MAX_LEN_ACCOUNT] = {0} ;
					char* pBuffer = (char*)pRet ;
					pBuffer += sizeof(stMsgRegisterRet);
					memcpy(pAccount,pBuffer,pRet->nAccountLen);
					CLogMgr::SharedLogMgr()->PrintLog("I Register ok. Accound = %s , Password = %s",pAccount,pAccount );
				}
				else
				{

				}
			}
			else
			{
				CLogMgr::SharedLogMgr()->PrintLog("I Register Fail , so pity !");
			}
			bRegisterOK = true ;
		}
		break; 
	case MSG_CONNECT_RET:
		{
			stMsgConnectRet* pMsgRet = (stMsgConnectRet*)pMsg ;
			if ( pMsgRet->bOk )
			{
				printf( "no game server \n");
			}
		}
		break;
	}
	return true ;
}

bool CMessageDelegate::OnConnectStateChanged( eConnectState eSate )
{
	if ( eConnect_Accepted == eSate )
	{
		printf( "Connected Server!\n" );
		bCannected = true ;
	}
	return true ;
}
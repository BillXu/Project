#include "LoginScene.h"
#include "MessageDefine.h"
#include "LogManager.h"
bool CLoginScene::OnMessage( RakNet::Packet* pPacket )
{
	stMsg* pMsg = (stMsg*)pPacket->data ;
	IScene::OnMessage(pPacket) ;
	switch (pMsg->usMsgType)
	{
	case MSG_REGISTE:
		{
			stMsgRegisterRet* pRealMsg = (stMsgRegisterRet*)pMsg ;
			if ( pRealMsg->bSuccess )
			{
				CLogMgr::SharedLogMgr()->PrintLog("Register OK ") ;
				if ( 0 == pRealMsg->nAccountType )
				{
					char* pAcount = new char[pRealMsg->nAccountLen + 1 ] ;
					memset(pAcount,0,pRealMsg->nAccountLen + 1);
					memcpy(pAcount,(char*)pMsg + sizeof(stMsgRegisterRet),pRealMsg->nAccountLen);
					CLogMgr::SharedLogMgr()->PrintLog("visitor register : Account = %s , Password = %s",pAcount,pAcount );
					delete [] pAcount ;
				}
			}
			else
			{
				CLogMgr::SharedLogMgr()->ErrorLog("Register Failed , Account had exist ") ;
			}
		}
		break; 
	case MSG_LOGIN:
		{
			stMsgLoginRet* pRealMsg = (stMsgLoginRet*)pMsg ;
			if ( pRealMsg->bOk )
			{
				CLogMgr::SharedLogMgr()->PrintLog("Login OK") ;
			}
			else
			{
				if (  1==pRealMsg->nRetFlag )
				{
					CLogMgr::SharedLogMgr()->PrintLog( "Login Error : Account don't exist" ) ;
				}
				else if ( 2 == pRealMsg->nRetFlag )
				{
					CLogMgr::SharedLogMgr()->PrintLog("Login Error : Password Error") ;
				}
			}

		}
		break;
	default:
		{

		}
		break;
	}
	return false ;
}

void CLoginScene::OnVerifyed()
{

}

void CLoginScene::Login( const char* pAccound , const char* pPassword )
{
	stMsgLogin msg ;
	msg.pAccount= NULL ;
	msg.pAccount = NULL ;
	msg.nAccountLen = strlen(pAccound);
	msg.nPaswordLen = strlen(pPassword);
	char* pbuffer = new char[msg.nPaswordLen + msg.nAccountLen + sizeof(msg)];
	memcpy(pbuffer,(void*)&msg,sizeof(msg));
	sprintf(pbuffer + sizeof(msg),"%s",pAccound) ;
	sprintf(pbuffer + msg.nAccountLen + sizeof(msg),"%s", pPassword) ;
	m_pNetWork->SendMsg(pbuffer,msg.nAccountLen + sizeof(msg) + msg.nPaswordLen) ;
	delete []pbuffer ;
}

void CLoginScene::Register( const char* pName ,const char* pAccound , const char* pPassword , int nType )
{
	stMsgRegister msg ;
	msg.nAccountType = nType ;
	msg.nAccountLen = msg.nPaswordLen = msg.nCharacterNameLen = 0 ;
	msg.nCharacterNameLen = strlen(pName) ;
	if ( 0 != nType )
	{
		msg.nAccountLen = strlen(pAccound);
		msg.nPaswordLen = strlen(pPassword) ;
	}

	unsigned short nLen = msg.nAccountLen + msg.nPaswordLen + msg.nCharacterNameLen + sizeof(msg
		);
	char* pbuffer = new char[nLen] ;
	unsigned nLenOffset = 0 ;
	memcpy(pbuffer + nLenOffset,&msg,sizeof(msg));
	nLenOffset = sizeof(msg);
	if ( 0 != msg.nAccountLen )
	{
		memcpy(pbuffer + nLenOffset, pAccound, msg.nAccountLen );
		nLenOffset += msg.nAccountLen ;
		memcpy(pbuffer + nLenOffset, pPassword, msg.nPaswordLen );
		nLenOffset += msg.nPaswordLen ;
	}
	memcpy(pbuffer + nLenOffset, pName, msg.nCharacterNameLen );
	nLenOffset += msg.nCharacterNameLen ;
	m_pNetWork->SendMsg(pbuffer,nLen) ;
	delete [] pbuffer ;
}
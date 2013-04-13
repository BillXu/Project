#include "Client.h"
#include "MessageDefine.h"
#include "CommonDefine.h"
void CClient::Init()
{
	m_pNetWork.SetupNetwork();
	m_pNetWork.ConnectToServer("127.0.0.1",3000) ;
	m_pNetWork.AddMessageDelegate(&m_pDelegate);
}

void CClient::Run()
{
	while ( m_pDelegate.bRegisterOK == false  )
	{
		m_pNetWork.ReciveMessage() ;
		if (m_pDelegate.bCannected )
		{
			stMsg msgVerify ;
			msgVerify.cSysIdentifer = ID_MSG_VERIFY ;
			msgVerify.usMsgType = MSG_VERIFY_CLIENT ;
			m_pNetWork.SendMsg((char*)&msgVerify,sizeof(msgVerify)) ;
			Sleep(10) ;
			m_pDelegate.bCannected = false ;
			m_pDelegate.bVerify = true ;
			continue; 
		}
		if ( m_pDelegate.bVerify )
		{
			// send register ;
			m_pDelegate.bVerify = false ;
			stMsgRegister msg ;
			msg.nAccountType = 0 ;
			msg.nCharacterNameLen = strlen("thisVisiter");
			char pBuffer [MAX_LEN_ACCOUNT] = {0} ;
			memcpy(pBuffer,&msg,sizeof(msg));
			sprintf(pBuffer + sizeof(msg),"%s","thisVisiter") ;
			m_pNetWork.SendMsg(pBuffer,sizeof(msg)+msg.nCharacterNameLen) ;
		}
		Sleep(10);
	}
	m_pNetWork.ShutDown();
}
#include "main.h"
#include "MessageDelegate.h"
#include "MessageDefine.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include <iostream>
extern bool bWaitRet ;
void Register(CMessageDelegate&delegateObject)
{
		
	CNetWorkMgr::SharedNetWorkMgr()->SetupNetwork();
	CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer("127.0.0.1",3000);
	CNetWorkMgr::SharedNetWorkMgr()->AddMessageDelegate(&delegateObject) ;



	bWaitRet = false ;
}

int main()
{
	CMessageDelegate delegateObject ;
	//CNetWorkMgr::SharedNetWorkMgr()->SetupNetwork();
	//CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer("127.0.0.1",3000);
	//CNetWorkMgr::SharedNetWorkMgr()->AddMessageDelegate(&delegateObject) ;
	Register(delegateObject);
	static bool b = false ;
	char pAccount[255] = { 0 } ;
	uint64_t tta = RakNet::RakPeerInterface::Get64BitUniqueRandomNumber();
	sprintf(pAccount,"%I64d",tta);
	if ( b )
	{
		Register(delegateObject);
		b = false ;
	}
	while ( true )
	{

		CNetWorkMgr::SharedNetWorkMgr()->ReciveMessage() ;
		if (bWaitRet)
		{
			stMsgRegister msg ;
			msg.nAccountType = 0 ;
			msg.nCharacterNameLen = 0 ;
			CNetWorkMgr::SharedNetWorkMgr()->SendMsg((char*)&msg,sizeof(msg)) ;
			CNetWorkMgr::SharedNetWorkMgr()->ShutDown();
			b = true ;
			bWaitRet = false ;
		}
		Sleep(5);
	}
	getchar();
}
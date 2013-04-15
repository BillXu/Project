#include "main.h"
#include "MessageDefine.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include <iostream>
#include "Client.h"

int main()
{
	CClient* pClient = NULL;
	int i = 0 ;
	while (  i++ < 5 )
	{
		pClient = new CClient ;
		pClient->Init() ;
		pClient->Start() ;
		Sleep(5);
	}
	getchar();
}
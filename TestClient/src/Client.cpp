#include "Client.h"
#include "MessageDefine.h"
#include "CommonDefine.h"
CClient::CClient()
{
	m_pPlayerData = NULL ;
	m_pCurentScene = NULL ;
}

CClient::~CClient()
{
	delete m_pPlayerData ;
	if ( m_pCurentScene )
	{
		m_pCurentScene->OnEixtScene();
		delete m_pCurentScene ;
	}
}

void CClient::Init()
{
	m_pNetWork.SetupNetwork();
	m_pNetWork.ConnectToServer("127.0.0.1",3000) ;
}

void CClient::Run()
{
	clock_t nTick = clock();
	while ( 1  )
	{
		m_pNetWork.ReciveMessage();

		if ( m_pCurentScene)
		{
			float fElasp = (float)(clock() - nTick ) / (float)CLOCKS_PER_SEC ;
			nTick = clock();
			m_pCurentScene->OnUpdate(fElasp);
		}
		Sleep(5);
	}
	m_pNetWork.ShutDown();
}
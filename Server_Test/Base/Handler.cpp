#include "PacketPriority.h"
#include "Handler.h"
//#include "CmdParser.h"
#include "InternalMsg.h"

CHandler::CHandler(RakNet::SystemAddress& peerID,RakNet::RakNetGUID guid)
{
	m_peerSystemAddress = peerID;
	m_peerGuid = guid;
	m_pIOServer = NULL;
	m_pTreatorBase = NULL;
	m_pListenThread = NULL;
	m_pUserBase = NULL;
}

CHandler::~CHandler()
{
	printf("CHandler::~CHandler()\n");
	//assert(0);
}

void CHandler::SetUser(IUserBase* pUser)
{
	assert(pUser);
	m_pUserBase = pUser;
}

IUserBase* CHandler::GetUser()
{
	return m_pUserBase;
}

void CHandler::SetIOServer(RakNet::RakPeerInterface* pServer)
{
	m_pIOServer = pServer;
}

void CHandler::RegisterTreator(ITreatorBase* pTreator)
{
	m_pTreatorBase = pTreator;
}

void CHandler::SetListenThread( CListenThread* const pListenThread)
{
	m_pListenThread = pListenThread;
}

void CHandler::OnHandle(unsigned char* pData, unsigned int length)
{
	//char* buf = new char(length);
	//memset(buf, 0, length);
	//memcpy(buf, pData, length);
	
	//stMsg* pMsg = (stMsg*)buf;
	//assert(pMsg->cSysIdentifer > INMSG_BEGIN);
	if (pData[0] <= INMSG_BEGIN)
	{
		printf("CHandler::OnHandle pInMsg->pMsg->cSysIdentifer = %d\n", pData[0]);
		return;
	}
	InternalMsg* pInMsg	= new InternalMsg;
	pInMsg->pHandler	= this;
	//pInMsg->guid		= m_peerGuid.g;
	pInMsg->nMsgLen		= length;
	pInMsg->pMsg		= (stMsg*)malloc(sizeof(char)*length);//pMsg;
	memcpy(pInMsg->pMsg, pData, length);
	
	m_pTreatorBase->Push(pInMsg);
}

void CHandler::OnDisconnect()
{
	InternalMsg* pInMsg	= new InternalMsg;
	pInMsg->pHandler	= this;
	//pInMsg->guid		= m_peerGuid.g;
	pInMsg->nMsgLen		= sizeof(stServerMsg);
	pInMsg->pMsg		= (stMsg*)malloc(sizeof(stServerMsg));//pMsg;
	stServerMsg* p 		= (stServerMsg*)(pInMsg->pMsg);
	memset(p, 0, sizeof(stServerMsg));
	p->cmdType = 255;//
	
	m_pTreatorBase->Push(pInMsg);
}

uint32_t CHandler::Send(const char *msg, const int length)
{
	return m_pIOServer->Send(msg, (const int) length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peerSystemAddress, false);
}

/*uint32_t CHandler::Send(const char *msg)
{
	return m_pIOServer->Send(msg, (const int) strlen(msg)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peerSystemAddress, true);
}*/



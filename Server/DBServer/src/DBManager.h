#pragma once
#include "RakNetTypes.h"
#include "ServerMessageDefine.h"
#include <list>
#include <map>
struct stDBResult ;
class CDBServerApp ;
class CDBManager
{
public:
	struct stArgData
	{
		RakNet::RakNetGUID m_nReqrestFromAdd ;
		unsigned int nSessionID ; // always , refer to client session with server , used in GameServer and LoginServer , to represent a Player ;
		int nExtenArg1 ; // reserver argument , for later use ;
		int nExtenArg2 ; // reserver argument , for later use ;
		stArgData(){ nSessionID = 0 ; nExtenArg2 = nExtenArg1 = 0 ;}
		void Reset(){nSessionID = 0 ; nExtenArg2 = nExtenArg1 = 0 ;}
	};
	typedef std::list<stArgData*> LIST_ARG_DATA ;
protected:
	typedef void (CDBManager::*lpMsgFunc)(RakNet::Packet*packet);
	typedef void (CDBManager::*lpDBResultFunc)( stDBResult* pResult);
	struct stMsgAndDBRetFunc
	{
		lpMsgFunc msgFunc ;
		lpDBResultFunc dbretFunc ;
	};
	typedef std::map<int,stMsgAndDBRetFunc> MAP_MSG_FUNC ;
public:
	CDBManager(CDBServerApp* theApp);
	~CDBManager();
	void Init();
	void OnMessage(RakNet::Packet* packet);
	void OnDBResult(stDBResult* pResult);
	stArgData* GetReserverArgData();
protected:
	void RegisterMsgFunc(unsigned int nMsg ,lpMsgFunc msgFunc , lpDBResultFunc bdresultFunc );
protected:
	// one request , one pair func , one recieave msg , one process DBResult for that msg ; and back message ; 
	void RequestBaseDataMsg(RakNet::Packet*pmsg);
	void RequestBasetDataDBResult(stDBResult* pResult);
protected:
	LIST_ARG_DATA m_vReserverArgData ;
	MAP_MSG_FUNC  m_vMsgFunc ;
	CDBServerApp* m_pTheApp ;
};
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
		unsigned int nSessionID ; // always , refer to client session with serveper , used in GameServer and LoginServer , to rresent a Player ;
		int nExtenArg1 ; // reserver argument , for later use ;
		int nExtenArg2 ; // reserver argument , for later use ;
		stArgData(){ nSessionID = 0 ; nExtenArg2 = nExtenArg1 = 0 ;}
		void Reset(){nSessionID = 0 ; nExtenArg2 = nExtenArg1 = 0 ;}
	};
	typedef std::list<stArgData*> LIST_ARG_DATA ;
public:
	CDBManager(CDBServerApp* theApp);
	~CDBManager();
	void Init();
	void OnMessage(RakNet::Packet* packet);
	void OnDBResult(stDBResult* pResult);
	stArgData* GetReserverArgData();
protected:
	LIST_ARG_DATA m_vReserverArgData ;
	CDBServerApp* m_pTheApp ;
};
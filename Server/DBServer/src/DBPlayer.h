#pragma once
#include <string>
#include "BaseData.h"
#include "RakNetTypes.h"
#include "DBRequest.h"
struct stAccount
{
	unsigned int nUserUID ;
	std::string strAccount ;
	std::string strPassworld ;
};

struct stDBBaseData
:public stBaseData
{

};

class CDBPlayer
{
public:
	CDBPlayer(RakNet::RakNetGUID& nFromGameServerGUID );
	~CDBPlayer();
	void SetFromServerGUID(RakNet::RakNetGUID& nFromGameServerGUID);
	RakNet::RakNetGUID& GetFromGameServerGUID();
	void OnDBResultAndProcessMsg(stDBResult* pResult );
	void OnDisconnected();
	void OnConnected();
	unsigned int GetUserUID();
protected:
	stDBBaseData m_stBaseData ;
	stAccount m_stAccount ;
	RakNet::RakNetGUID m_nFromGUID ;
	bool m_bActive ;
};
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
	enum ePlayerState
	{
		ePlayerState_None ,
		ePlayerState_Active,
		ePlayerState_Resever,
		ePlayerState_Max,
	};
public:
	CDBPlayer(RakNet::RakNetGUID& nFromGameServerGUID );
	~CDBPlayer();
	void SetFromServerGUID(RakNet::RakNetGUID& nFromGameServerGUID);
	RakNet::RakNetGUID& GetFromGameServerGUID();
	void OnDBResult(stDBResult* pResult );
	void OnDisconnected();
	void OnConnected();
	unsigned int GetUserUID();
	void OnPassAcountCheck( unsigned int nUserUID);
	ePlayerState GetState(){ return m_eState ; }
protected:
	void SetState( ePlayerState eState ){ m_eState = eState ;}
protected:
	stDBBaseData m_stBaseData ;
	stAccount m_stAccount ;
	RakNet::RakNetGUID m_nFromGUID ;
	ePlayerState m_eState ;
};
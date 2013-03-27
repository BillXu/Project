#ifndef   _SERVER_TREATOR__H_
#define   _SERVER_TREATOR__H_

#include "../Base/TreatorBase.h"
#include "../Base/ThreadMod.h"
#include <errno.h>

class  CServerTreator : public ITreatorBase, public CThreadT
{
public:
	static CServerTreator* _instance()
	{
		static CServerTreator* __inst = NULL;
		if(!__inst)
		{
			__inst = new CServerTreator;
		}
		return __inst;
	}
	CServerTreator();
	~CServerTreator();
public:
	void InitSysData();
	void LoginUserId(unsigned int userId);
	void HandleLoadAllUser(InternalMsg* pInmsg);
	void HandleSysInit(InternalMsg* pInmsg);
	void HandleLoadTopInfo(InternalMsg* pInmsg);
	void HandleSysMsg(int subCmd);

	void HandleLoginSucess(stL2G_Login* pMsg);
	void HandleCheckUser(InternalMsg* pInmsg);
	void HandleLoadGeneralInfo(InternalMsg* pInmsg);
	void HandleLoadEquipInfo(InternalMsg* pInmsg);
	void HandleLoadItemInfo(InternalMsg* pInmsg);
	void HandleLoadRelationInfo(InternalMsg* pInmsg);
	void HandleLoadShopHistoryInfo(InternalMsg* pInmsg);
	void HandleLoadChallengeInfo(InternalMsg* pInmsg);
	void HandleLoadChallTaskInfo(InternalMsg* pInmsg);
	void HandleLoadBattleInfo(InternalMsg* pInmsg);

	void LoadTopInfo(char* buf, short count);
	void SaveTopInfo();
	
	void __run();
private:
	bool m_bRun;
	
};

#define ServerTreator CServerTreator::_instance()

#endif


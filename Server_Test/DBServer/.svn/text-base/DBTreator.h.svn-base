#ifndef   _DATABASE_TREATOR__H_
#define   _DATABASE_TREATOR__H_

#include "../Base/TreatorBase.h"
#include "../Base/ThreadMod.h"
#include <errno.h>

class  CDBTreator : public ITreatorBase, public CThreadT
{
public:
	static CDBTreator* _instance()
	{
		static CDBTreator* __inst = NULL;
		if(!__inst)
		{
			__inst = new CDBTreator;
		}
		return __inst;
	}
	CDBTreator();
	~CDBTreator();
public:
	void Login(char* sql, short sqlLen, uint64_t clientGuid);
	void OnSysInit(stG2DSys* pMsg);
	int OnSaveRoleBaseInfo(stG2D_SaveRoleBase* pMsg);
	int OnSaveRoleInfo(stG2D_SaveRole* pMsg);
	int OnSaveGeneralInfo(stG2D_SaveGeneral* pMsg);
	int OnSaveEquipInfo(stG2D_SaveEquip* pMsg);
	int OnSaveItemInfo(stG2D_SaveItem* pMsg);
	int OnSaveRelationInfo(stG2D_SaveRelation* pMsg);
	int OnSaveShopHistory(stG2D_SaveShopHistory* pMsg);
	int OnSaveChallTask(stG2D_SaveChallTask* pMsg);
	int OnSaveTopInfo(stG2D_SaveTopInfo* pMsg);
	int OnSaveBattleInfo(stG2D_SaveBattle* pMsg);

	int OnSelect(stG2D_Select* pMsg);
	void CheckUserId(stG2D_Select* pMsg);
	int HandleLogin(stL2DLogin* pMsg);

	void LoadUserInfo(unsigned int userId);
	void LoadGeneralInfo(unsigned int userId);
	void LoadShopHistoryInfo(unsigned int userId);
	void LoadChallTaskInfo(unsigned int userId);
	void LoadChallengeInfo(unsigned int userId);
	void LoadBattleInfo(unsigned int userId);

	void LoadEquipInfo(unsigned int userId);
	void LoadItemInfo(unsigned int userId);
	void LoadTop50Info();
	void LoadRelationInfo(unsigned int userId);
	void __run();
private:
	bool m_bRun;
	
};

#define DBTreator CDBTreator::_instance()

#endif


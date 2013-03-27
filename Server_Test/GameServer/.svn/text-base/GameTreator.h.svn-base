#ifndef   _GAME_TREATOR__H_
#define   _GAME_TREATOR__H_

#include "../Base/TreatorBase.h"
#include "../Base/ThreadMod.h"
#include <errno.h>
#include "../Share/Common.h"
#include "../Base/InternalMsg.h"

class CGameTreator;
struct MsgHandler
{
	void (CGameTreator::*handler)(InternalMsg* pInmsg);
};

class  CGameTreator : public ITreatorBase, public CThreadT
{
public:
	static CGameTreator* _instance()
	{
		static CGameTreator* __inst = NULL;
		if(!__inst)
		{
			__inst = new CGameTreator;
		}
		return __inst;
	}
	CGameTreator();
	~CGameTreator();

	static void InitPacketHandlerTable();
public:
	void OnLogout(InternalMsg* pInmsg);
	void OnCheckUser(InternalMsg* pInmsg);
	void CheckUser(uint32_t uId, CHandler* pHandler);
	void OnTaskInfo(InternalMsg* pInmsg);
	void OnTask(InternalMsg* pInmsg);
	
	void OnItemList(InternalMsg* pInmsg);
	void OnGodPlay(InternalMsg* pInmsg);
	void OnEquipPlay(InternalMsg* pInmsg);
	void OnGodBack(InternalMsg* pInmsg);
	void OnEquipBack(InternalMsg* pInmsg);
	void OnGodUpLev(InternalMsg* pInmsg);
	void OnEquipUpLev(InternalMsg* pInmsg);
	void OnGeneralChangeSkill(InternalMsg* pInmsg);

	void OnGodCrack(InternalMsg* pInmsg);
	void OnEquipCrack(InternalMsg* pInmsg);

	void OnTopUserList(InternalMsg* pInmsg);
	void OnGetDailyReward(InternalMsg* pInmsg);

	void OnGetChallTaskList(InternalMsg* pInmsg);
	void OnFlushChallTaskList(InternalMsg* pInmsg);
	void OnWarUserList(InternalMsg* pInmsg);
	void OnUserChallenge(InternalMsg* pInmsg);
	void OnUserChallengeRes(InternalMsg* pInmsg);
	void OnChallTaskReward(InternalMsg* pInmsg);

	void OnHonorExcList(InternalMsg* pInmsg);
	void OnHonorExchange(InternalMsg* pInmsg);
	void OnUseGift(InternalMsg* pInmsg);
	void OnUseItem(InternalMsg* pInmsg);

	void OnShopList(InternalMsg* pInmsg);
	void OnShopBuy(InternalMsg* pInmsg);

	void OnBattleList(InternalMsg* pInmsg);
	void OnDoBattle(InternalMsg* pInmsg);
	void OnBattleReset(InternalMsg* pInmsg);
public:
	void __run();
private:
	bool m_bRun;
	
};

#define GameTreator CGameTreator::_instance()

#endif


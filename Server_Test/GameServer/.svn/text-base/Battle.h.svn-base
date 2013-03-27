#ifndef _BATTLE__H_
#define _BATTLE__H_

#include "../Base/Define.h"
#include <map>
#include "../Share/mutex.h"
#include "../Base/InternalMsg.h"

#define BATTLE_RESET_ITEM	1104

class CBattle
{
public:
	CBattle(unsigned short mainId, unsigned short curLeafId=0);
	CBattle(stBattleInfo* pInfo);
	~CBattle();
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline bool IsDelete()
	{
		return (changeFlag == 2);
	}
	inline void SetChangeFlag()
	{
		changeFlag = 1;
	}
	inline void SetDelFlag()
	{
		changeFlag = 2;
	}
	inline void SetAddFlag()
	{
		changeFlag = 3;
	}
	inline void ResetFlag()
	{
		changeFlag = 0;
	}
public:
	inline void Update(unsigned short curId)
	{
		m_info.curLeafId = curId;
		SetChangeFlag();
	}
public:
	t_BattleInfo m_info;
protected:
	unsigned char changeFlag;	//3Ôö ; 2 É¾ ;1 ¸Ä
};

class CBattleMgr
{
public:
	CBattleMgr();
	~CBattleMgr();

	bool CheckOpenBattle(unsigned int userId, unsigned short level);
	int FillBattleInfo(char* buf, unsigned short* count);
	int Add(unsigned short mainId, unsigned short curId);
	void Add(CBattle* pBattle);
	void Del(CBattle* pObj);
	CBattle* Get(unsigned short mainId);
	bool Update(unsigned short mainId, unsigned short curId);

public:
	short CopyBattleInfo(char* buf, short bufLen);
	unsigned int GetResetLeftTime();
	void Reset();
private:
	typedef std::map<unsigned short,CBattle*>  MAP_BATTLE;
	MAP_BATTLE m_mapBattle;
	bool m_bIsAllOpen;
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
	Mutex m_mutex;
	unsigned int m_lastResetTime;
};

#endif

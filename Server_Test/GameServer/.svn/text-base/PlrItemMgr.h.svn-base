#ifndef _ITEM_MGR__H_
#define _ITEM_MGR__H_

#include "../Share/Common.h"
#include "../Share/mutex.h"
#include "General.h"
#include "Equip.h"
#include "CommItem.h"
#include "ShopConfig.h"
#include <map>
using namespace std;

class CUser;
class CPlrItemMgr
{
public:
	short CopyGeneralInfo(unsigned int roleId, char* buf, short bufLen);
	short CopyEquipInfo(unsigned int roleId, char* buf, short bufLen);
	short CopyItemInfo(unsigned int roleId, char* buf, short bufLen);
	int FillGeneralInfo(char* buf, unsigned short* count);
	int FillEquipInfo(char* buf, unsigned short* count);
	int FillItemInfo(char* buf, unsigned short* count);
public:
	IItemBase* AddNewItem(uint32_t guid, int count=1, unsigned int godSkill=0);
	int AddItem(IItemBase* pItem, bool bCommFlag = false);
	IItemBase* GetItem(eItemType type, uint32_t serialNo);
	//IItemBase* GetItemByGuid(eItemType type, uint32_t guid);
	//IItemBase* GetItemBase(uint32_t guid);
	IItemBase* GetItem(uint32_t index, bool bCommFlag = false);
	CGeneral* GetGeneral(uint32_t serialNo);
	CCommItem* GetMaterial(uint32_t itemId);
	CCommItem* GetGiftItem(uint32_t itemId);
	CCommItem* GetResActItem(uint32_t itemId);
	CCommItem* GetAddChallItem(uint32_t itemId);
	CEquip* GetEquip(uint32_t serialNo);
	//void ResetGeneral(CGeneral* pGen);
	//void SetDelFlag(IItemBase* pItem);
	int DelItem(uint32_t serialNo, eItemType type);
	void DelItem(IItemBase* pItem, bool bCommFlag = false);
	int ReduceCommItem(uint32_t guid, int count);
	IItemBase* Add(uint32_t guid, int count=1);

	unsigned int GetItemPrice(stCommodityConfig* pConfig);
public:
	void SetUser(CUser* pUser)
	{
		assert(pUser);
		m_pUser = pUser;
	}
	CPlrItemMgr();
	~CPlrItemMgr();
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
private:
	Mutex m_mutex;
	CUser* m_pUser;
public:
	typedef std::multimap<uint32_t, IItemBase*> MULTIMAP_ITEMS;
	typedef std::map<uint32_t, IItemBase*> MAP_ITEMS;
	MULTIMAP_ITEMS m_vItems[eItem_Max];
	MAP_ITEMS m_vItems2[eItem_Max];
	//MAP_ITEMS m_vDelItems[eItem_Max];
	//MAP_ITEMS m_vAddItems[eItem_Max];
	//std::map<uint32_t,IItemBase*> m_ItemMap;
};

#endif


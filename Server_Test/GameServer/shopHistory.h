
#ifndef _SHOP_HISTORY__H_
#define _SHOP_HISTORY__H_

#include "../Base/Define.h"
#include "../Base/InternalMsg.h"
#include "../Share/mutex.h"

class CShopHistory
{
public:
	CShopHistory(unsigned int guid);
	CShopHistory(stShopHistoryInfo* pInfo);
	~CShopHistory();
	
	unsigned short GetCount();
	unsigned short AddCount(unsigned short n = 1);
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
	t_ShopHistoryInfo m_info;
protected:
	unsigned char changeFlag;	//3Ôö ; 2 É¾ ;1 ¸Ä
};

class CShopHistoryMgr
{
public:
	int Add(unsigned int roleId, unsigned int guid, unsigned int price, unsigned char priceType);
	int Add(CShopHistory* rel);
	int Del(CShopHistory* rel);

	short CopyShopHistoryInfo(unsigned int roleId, char* buf, short bufLen);

	CShopHistory* GetShopHistory(unsigned int id);
	unsigned int GetBuyCount(unsigned int id);
public:
	CShopHistoryMgr();
	~CShopHistoryMgr();
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
public:
	typedef std::map<uint32_t, CShopHistory*> MAP_SHOP;
	MAP_SHOP m_shopHistory;
};

#endif

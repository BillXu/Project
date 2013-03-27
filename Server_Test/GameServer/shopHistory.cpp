#include "shopHistory.h"
#include "UserMgr.h"

CShopHistory::CShopHistory(unsigned int guid)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.guid = guid;
	m_info.count = 1;
	changeFlag = 0;
}

CShopHistory::CShopHistory(stShopHistoryInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.roleId = pInfo->roleId;
	m_info.guid = pInfo->guid;
	m_info.count = pInfo->count;
	m_info.priceType = pInfo->priceType;
	m_info.price = pInfo->price;
	m_info.lastTime = pInfo->lastTime;
	changeFlag = 0;
}

CShopHistory::~CShopHistory()
{
}

unsigned short CShopHistory::GetCount()
{
	return m_info.count;
}
unsigned short  CShopHistory::AddCount(unsigned short n)
{
	m_info.count += n;
	return m_info.count;
}

CShopHistoryMgr::CShopHistoryMgr()
{
	//
}

CShopHistoryMgr::~CShopHistoryMgr()
{
	//
}

int CShopHistoryMgr::Add(unsigned int roleId, unsigned int guid, unsigned int price, unsigned char priceType)
{
	CShopHistory* pSH = GetShopHistory(guid);
	if (pSH && pSH->GetCount() > 0)
	{
		Lock();
		pSH->AddCount();
		pSH->SetChangeFlag();
		UnLock();
		return pSH->GetCount(); 
	} 
	else
	{
		CShopHistory* pSH = new CShopHistory(guid);
		pSH->m_info.roleId = roleId;
		pSH->m_info.priceType = priceType;
		pSH->m_info.price = price;
		pSH->m_info.lastTime = getSecTime();
		pSH->SetAddFlag();
		CUser* pUser = g_UserMgr->GetUser(roleId);
		if (pUser)
		{
			pUser->SaveShopHistory();
		}
		Add(pSH);
		return pSH->GetCount();
	}
}

int CShopHistoryMgr::Add(CShopHistory* shop)
{
	Lock();
	m_shopHistory.insert(MAP_SHOP::value_type(shop->m_info.guid, shop));
	UnLock();
	return m_shopHistory.size();
}

int CShopHistoryMgr::Del(CShopHistory* shop)
{
	Lock();
	MAP_SHOP::iterator it = m_shopHistory.find(shop->m_info.guid);
	if (it != m_shopHistory.end())
	{
		m_shopHistory.erase(it);
	}
	UnLock();
	return m_shopHistory.size();
}

short CShopHistoryMgr::CopyShopHistoryInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CShopHistory* pObj = NULL;
	stShopHistoryInfo* pInfo = NULL;
	Lock();
	MAP_SHOP::iterator ipos = m_shopHistory.begin();
	MAP_SHOP::iterator iend = m_shopHistory.end();
	while(ipos != iend)
	{
		pObj = (CShopHistory*)(ipos->second);
		if( pObj->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stShopHistoryInfo*)p;
			pInfo->changeFlag = pObj->GetChangeFlag();
			pInfo->roleId = pObj->m_info.roleId;
			pInfo->guid = pObj->m_info.guid;
			pInfo->count = pObj->m_info.count;
			pInfo->priceType = pObj->m_info.priceType;
			pInfo->price = pObj->m_info.price;
			pInfo->lastTime = pObj->m_info.lastTime;
			p += sizeof(stShopHistoryInfo);
			if (pObj->IsDelete())
			{
				Del(pObj);
			}
			pObj->ResetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}

CShopHistory* CShopHistoryMgr::GetShopHistory(unsigned int id)
{
	CShopHistory* pRet = NULL;
	Lock();
	MAP_SHOP::iterator ipos = m_shopHistory.find(id);
	if (ipos != m_shopHistory.end())
	{
		pRet = ipos->second;
	}
	UnLock();
	return pRet;
}

unsigned int CShopHistoryMgr::GetBuyCount(unsigned int id)
{
	CShopHistory* pSH = GetShopHistory(id);
	if (!pSH)
	{
		return 0;
	} 
	else
	{
		return pSH->GetCount();
	}
}





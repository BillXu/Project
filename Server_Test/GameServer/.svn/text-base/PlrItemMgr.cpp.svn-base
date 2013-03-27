#include "PlrItemMgr.h"
#include "ItemManager.h"
#include "../Base/InternalMsg.h"
#include "User.h"

CPlrItemMgr::CPlrItemMgr()
{
	m_pUser = NULL;
}

CPlrItemMgr::~CPlrItemMgr()
{
}

short CPlrItemMgr::CopyGeneralInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CGeneral* pGeneral = NULL;
	stGeneralInfo* pInfo = NULL;
	Lock();
	MAP_ITEMS::iterator ipos = m_vItems2[eItem_God].begin();
	MAP_ITEMS::iterator iend = m_vItems2[eItem_God].end();
	while(ipos != iend)
	{
		pGeneral = (CGeneral*)(ipos->second);
		if( pGeneral->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stGeneralInfo*)p;
			pInfo->changeFlag = pGeneral->GetChangeFlag();
			pInfo->serialNo = pGeneral->GetSerialNo();
			pInfo->roleId = roleId;
			pInfo->itemId = pGeneral->m_info.itemId;
			pInfo->guid = pGeneral->GetGuid();
			pInfo->warFlag = pGeneral->m_info.warFlag;
			pInfo->group = pGeneral->m_info.group;
			pInfo->curStar = pGeneral->m_info.curStar;
			pInfo->curLevel = pGeneral->m_info.curLevel;
			pInfo->godSkill = pGeneral->m_info.godSkill;
			p += sizeof(stGeneralInfo);
			if (pGeneral->IsDelete())
			{
				DelItem(pGeneral);
			}
			pGeneral->UnsetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}

short CPlrItemMgr::CopyEquipInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;

	CEquip* pItem = NULL;
	stEquipInfo* pInfo = NULL;
	Lock();
	MAP_ITEMS::iterator ipos = m_vItems2[eItem_Equip].begin();
	MAP_ITEMS::iterator iend = m_vItems2[eItem_Equip].end();	
	while(ipos != iend)
	{
		pItem = (CEquip*)(ipos->second);
		if( pItem->GetChangeFlag() > 0 )
		{
			++count;
			pInfo = (stEquipInfo*)p;
			pInfo->changeFlag = pItem->GetChangeFlag();
			pInfo->serialNo = pItem->GetSerialNo();
			pInfo->type = (unsigned char)(pItem->GetEquipType());
			pInfo->roleId = roleId;
			pInfo->itemId = pItem->m_info.itemId;
			pInfo->guid = pItem->GetGuid();
			pInfo->warFlag = pItem->m_info.warFlag;
			pInfo->curStar = pItem->m_info.curStar;
			pInfo->curLevel = pItem->m_info.curLevel;
			p += sizeof(stEquipInfo);
			if (pItem->IsDelete())
			{
				DelItem(pItem);
			}
			pItem->UnsetFlag();
		}
		++ipos;
	}
	UnLock();
	return count;
}

short CPlrItemMgr::CopyItemInfo(unsigned int roleId, char* buf, short bufLen)
{
	assert(buf);
	char* p = buf;
	short count = 0;
	Lock();
	for(int i = eItem_Material; i < eItem_Max; i++)
	{
		MULTIMAP_ITEMS::iterator ipos = m_vItems[i].begin();
		MULTIMAP_ITEMS::iterator iend = m_vItems[i].end();
		CCommItem* pItem = NULL;
		stItemInfo* pInfo = NULL;
		while(ipos != iend)
		{
			pItem = (CCommItem*)(ipos->second);
			if( pItem->GetChangeFlag() > 0 )
			{
				++count;
				pInfo = (stItemInfo*)p;
				pInfo->changeFlag = pItem->GetChangeFlag();
				pInfo->type = (unsigned char)(pItem->GetType());
				pInfo->roleId = roleId;
				pInfo->itemId = pItem->GetGuid();
				pInfo->count = pItem->GetCount();
				p += sizeof(stItemInfo);
				if (pItem->IsDelete())
				{
					DelItem(pItem, true);
				}
				pItem->UnsetFlag();
			}
			++ipos;
		}
	}
	UnLock();
	return count;
}

int CPlrItemMgr::FillGeneralInfo(char* buf, unsigned short* count)
{
	MAP_ITEMS::iterator ipos = m_vItems2[eItem_God].begin();
	MAP_ITEMS::iterator iend = m_vItems2[eItem_God].end();
	int offset = 0;
	t_GodInfo* pTmp = NULL;
	CGeneral* pInfo = NULL;
	int n = 0;
	while(ipos != iend)
	{
		pTmp = (t_GodInfo*)(buf+offset);
		pInfo = (CGeneral*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			pTmp->serialNo = pInfo->GetSerialNo();
			pTmp->guid = pInfo->GetGuid();
			pTmp->warFlag = pInfo->m_info.warFlag;
			//pTmp->curStar = pInfo->m_info.curStar;
			//pTmp->curLevel = pInfo->m_info.curLevel;
			pTmp->godSkill = pInfo->m_info.godSkill;
			offset += sizeof(t_GodInfo);
		}
		++ipos;
	}
	*count = n;
	return offset;
}

int CPlrItemMgr::FillEquipInfo(char* buf, unsigned short* count)
{
	MAP_ITEMS::iterator ipos = m_vItems2[eItem_Equip].begin();
	MAP_ITEMS::iterator iend = m_vItems2[eItem_Equip].end();
	int offset = 0;
	t_EquipInfo* pTmp = NULL;
	CEquip* pInfo = NULL;
	int n = 0;
	while(ipos != iend)
	{
		pTmp = (t_EquipInfo*)(buf+offset);
		pInfo = (CEquip*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			//pInfo->Lock();
			pTmp->serialNo = pInfo->GetSerialNo();
			pTmp->guid = pInfo->GetGuid();
			pTmp->warFlag = pInfo->m_info.warFlag;
			//pInfo->UnLock();
			offset += sizeof(t_EquipInfo);
		}
		++ipos;
	}
	*count = n;
	return offset;
}

int CPlrItemMgr::FillItemInfo(char* buf, unsigned short* count)
{
	int offset = 0;
	int n = 0;
	t_ItemInfo* pTmp = NULL;
	CCommItem* pInfo = NULL;

	MULTIMAP_ITEMS::iterator ipos = m_vItems[eItem_Material].begin();
	MULTIMAP_ITEMS::iterator iend = m_vItems[eItem_Material].end();
	while(ipos != iend)
	{
		pTmp = (t_ItemInfo*)(buf+offset);
		pInfo = (CCommItem*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			pInfo->Lock();
			pTmp->itemId = pInfo->GetGuid();
			pTmp->type = pInfo->m_info.type;
			pTmp->count = pInfo->GetCount();
			pInfo->UnLock();
			offset += sizeof(t_ItemInfo);
		}
		++ipos;
	}
	ipos = m_vItems[eItem_RestoreActive].begin();
	iend = m_vItems[eItem_RestoreActive].end();
	while(ipos != iend)
	{
		pTmp = (t_ItemInfo*)(buf+offset);
		pInfo = (CCommItem*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			pInfo->Lock();
			pTmp->itemId = pInfo->GetGuid();
			pTmp->type = pInfo->m_info.type;
			pTmp->count = pInfo->GetCount();
			pInfo->UnLock();
			offset += sizeof(t_ItemInfo);
		}
		++ipos;
	}
	ipos = m_vItems[eItem_AddChallengeTimes].begin();
	iend = m_vItems[eItem_AddChallengeTimes].end();
	while(ipos != iend)
	{
		pTmp = (t_ItemInfo*)(buf+offset);
		pInfo = (CCommItem*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			pInfo->Lock();
			pTmp->itemId = pInfo->GetGuid();
			pTmp->type = pInfo->m_info.type;
			pTmp->count = pInfo->GetCount();
			pInfo->UnLock();
			offset += sizeof(t_ItemInfo);
		}
		++ipos;
	}
	ipos = m_vItems[eItem_Gift].begin();
	iend = m_vItems[eItem_Gift].end();
	while(ipos != iend)
	{
		pTmp = (t_ItemInfo*)(buf+offset);
		pInfo = (CCommItem*)(ipos->second);
		if (!pInfo->IsDelete())
		{
			++n;
			pInfo->Lock();
			pTmp->itemId = pInfo->GetGuid();
			pTmp->type = pInfo->m_info.type;
			pTmp->count = pInfo->GetCount();
			pInfo->UnLock();
			offset += sizeof(t_ItemInfo);
		}
		++ipos;
	}

	*count = n;
	printf("CPlrItemMgr::FillItemInfo item count=%u\n", n);
	return offset;
}

IItemBase* CPlrItemMgr::AddNewItem(uint32_t guid, int count, unsigned int godSkill)
{
	CItem* pStatic = CItemManager::SharedItemMgr()->GetItemByUID(guid);
	assert(pStatic);
	printf("CPlrItemMgr::AddNewItem type=%d\n", pStatic->m_eItemType);
	switch (pStatic->m_eItemType)
	{
		case eItem_God:
		{
			CGeneral* p = new CGeneral(guid);
			p->SetType(pStatic->m_eItemType);
			p->Add();
			p->SetSerialNo();
			p->SetGodSkill(godSkill);
			p->SetAddFlag();

			AddItem(p);
			m_pUser->SaveGeneralInfo();
			return (IItemBase*)p;
		}
		break;
		case eItem_Equip:
		{
			CEquip* p = new CEquip(guid);
			p->SetType(pStatic->m_eItemType);
			p->SetEquipType(((stEquip*)pStatic)->eType);
			p->Add();
			p->SetSerialNo();
			p->SetAddFlag();
			AddItem(p);
			m_pUser->SaveEquipInfo();
			return (IItemBase*)p;
		}
		break;
		case eItem_Material:
		case eItem_RestoreActive:
		case eItem_AddChallengeTimes:
		case eItem_Gift:
		{
			CCommItem* p = new CCommItem(guid);
			p->SetType(pStatic->m_eItemType);
			p->Add(count);
			p->SetAddFlag();
			AddItem(p,true);
			m_pUser->SaveCommItem();
			return (IItemBase*)p;
		}
		break;
		default:
		{
			assert(0 &&"unknow itemIType") ;
		}
		break;
	}
	return NULL;
}

int CPlrItemMgr::AddItem(IItemBase* pItem, bool bCommFlag)
{
	if ( !pItem ) return -1;
	eItemType type = pItem->GetType();
	Lock();
	if(bCommFlag)
	{
		MULTIMAP_ITEMS* pVitem = &(m_vItems[type]);
		pVitem->insert(MULTIMAP_ITEMS::value_type(pItem->GetGuid(),pItem));
	}
	else
	{
		MAP_ITEMS* pVitem2 = &(m_vItems2[type]);
		pVitem2->insert(MAP_ITEMS::value_type(pItem->GetSerialNo(),pItem));
	}
	UnLock();
	return pItem->GetCount();
}

/*void CPlrItemMgr::SetDelFlag(IItemBase* pItem)
{
	if(!pItem) return;
	Lock();
	eItemType type = pItem->GetType();

	MULTIMAP_ITEMS::iterator iter = m_vItems[type].find(pItem->GetGuid());
	if(iter != m_vItems[type].end()) 
	{
		m_vItems[type].erase(iter);
	}
	MAP_ITEMS::iterator iter2 = m_vItems2[type].find(pItem->GetSerialNo());
	if(iter2 != m_vItems2[type].end()) 
	{
		m_vItems2[type].erase(iter2);
	}
	UnLock();
}*/

void CPlrItemMgr::DelItem(IItemBase* pItem, bool bCommFlag)
{
	if(!pItem) return;
	printf(" CPlrItemMgr::DelItem(%u) \n", pItem->GetGuid());
	eItemType type = pItem->GetType();
	Lock();
	if(bCommFlag)
	{
		MULTIMAP_ITEMS::iterator iter = m_vItems[type].find(pItem->GetGuid());
		if(iter != m_vItems[type].end()) 
		{
			m_vItems[type].erase(iter);
		}
	}
	else
	{
		MAP_ITEMS::iterator iter2 = m_vItems2[type].find(pItem->GetSerialNo());
		if(iter2 != m_vItems2[type].end()) 
		{
			m_vItems2[type].erase(iter2);
		}
	}
	
	UnLock();
}

int CPlrItemMgr::ReduceCommItem(uint32_t guid, int count)
{
	IItemBase* pItem = GetItem(guid, true);
	if (pItem->GetCount() > (unsigned int)count)
	{
		pItem->Lock();
		pItem->Reduce(count);
		pItem->SetChangeFlag();
		pItem->UnLock();
		return pItem->GetCount();
	} 
	else if(pItem->GetCount() == (unsigned int)count)
	{
		//DelItem(pItem, true);
		pItem->Lock();
		pItem->Reduce(count);
		pItem->SetDelFlag();
		pItem->UnLock();
		return 0;
	}
	else
	{
		printf("Err: CPlrItemMgr::ReduceCommItem return -1\n");
		return -1;
	}
}

IItemBase* CPlrItemMgr::Add(uint32_t guid, int count)
{
	IItemBase* pItem = GetItem(guid, true);
	if (pItem && pItem->GetCount() > 0)
	{
		pItem->Lock();
		pItem->Add(count);
		pItem->SetChangeFlag();
		pItem->UnLock();
		return pItem;
	} 
	else
	{
		return AddNewItem(guid, count);
	}
}

unsigned int CPlrItemMgr::GetItemPrice(stCommodityConfig* pConfig)
{
	CItem* pItem = CItemManager::SharedItemMgr()->GetItemByUID(pConfig->nItemUID);
	int coinPrice = 0;
	int goldPrice = 0;
	switch(pItem->m_eItemType)
	{
	case eItem_Material:
		{
			CMaterialItem* pObj = (CMaterialItem*)pItem;
			coinPrice = pObj->m_nCoinPrice;
			goldPrice = pObj->m_nGoldenPrice;
		}
		break;
	case eItem_RestoreActive:
		{
			CActiveRestoreItem* pObj = (CActiveRestoreItem*)pItem;
			coinPrice = pObj->m_nCoinPrice;
			goldPrice = pObj->m_nGoldenPrice;
		}
		break;
	case eItem_AddChallengeTimes:
		{
			CAddChallengeItem* pObj = (CAddChallengeItem*)pItem;
			coinPrice = pObj->m_nCoinPrice;
			goldPrice = pObj->m_nGoldenPrice;
		}
		break;
	case eItem_Gift:
		{
			CGiftItem* pObj = (CGiftItem*)pItem;
			coinPrice = pObj->m_nCoinPrice;
			goldPrice = pObj->m_nGoldenPrice;
		}
		break;
	default:
		break;
	}
	unsigned int price = 0;
	if (pConfig->IsCoinType())
	{
		price = (unsigned int)((pConfig->nSale / WEIGHT_SUM) * coinPrice);
	} 
	else
	{
		price = (unsigned int)((pConfig->nSale / WEIGHT_SUM) * goldPrice);
	}

	return price;
}

int CPlrItemMgr::DelItem(uint32_t serialNo, eItemType type)
{
	IItemBase* pItem = GetItem(type, serialNo);
	if(pItem)
	{
		DelItem(pItem);
	}
	return 0;
}

IItemBase* CPlrItemMgr::GetItem(eItemType type, uint32_t index)
{
	IItemBase* pItem = NULL;
	Lock();
	if(type <= eItem_Equip)
	{
		MAP_ITEMS::iterator iter = m_vItems2[type].find(index);
		if ( iter != m_vItems2[type].end() )
			pItem = iter->second;
	}
	else
	{
		MULTIMAP_ITEMS::iterator iter = m_vItems[type].find(index);
		if ( iter != m_vItems[type].end() )
			pItem = iter->second;
	}
	UnLock();
    return pItem;
}

/*IItemBase* CPlrItemMgr::GetItemByGuid(eItemType type, uint32_t guid)
{
	IItemBase* pItem = NULL;
	Lock();
    MULTIMAP_ITEMS::iterator iter = m_vItems[type].find(guid);
	if ( iter != m_vItems[type].end() )
		pItem = iter->second;
	UnLock();
    return pItem;
}*/

/*IItemBase* CPlrItemMgr::GetItemBase(uint32_t guid)
{
	IItemBase* pItem = NULL;
	pItem = GetItem(guid);
	if (pItem)
	{
		return pItem;
	} 
	else
	{
		pItem = GetItem(guid, true);
	}
	return pItem;
}*/

IItemBase* CPlrItemMgr::GetItem(uint32_t index, bool bCommFlag)
{
	IItemBase* pItem = NULL;
	Lock();
    MAP_ITEMS::iterator iter;
	if (bCommFlag)
	{
		for ( int i = eItem_Material; i < eItem_Max; ++i )
		{
			iter = m_vItems[i].find(index);
			if ( iter != m_vItems[i].end() )
				pItem = iter->second;
		}
	}
	else
	{
		for ( int i = eItem_None; i < eItem_Material; ++i )
		{
			iter = m_vItems2[i].find(index);
			if ( iter != m_vItems2[i].end() )
				pItem = iter->second;
		}
	}
	UnLock();
    return pItem;
}

CGeneral* CPlrItemMgr::GetGeneral(uint32_t serialNo)
{
	return (CGeneral*)GetItem(eItem_God, serialNo);
}

CCommItem* CPlrItemMgr::GetMaterial(uint32_t itemId)
{
	return (CCommItem*)GetItem(eItem_Material, itemId);
}

CCommItem* CPlrItemMgr::GetGiftItem(uint32_t itemId)
{
	return (CCommItem*)GetItem(eItem_Gift, itemId);
}

CCommItem* CPlrItemMgr::GetResActItem(uint32_t itemId)
{
	return (CCommItem*)GetItem(eItem_RestoreActive, itemId);
}

CCommItem* CPlrItemMgr::GetAddChallItem(uint32_t itemId)
{
	return (CCommItem*)GetItem(eItem_AddChallengeTimes, itemId);
}

CEquip* CPlrItemMgr::GetEquip(uint32_t serialNo)
{
	return (CEquip*)GetItem(eItem_Equip, serialNo);
}

/*void CPlrItemMgr::ResetGeneral(CGeneral* pGen)
{
	//unsigned int oldNo = 0;
	Lock();
	//oldNo = GetSerialNo();
	//ChangeNo();
	//m_vItems[eItem_God][pGen->GetGuid()] = (IItemBase*)pGen;
	UnLock();
}*/




















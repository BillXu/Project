
#ifndef _EQUIP__H_
#define _EQUIP__H_

#include "ItemBase.h"
#include "../Base/InternalMsg.h"

class CEquip : public IItemBase
{
public:
	inline int GetEquipType()
	{
		return m_info.type;
	}
	inline void SetEquipType(int equipType)
	{
		m_info.type = (unsigned char)equipType;
	}
	inline unsigned char GetWarFlag()
	{
		return m_info.warFlag;
	}
	inline unsigned int GetId()
	{
		return m_info.itemId;
	}
public:
	CEquip(unsigned int guid);
	CEquip(stEquipInfo* pInfo);
	~CEquip();
public:
	t_EQUIPINFO m_info;
};

#endif

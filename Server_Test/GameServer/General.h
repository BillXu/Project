
#ifndef _GENERAL__H_
#define _GENERAL__H_

#include "ItemBase.h"
#include "../Base/InternalMsg.h"

class CGeneral : public IItemBase
{
public:
    CGeneral();
	CGeneral(unsigned int guid);
	CGeneral(stGeneralInfo* pInfo);
	~CGeneral();
	
	unsigned int GetGodSkill();
	void SetGodSkill(unsigned int skill);
	unsigned char GetWarFlag();
	eCardGroupType GetGroupType();
public:
	t_GeneralInfo m_info;
};

#endif

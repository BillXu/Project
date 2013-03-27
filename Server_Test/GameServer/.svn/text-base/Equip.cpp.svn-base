
#include "Equip.h"
CEquip::CEquip(unsigned int guid)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.itemId = (unsigned int)(guid/100);	//guid Ç°3Î»¼´ÎªitemId
	SetGuid(guid);
}

CEquip::CEquip(stEquipInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	SetGuid(pInfo->guid);
	m_info.type = pInfo->type;
	m_info.roleId = pInfo->roleId;
	m_info.itemId = pInfo->itemId;
	m_info.warFlag = pInfo->warFlag;
	m_info.curStar = pInfo->curStar;
	m_info.curLevel = pInfo->curLevel;
}

CEquip::~CEquip()
{

}
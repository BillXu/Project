
#include "General.h"
CGeneral::CGeneral()
{
	memset(&m_info, 0, sizeof(m_info));
}

CGeneral::CGeneral(unsigned int guid)
{
	memset(&m_info, 0, sizeof(m_info));
	m_info.itemId = (unsigned int)(guid/1000);	//guid Ç°4Î»¼´ÎªitemId
	SetGuid(guid);
}

CGeneral::CGeneral(stGeneralInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	SetGuid(pInfo->guid);
	m_info.serialNo = pInfo->serialNo;
	m_info.roleId = pInfo->roleId;
	m_info.itemId = pInfo->itemId;
	m_info.warFlag = pInfo->warFlag;
	m_info.group = (eCardGroupType)(pInfo->group);
	m_info.curStar = pInfo->curStar;
	m_info.curLevel = pInfo->curLevel;
	m_info.godSkill = pInfo->godSkill;
}

CGeneral::~CGeneral()
{

}

unsigned int CGeneral::GetGodSkill()
{
	return m_info.godSkill;
}
void CGeneral::SetGodSkill(unsigned int skill)
{
	m_info.godSkill = skill;
}

unsigned char CGeneral::GetWarFlag()
{
	return m_info.warFlag;
}

eCardGroupType CGeneral::GetGroupType()
{
	return m_info.group;
}



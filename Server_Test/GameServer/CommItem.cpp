
#include "CommItem.h"
CCommItem::CCommItem(unsigned int guid)
{
	memset(&m_info, 0, sizeof(m_info));
	SetGuid(guid);
}

CCommItem::CCommItem(stItemInfo* pInfo)
{
	memset(&m_info, 0, sizeof(m_info));
	SetGuid(pInfo->itemId);
	m_info.roleId = pInfo->roleId;
	//m_info.type = pInfo->type;
	//m_info.count = pInfo->count;
}

CCommItem::~CCommItem()
{

}





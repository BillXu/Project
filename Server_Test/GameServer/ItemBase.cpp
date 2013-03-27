
#include "ItemBase.h"
#include "StartServer.h"

IItemBase::IItemBase()
{
	m_serialNo = 0;
	changeFlag = 0;
	m_guid = 0;
	m_itemType = eItem_None;
	m_count = 0;
}

IItemBase::~IItemBase()
{
}

unsigned int IItemBase::CreateSerialNo()
{
	printf("Info: IItemBase::CreateSerialNo %u\n", g_GodSerialNoBegin);
	return (++g_GodSerialNoBegin);
}


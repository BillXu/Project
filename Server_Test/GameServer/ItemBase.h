
#ifndef _ITEM_BASE__H_
#define _ITEM_BASE__H_

#include "../Base/Define.h"
#include "../Share/mutex.h"

class IItemBase
{
public:
    IItemBase();
    virtual ~IItemBase();
	
	inline void SetGuid(unsigned int id)
	{ m_guid = id; }
	inline unsigned int GetGuid()
	{ return m_guid; }
	
	inline void SetType(eItemType type)
	{ m_itemType = type;}
	inline eItemType GetType()
	{ return m_itemType; }
	
	inline void Add(int n=1)
	{ m_count += n; }
	inline void Reduce(int n)
	{
		m_count -= n;
	}
	inline unsigned int GetCount()
	{ return m_count; }
	
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline bool IsDelete()
	{
		unsigned char flag = 0;
		Lock();
		flag = changeFlag;
		UnLock();
		return (flag == 2);
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
	inline void UnsetFlag()
	{
		changeFlag = 0;
	}
	inline void SetSerialNo(unsigned int no)
	{
		m_serialNo = no;
	}
	inline unsigned int SetSerialNo()
	{
		m_serialNo = CreateSerialNo();
		return m_serialNo;
	}
	inline unsigned int GetSerialNo()
	{
		return m_serialNo;
	}
	inline unsigned int ChangeNo()
	{
		return ChangeNo(CreateSerialNo());
	}
	inline unsigned int ChangeNo(unsigned int newNo)
	{
		unsigned int old = m_serialNo;
		m_serialNo = newNo;
		return old;
	}
	unsigned int CreateSerialNo();
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
protected:
	unsigned int m_serialNo;
    unsigned int m_guid;
    eItemType m_itemType ;
    int m_count; //堆叠数；
	unsigned char changeFlag;	//3增 ; 2 删 ;3 改

	Mutex m_mutex;
};

#endif 

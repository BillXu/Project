#pragma once 
#include <string>
#include <list>
enum eValueType
{
	eValue_Float,
	eValue_Int,
	eValue_Char,
	eValue_UInt,
	eValue_Max,
};
struct stMysqlField
{
	std::string strFieldName ;
	eValueType eType ;
	union 
	{
		float fValue ;
		int iValue ;
		unsigned int uIvlue ;
		char* pBuffer;
	} Value ;
	int nBufferLen ;
public:
	stMysqlField(){Value.pBuffer = NULL ;}
	~stMysqlField();
};

class CMysqlRow
{
public:
	typedef std::list<stMysqlField*> LIST_FIELD ;
public:
	CMysqlRow();
	~CMysqlRow();
	void PushFiled(stMysqlField* pFiled );
	stMysqlField* GetFiledByName( const char* pFiledName );
	stMysqlField* operator [](const char* pFieldname );
	int GetFieldCount(){return m_vField.size() ;}
protected:
	void ClearAllFiled();
protected:
	LIST_FIELD m_vField ;
};

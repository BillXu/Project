#pragma once 
#include <string>
#include <list>
enum eValueType
{
	eValue_Float,
	eValue_Int,
	eValue_Double,
	eValue_longLong,
	eValue_String,
	eValue_Bin,
	eValue_Char,
	eValue_Short,
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
		long long llValue ; // 64 int ;
		double dfValue;
		long lValue ;
		char* pBuffer;
		char cValue;
		short sValue ;
	} Value ;
	int nBufferLen ;
public:
	stMysqlField(const char* pName,int nLen):strFieldName(pName,nLen){Value.pBuffer = NULL ;}
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

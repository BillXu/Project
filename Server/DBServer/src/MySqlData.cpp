#include "MySqlData.h"
stMysqlField::~stMysqlField()
{
	if ( eType == eValue_Bin || eValue_String == eType )
	{
		delete[] Value.pBuffer ;
	}
}

// row 
CMysqlRow::CMysqlRow()
{
	ClearAllFiled();
}

CMysqlRow::~CMysqlRow()
{
	ClearAllFiled() ;
}

void CMysqlRow::PushFiled(stMysqlField* pFiled )
{
	if ( pFiled )
		m_vField.push_back(pFiled);
}

stMysqlField* CMysqlRow::GetFiledByName( const char* pFiledName )
{
	if ( !pFiledName )
		return NULL ;
	LIST_FIELD::iterator iter = m_vField.begin() ;
	for ( ; iter != m_vField.end() ; ++iter )
	{
		stMysqlField* pField = *iter ;
		if ( pField == NULL )
			continue ;
		if ( strcmp(pFiledName,pField->strFieldName.c_str()) == 0 )
		{
			return pField ;
		}
	}
	return NULL ;
}

stMysqlField* CMysqlRow::operator[](const char* pFieldname )
{
	return GetFiledByName(pFieldname) ;
}

void CMysqlRow::ClearAllFiled()
{
	LIST_FIELD::iterator iter = m_vField.begin() ;
	for ( ; iter != m_vField.end() ; ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vField.clear() ;
}
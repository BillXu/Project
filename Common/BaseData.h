#pragma once
#include "CommonDefine.h"
// �����治����ָ�룬��Ϊ����ṹҪ������Ϣ.  
struct stBaseData  
{
	long long nCoin ;
	unsigned int nDiamoned ;
	char strName[MAX_LEN_CHARACTER_NAME] ;
public:
	stBaseData();
};
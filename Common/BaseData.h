#pragma once
#include "CommonDefine.h"
// �����治����ָ�룬��Ϊ����ṹҪ������Ϣ.  
struct stBaseData  
{
	int nCoin ;
	unsigned int nDiamoned ;
	unsigned char nTitle ;
	unsigned char nVipLevel ; // 0 is not vip ;
	char strName[MAX_LEN_CHARACTER_NAME] ;
public:
	stBaseData();
};
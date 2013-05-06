#pragma once
#include "CommonDefine.h"
// 这里面不能有指针，因为这个结构要发送消息.  
struct stBaseData  
{
	int nCoin ;
	unsigned int nDiamoned ;
	char strName[MAX_LEN_CHARACTER_NAME] ;
public:
	stBaseData();
};
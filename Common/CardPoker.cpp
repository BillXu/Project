#include "CardPoker.h"
#include <assert.h>
#include <stdlib.h>
#include "LogManager.h"
// card 
CCard::CCard(  unsigned char nCompositeNum  )
	:m_eType(eCard_Heart)
	,m_nCardFaceNum(0)
{
	RsetCardByCompositeNum(nCompositeNum) ;
}

CCard::~CCard()
{

}

void CCard::RsetCardByCompositeNum( unsigned char nCompositeNum )
{
	assert(nCompositeNum > 0 && nCompositeNum <= 52 && "illegal composite Number" );
	if ( (nCompositeNum > 0 && nCompositeNum <= 52) != true )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("Illegal composite Number = %d",nCompositeNum );
		return ;
	}
	m_eType = (eCardType)((nCompositeNum - 1)/13);
	m_nCardFaceNum = nCompositeNum - m_eType * 13 ;
}

unsigned char CCard::GetCardCompositeNum()
{
	return ( m_eType * 13 + m_nCardFaceNum );
}

void CCard::LogCardInfo()
{
	const char* pType = NULL ;
	switch ( m_eType )
	{
	case eCard_Heart:
		pType = "ºìÌÒ";
		break;
	case eCard_Sword:
		pType = "ºÚÌÒ";
		break;
	case eCard_Club:
		pType = "²Ý»¨";
		break;
	case eCard_Diamond:
		pType = "·½¿é";
		break;
	default:
		break; 
	}
	CLogMgr::SharedLogMgr()->PrintLog("this is %s : %d . Composite Number: %d",pType,m_nCardFaceNum, GetCardCompositeNum() );
}

// Poker
CPoker::CPoker()
{
	m_nCurrentCardIndex = 0 ;
	RestAllPoker();
}

CPoker::~CPoker()
{

}

unsigned char CPoker::GetCardWithCompositeNum()
{
	if ( m_nCurrentCardIndex >= ePoker_Card_Count )	
		RestAllPoker();
	return m_vAllCard[m_nCurrentCardIndex++] ;
}

void CPoker::ComfirmKeepCard( unsigned char nCardLeft )
{
	if ( ePoker_Card_Count - m_nCurrentCardIndex < nCardLeft )
		RestAllPoker();
}

void CPoker::RestAllPoker()
{
	for ( int i = 0 ; i < ePoker_Card_Count ; ++i )
	{
		m_vCardHelpReset[i] = i + 1 ;
		m_vAllCard[i]=0;
	}

	int nRightIndx = 0 ;
	double nAllLeftCound = ePoker_Card_Count ;
	for ( int i = 0 ; i < ePoker_Card_Count ; ++i )
	{
		nRightIndx  = ( (double)rand() / (double)RAND_MAX ) * nAllLeftCound ;
		for ( int nGetNum = 0 , idx = 0 ; nGetNum < ePoker_Card_Count ; ++nGetNum )
		{
			 if ( m_vCardHelpReset[nGetNum] == 0 )
				 continue ;
			 if ( idx++ == nRightIndx )
			 {
				 m_vAllCard[i] = m_vCardHelpReset[nGetNum] ;
				 m_vCardHelpReset[nGetNum] = 0 ;
				 --nAllLeftCound ;
				 break; 
			 }
		}
	}
	m_nCurrentCardIndex = 0 ;
}


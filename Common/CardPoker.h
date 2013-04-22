#pragma once 
class CCard
{
public:
	enum eCardType
	{
		eCard_Heart, // 红桃 
		eCard_Sword, // 黑桃 
		eCard_Club, // 草花
		eCard_Diamond, // 方块
		eCard_Max,
	};
public:
	CCard( unsigned char nCompositeNum );
	~CCard();
	void RsetCardByCompositeNum( unsigned char nCompositeNum );
	unsigned char GetCardFaceNum(){ return m_nCardFaceNum ; } // face num is the num show on the card ;
	unsigned char GetCardCompositeNum();  // Composite are make of face num and card type ;
	eCardType GetType(){ return m_eType ;}
	void LogCardInfo();
protected:
	eCardType m_eType ;
	unsigned char m_nCardFaceNum ;
};

class CPoker
{
public:
	enum 
	{
		ePoker_Card_Count = 52 , // this poker do n't have two joke ;
	};
public:
	CPoker();
	~CPoker();
	unsigned char GetCardWithCompositeNum();
	void ComfirmKeepCard( unsigned char nCardLeft = 3 * 5 ); // 开局发牌之前，一定要确认牌堆里是否有足够的牌。 .
protected:
	void RestAllPoker();
protected:
	unsigned char m_vAllCard[ePoker_Card_Count] ;
	unsigned char m_vCardHelpReset[ePoker_Card_Count] ;
	unsigned char m_nCurrentCardIndex ;
};
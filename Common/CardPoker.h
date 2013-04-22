#pragma once 
class CCard
{
public:
	enum eCardType
	{
		eCard_Heart, // ���� 
		eCard_Sword, // ���� 
		eCard_Club, // �ݻ�
		eCard_Diamond, // ����
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
	void ComfirmKeepCard( unsigned char nCardLeft = 3 * 5 ); // ���ַ���֮ǰ��һ��Ҫȷ���ƶ����Ƿ����㹻���ơ� .
protected:
	void RestAllPoker();
protected:
	unsigned char m_vAllCard[ePoker_Card_Count] ;
	unsigned char m_vCardHelpReset[ePoker_Card_Count] ;
	unsigned char m_nCurrentCardIndex ;
};
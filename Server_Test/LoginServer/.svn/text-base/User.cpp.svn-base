//#include "User.h"
#include "UserMgr.h"

CUser::CUser(uint32_t uid)
{
	m_nUserId = uid;
	memset(m_szName, 0, MAX_NAME_SIZE);
	memset(m_szPwd, 0, MAX_PASS_SIZE);
	m_nOnlineFlag = 0;
	memset(m_szEmail, 0, MAX_EMAIL_SIZE);
	memset(m_szMobile, 0, MAX_MOBILE_LEN);
	memset(&m_roleInfo, 0, sizeof(m_roleInfo));
	m_nCheckNo = 0;
}

CUser::~CUser()
{
	
}

uint32_t CUser::Login(stUserInfo userInfo, stRoleInfo roleInfo)
{
	//fill data
	m_nUserId = userInfo.id;
	m_nOnlineFlag = userInfo.onlineFlag;
	memcpy(m_szName, userInfo.name, userInfo.nameLen);
	memcpy(m_szPwd, userInfo.pwd, userInfo.pwdLen);
	memcpy(m_szEmail, userInfo.email, userInfo.emailLen);
	memcpy(m_szMobile, userInfo.mobile, userInfo.mobileLen);
	memcpy(&m_roleInfo, &roleInfo, sizeof(roleInfo));
	
	CreateCheckNo();
	g_UserMgr->AddUser(this);
	
	return GetCheckNo();
}

void CUser::CreateCheckNo()
{
	srand((unsigned int)time(NULL)); 
	while(m_nCheckNo < 1000)
	{
		m_nCheckNo = rand();
	}
}

uint32_t CUser::GetCheckNo()
{
	return m_nCheckNo;
}

void CUser::Logout()
{
	//assert(m_pHandler);
	//m_pHandler->Disconnect();
}









#pragma once
#include "../ServerDefine.h"
#include "../ModuleBehaviour.h"
struct AccountData
{
	std::string						username;
	std::string						password;
	std::string						address;
	account_id					accountID;
	bool								bOnline;
};

class AccountDataCache:public ModuleBehaviour
{
public:
	AccountDataCache();
	~AccountDataCache(void);

	bool Init() override;
	//�����û��������˺�ID, ����ֵ 0 δ�ҵ�
	//uint32_t	FindAccountID(const std::string& username);
	bool			IsExistAccount(account_id accountID);
	bool			FindAccountInfo(account_id accountID, AccountData& info);
	//�ж��˺������Ƿ�ƥ��
	bool			Match(account_id, const std::string& password);
	//�ж��Ƿ��Ѿ���½
	bool			IsOnline(account_id);
	//�˺�����
	bool			OnLine(account_id accountID);
	//�˺�����
	bool			OffLine(account_id accountID);
	//����˺�
	bool			AddAccount(account_id accountID,const std::string& username, const std::string& password);

private:
	std::unordered_map<account_id, AccountData>		m_AccountMap;
};




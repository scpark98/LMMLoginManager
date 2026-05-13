#pragma once
class GroupInfo
{
public:
	GroupInfo(void);
	~GroupInfo(void);

	void SetGroupNum(CString groupNum) { m_groupNum = groupNum; }
	void SetFlagDfGroup(int flagDfGroup) { m_flagDfGroup = flagDfGroup; }
	void SetFlagConfirm(int flagConfirm) { m_flagConfirm = flagConfirm; }
	void SetIW1Use(int use) { m_IW1Use = use; }
	void SetIW1Time(int time) { m_IW1Time = time; }
	void SetIW2Use(int use) { m_IW2Use = use; }
	void SetIW2Time(int time) { m_IW2Time = time; }
	void SetIW2Threshold(int threshold) { m_IW2Threshold = threshold; }
	void SetIW3Use(int use) { m_IW3Use = use; }
	void SetIW3Time(int time) { m_IW3Time = time; }
	void SetIW3Threshold(int threshold) { m_IW3Threshold = threshold; }
	void SetIW4Use(int use) { m_IW4Use = use; }
	void SetIW4Time(int time) { m_IW4Time = time; }
	void SetIW4Threshold(int threshold) { m_IW4Threshold = threshold; }
	void SetIW5Use(int use) { m_IW5Use = use; }
	void SetIW5Time(int time) { m_IW5Time = time; }
	void SetIW5Threshold(int threshold) { m_IW5Threshold = threshold; }
	void SetIW6Use(int use) { m_IW6Use = use; }
	void SetIW6Time(int time) { m_IW6Time = time; }
	void SetIW6Threshold(int threshold) { m_IW6Threshold = threshold; }
	void SetIW7Use(int use) { m_IW7Use = use; }
	void SetIW7Time(int time) { m_IW7Time = time; }
	void SetIW7Threshold(int threshold) { m_IW7Threshold = threshold; }
	void SetIW8Use(int use) { m_IW8Use = use; }
	void SetIW8Time(int time) { m_IW8Time = time; }
	void SetIW8Threshold(int threshold) { m_IW8Threshold = threshold; }
	void SetCompanyFk(CString companyFk) { m_companyFk = companyFk; }
	void SetParentGroupFk(CString groupFk) { m_parentGroupFk = groupFk; }
	void SetRootGroupFk(CString rootFk) { m_rootGroupFk = rootFk; }
	void SetCreateTime(CString createTime) { m_createTime = createTime; }
	void SetGroupName(CString groupName) { m_groupName = groupName; }
	void SetUpdateTime(CString updateTime) { m_updateTime = updateTime; }
	void SetManagerName(CString managerName) { m_managerName = managerName; }
	void SetManagerEmail(CString managerEmail) { m_managerEmail = managerEmail; }
	void SetManagerPhone(CString managerPhone) { m_managerPhone = managerPhone; }
	void SetLocalPath(CString localPath) { m_localPath = localPath; }

	CString GetGroupNum() { return m_groupNum; }
	int GetFlagDfGroup() { return m_flagDfGroup; }
	int GetFlagConfirm() { return m_flagConfirm; }
	int GetIW1Use() { return m_IW1Use; }
	int GetIW1Time() { return m_IW1Time; }
	int GetIW2Use() { return m_IW2Use; }
	int GetIW2Time() { return m_IW2Time; }
	int GetIW2Threshold() { return m_IW2Threshold; }
	int GetIW3Use() { return m_IW3Use; }
	int GetIW3Time() { return m_IW3Time; }
	int GetIW3Threshold() { return m_IW3Threshold; }
	int GetIW4Use() { return m_IW4Use; }
	int GetIW4Time() { return m_IW4Time; }
	int GetIW4Threshold() { return m_IW4Threshold; }
	int GetIW5Use() { return m_IW5Use; }
	int GetIW5Time() { return m_IW5Time; }
	int GetIW5Threshold() { return m_IW5Threshold; }
	int GetIW6Use() { return m_IW6Use; }
	int GetIW6Time() { return m_IW6Time; }
	int GetIW6Threshold() { return m_IW6Threshold; }
	int GetIW7Use() { return m_IW7Use; }
	int GetIW7Time() { return m_IW7Time; }
	int GetIW7Threshold() { return m_IW7Threshold; }
	int GetIW8Use() { return m_IW8Use; }
	int GetIW8Time() { return m_IW8Time; }
	int GetIW8Threshold() { return m_IW8Threshold; }
	CString GetCompanyFk() { return m_companyFk; }
	CString GetParentGroupFk() { return m_parentGroupFk; }
	CString GetRootGroupFk() { return m_rootGroupFk; }
	CString GetCreateTime() { return m_createTime; }
	CString GetGroupName() { return m_groupName; }
	CString GetUpdateTime() { return m_updateTime; }
	CString GetManagerName() { return m_managerName; }
	CString GetManagerEmail() { return m_managerEmail; }
	CString GetManagerPhone() { return m_managerPhone; }
	CString GetLocalPath() { return m_localPath; }

	HTREEITEM m_hItem = nullptr;

private:
	CString m_groupNum;
	int m_flagDfGroup;
	int m_flagConfirm;
	int m_IW1Use;
	int m_IW1Time;
	int m_IW2Use;
	int m_IW2Time;
	int m_IW2Threshold;
	int m_IW3Use;
	int m_IW3Time;
	int m_IW3Threshold;
	int m_IW4Use;
	int m_IW4Time;
	int m_IW4Threshold;
	int m_IW5Use;
	int m_IW5Time;
	int m_IW5Threshold;
	int m_IW6Use;
	int m_IW6Time;
	int m_IW6Threshold;
	int m_IW7Use;
	int m_IW7Time;
	int m_IW7Threshold;
	int m_IW8Use;
	int m_IW8Time;
	int m_IW8Threshold;
	CString m_companyFk;
	CString m_parentGroupFk;
	CString m_rootGroupFk;
	CString m_createTime;
	CString m_groupName;
	CString m_updateTime;
	CString m_managerName;
	CString m_managerEmail;
	CString m_managerPhone;

	CString m_localPath;
};


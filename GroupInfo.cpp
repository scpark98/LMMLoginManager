#include "pch.h"
#include "GroupInfo.h"


GroupInfo::GroupInfo(void)
{
	m_groupNum = _T("");
	m_flagDfGroup = 0;
	m_flagConfirm = 0;
	m_IW1Use = 0;
	m_IW1Time = 0;
	m_IW2Use = 0;
	m_IW2Time = 0;
	m_IW2Threshold = 0;
	m_IW3Use = 0;
	m_IW3Time = 0;
	m_IW3Threshold = 0;
	m_IW4Use = 0;
	m_IW4Time = 0;
	m_IW4Threshold = 0;
	m_IW5Use = 0;
	m_IW5Time = 0;
	m_IW5Threshold = 0;
	m_IW6Use = 0;
	m_IW6Time = 0;
	m_IW6Threshold = 0;
	m_IW7Use = 0;
	m_IW7Time = 0;
	m_IW7Threshold = 0;
	m_IW8Use = 0;
	m_IW8Time = 0;
	m_IW8Threshold = 0;
	m_companyFk = _T("");
	m_parentGroupFk = _T("");
	m_rootGroupFk = _T("");
	m_createTime = _T("");
	m_groupName = _T("");
	m_updateTime = _T("");
	m_managerName = _T("");
	m_managerEmail = _T("");
	m_managerPhone = _T("");
	m_localPath = _T("");
}


GroupInfo::~GroupInfo(void)
{
}

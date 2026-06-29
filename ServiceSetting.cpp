#include "pch.h"
#include "LMMLoginManager.h"
#include "ServiceSetting.h"

CString ServiceSetting::strManagerToken = _T("");

BOOL ServiceSetting::isService = FALSE;
BOOL ServiceSetting::isSolution = FALSE;
BOOL ServiceSetting::isServerInput = FALSE;
BOOL ServiceSetting::isServiceP2P = FALSE;
BOOL ServiceSetting::isVisibleSignUp = FALSE;
BOOL ServiceSetting::isVisibleLicense = FALSE;
BOOL ServiceSetting::isKMSCrypt = FALSE;
BOOL ServiceSetting::isCheckLicense = FALSE;
BOOL ServiceSetting::isRemoteThumbnail = FALSE;
BOOL ServiceSetting::isSyncGroup = FALSE;
BOOL ServiceSetting::isThumbnailViewer = FALSE;
BOOL ServiceSetting::isAlertCheck = FALSE;
BOOL ServiceSetting::isAutoRecord = FALSE;
BOOL ServiceSetting::isRegionJA = FALSE;


ServiceSetting::ServiceSetting(void)
{
}


ServiceSetting::~ServiceSetting(void)
{
}


BOOL ServiceSetting::LoadServiceSetting()
{
	CString strProgramType = GetProgramType();
	if (strProgramType == _T("LMM_SERVICE"))
	{
		isService = TRUE;
		isSolution = FALSE;
	}
	else if (strProgramType == _T("SOLUTION"))
	{
		isService = FALSE;
		isSolution = TRUE;
	}
	else
	{
		return FALSE;
	}

	CString strSetting = GetSetting();
	if (strSetting.Find(_T("SERVER_INPUT")) >= 0)
	{
		isServerInput = TRUE;
	}
	else
	{
		isServerInput = FALSE;
	}
	if (strSetting.Find(_T("SERVICE_P2P")) >= 0)
	{
		isServiceP2P = TRUE;
	}
	else
	{
		isServiceP2P = FALSE;
	}
	if (strSetting.Find(_T("VISIBLE_SIGNUP")) >= 0)
	{
		isVisibleSignUp = TRUE;
	}
	else
	{
		isVisibleSignUp = FALSE;
	}
	if (strSetting.Find(_T("VISIBLE_LICENSE")) >= 0)
	{
		isVisibleLicense = TRUE;
	}
	else
	{
		isVisibleLicense = FALSE;
	}
	if (strSetting.Find(_T("KMS_CRYPT")) >= 0)
	{
		isKMSCrypt = TRUE;
	}
	else
	{
		isKMSCrypt = FALSE;
	}
	if (strSetting.Find(_T("CHECK_LICENSE")) >= 0)
	{
		isCheckLicense = TRUE;
	}
	else
	{
		isCheckLicense = FALSE;
	}
	if (strSetting.Find(_T("SYNC_GROUP")) >= 0)
	{
		isSyncGroup = TRUE;
	}
	else
	{
		isSyncGroup = FALSE;
	}
	if (strSetting.Find(_T("REMOTE_THUMBNAIL")) >= 0)
	{
		isRemoteThumbnail = TRUE;
	}
	else
	{
		isRemoteThumbnail = FALSE;
	}
	if (strSetting.Find(_T("THUMBNAIL_VIEWER")) >= 0)
	{
		isThumbnailViewer = TRUE;
	}
	if (strSetting.Find(_T("REGION_JA")) >= 0)
	{
		isRegionJA = TRUE;
	}
	else
	{
		isThumbnailViewer = FALSE;
	}

	return TRUE;
}


CString ServiceSetting::GetProgramType()
{
	return theApp.m_ini["TYPE"]["PROGRAM_TYPE"].to_CString();
	//return Config::GetConfigValue(_T("TYPE"), _T("PROGRAM_TYPE"), _T(""), CUtil::GetProgramTypePath());
}


CString ServiceSetting::GetSetting()
{
	return theApp.m_ini["TYPE"]["SETTING"].to_CString();
	//return Config::GetConfigValue(_T("TYPE"), _T("SETTING"), _T(""), CUtil::GetProgramTypePath());
}

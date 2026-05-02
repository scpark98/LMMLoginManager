#pragma once
#include "winsvc.h"

#define _S Util::LoadMyString


class Util
{
public:
	Util();
	~Util();

	static CString GetCurrentPath();
	static CString GetCompanyPath();
	static CString GetConfigPath();
	static CString GetManagerPath();
	static CString GetAgentPath();
	static CString GetProgramTypePath();
	
	static CString GetFileVersion(CString filePath);

	static CString UTF8toCString(char *pszCode);

	static CString GetMyPrivateProfileString(CString strSection, CString strKey, CString strDefault, CString strPath);
	static int GetMyPrivateProfileInt(CString strSection, CString strKey, int strDefault, CString strPath);

	static void KillProcessByName(CString processName);
	static int SearchProcessCount(CString processName);

	static BOOL ClearProcess(CString strUpdateType);

	static BOOL GetIsService();
	static BOOL GetIsServerInput();

	static BOOL ServiceStop();

	static CString GetCurrentDirPath();
	static CString GetCurrentAgentPath();
	static CString GetCurrentLgiMgrPath();
	static CString GetCurrentSvcMgrPath();

	static CString LoadMyString(UINT nID);

	static CString CryptPassword(CString password);
	static CString EnCryptPassword(CString cryptedPass);
};

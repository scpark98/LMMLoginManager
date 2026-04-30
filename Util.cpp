#include "pch.h"
#include "Util.h"
#include "./Util/neturoPassword.h"
#include "LMMLoginManager.h"
#include <tlhelp32.h>
#pragma comment(lib,"version.lib")
#include <vector>

#include "Common/Functions.h"

Util::Util()
{
}


Util::~Util()
{
}


CString Util::GetCurrentPath()
{
	TCHAR strMyDir[MAX_PATH];
	ZeroMemory(strMyDir, MAX_PATH);

	int res = GetModuleFileName(NULL, strMyDir, MAX_PATH);
	while (res > 0)
	{
		if (strMyDir[--res] == '\\')
		{
			strMyDir[res] = '\0';
			break;
		}
	}

	CString path;
	path.Format(_T("%s"), strMyDir);
	return path;
}


CString Util::GetCompanyPath()
{
	CString configPath;
	configPath.Format(_T("%s\\company.ini"), GetCurrentPath());

	return configPath;
}


CString Util::GetConfigPath()
{
	CString configPath;
	configPath.Format(_T("%s\\config.ini"), GetCurrentPath());

	return configPath;
}


CString Util::GetManagerPath()
{
	CString managerPath;
	managerPath.Format(_T("%s\\LMMManager.exe"), GetCurrentPath());

	return managerPath;
}


CString Util::GetAgentPath()
{
	CString agentPath;
	agentPath.Format(_T("%s\\LMMAgent.exe"), GetCurrentPath());

	return agentPath;
}


CString Util::GetProgramTypePath()
{
	CString preCheckPath;
	preCheckPath.Format(_T("%s\\programtype.ini"), GetCurrentPath());

	return preCheckPath;
}


CString Util::GetMyPrivateProfileString(CString strSection, CString strKey, CString strDefault, CString strPath)
{
	TCHAR strValue[256] = { 0, };
	GetPrivateProfileString(strSection, strKey, strDefault, strValue, 256, strPath);

	CString strReturn;
	strReturn.Format(_T("%s"), strValue);
	return strReturn;
}


int Util::GetMyPrivateProfileInt(CString strSection, CString strKey, int strDefault, CString strPath)
{
	TCHAR strValue[256] = { 0, };
	return GetPrivateProfileInt(strSection, strKey, strDefault, strPath);
}


CString Util::GetFileVersion(CString filePath)
{
	CString strVersion = _T("");
	DWORD dwHdlDest;
	DWORD dwSizeDest;

	dwSizeDest = GetFileVersionInfoSize(
		filePath,
		&dwHdlDest);
	if (dwSizeDest > 0)
	{
		BYTE* pDestData = new BYTE[dwSizeDest + 1];
		if (GetFileVersionInfo(
			filePath,
			dwHdlDest,
			dwSizeDest,
			pDestData))
		{
			VS_FIXEDFILEINFO * pvsInfo;
			UINT uLen;
			if (VerQueryValue(pDestData, _T("\\"), (void**)&pvsInfo, &uLen))

			{

				strVersion.Format(_T("%d.%d.%d.%d"), (pvsInfo->dwProductVersionMS / 65536),
					(pvsInfo->dwProductVersionMS % 65536), (pvsInfo->dwProductVersionLS / 65536),
					(pvsInfo->dwProductVersionLS % 65536));
				/*
				strVersion.Format("%d.%d", (pvsInfo->dwProductVersionMS / 65536),
				(pvsInfo->dwProductVersionMS % 65536));
				*/
			}
		}

		delete[] pDestData;
	}

	return strVersion;
}


CString Util::UTF8toCString(char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;
	// Get nLength of the Wide Char buffer
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);
	// Change UTF-8 to Unicode (UTF-16)
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);
	// Get nLength of the multi byte buffer
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];
	// Change from unicode to mult byte
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	CString strResult = (CString)pszAnsi;
	delete[] pszAnsi;

	return strResult;
}


void Util::KillProcessByName(CString processName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (processName.Compare(pEntry.szExeFile) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}


int Util::SearchProcessCount(CString processName)
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	int count = 0;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	pEntry.dwSize = sizeof(pEntry);
	// 실행중인 프로세스들의 첫번재 정보를 가져온다.
	Process32First(hSnapShot, &pEntry);

	// Process 가 실행중인지 확인
	while (1)
	{
		// 다음번 프로세스의 정보를 가져온다.
		BOOL hRes = Process32Next(hSnapShot, &pEntry);

		if (hRes == FALSE)
			break;

		if (processName.Compare(pEntry.szExeFile) == 0)
		{
			++count;
		}
	}

	CloseHandle(hSnapShot);
	return count;
}


BOOL Util::ClearProcess(CString strUpdateType)
{
	if (strUpdateType == _T("AGENT"))
	{
		logWrite(_T("[INFO][Util::ClearProcess] UpdateType = AGENT"));

		BOOL isStopped = ServiceStop();

		if (GetIsService())
		{
			Util::KillProcessByName(_T("LMMLgiMgr.exe"));
			Util::KillProcessByName(_T("nSSDClient.exe"));
			Util::KillProcessByName(_T("nSSDServer.exe"));
		}
		else
		{
			Util::KillProcessByName(_T("LMMSvcMgr.exe"));
			Util::KillProcessByName(_T("OptimalViewer.exe"));
		}

		Util::KillProcessByName(_T("nFTDClient.exe"));
		Util::KillProcessByName(_T("nFTDServer.exe"));
		Util::KillProcessByName(_T("ResourceCollector.exe"));
		Util::KillProcessByName(_T("RelayAgent.exe"));
		Util::KillProcessByName(_T("ProcChecker.exe"));
		Util::KillProcessByName(_T("hwmon.exe"));
		Util::KillProcessByName(_T("nScreenClient.exe"));

		if (isStopped)
		{
			//20241205 scpark AutoPatcher.exe로 동작될 때 LMMAgent.exe를 강제 종료시키면
			//LMM 1.0, 3.0 모두 LMMAgent.exe가 종료된다.
			//위에서 서비스를 종료시키면 LMMAgent.exe는 자동 종료되므로 여기서는 LMMAgent.exe는 별도 종료시키지 않아도 된다.
			//int nAgentCount = Util::SearchProcessCount(_T("LMMAgent.exe"));
			//if (nAgentCount > 0)
			{
				//ShellExecute(NULL, _T("open"), _T("taskkill.exe"), _T("/f /im LMMAgent.exe"), NULL, SW_HIDE);
				int res = kill_process_by_fullpath(GetAgentPath());
				logWrite(_T("result of kill_process_by_fullpath(GetAgentPath()) = %d"), res);

				res = kill_process_by_fullpath(get_exe_directory() + _T("\\nScreenClient.exe"));
				logWrite(_T("result of kill_process_by_fullpath(full path of nScreenClient.exe) = %d"), res);
				//ShellExecute(NULL, _T("open"), _T("taskkill.exe"), _T("/f /im nScreenClient.exe"), NULL, SW_HIDE);
			}
			return TRUE;
		}
		else
		{
			int res = kill_process_by_fullpath(GetAgentPath());
			logWrite(_T("result of kill_process_by_fullpath(GetAgentPath()) = %d"), res);
			//ShellExecute(NULL, _T("open"), _T("taskkill.exe"), _T("/f /im LMMAgent.exe"), NULL, SW_HIDE);
			res = kill_process_by_fullpath(get_exe_directory() + _T("\\nScreenClient.exe"));
			logWrite(_T("result of kill_process_by_fullpath(full path of nScreenClient.exe) = %d"), res);
			//ShellExecute(NULL, _T("open"), _T("taskkill.exe"), _T("/f /im nScreenClient.exe"), NULL, SW_HIDE);

			return FALSE;
		}
	}
	else if (strUpdateType == _T("MANAGER"))
	{
		logWrite(_T("[INFO][Util::ClearProcess] UpdateType = MANAGER"));

		if (GetIsService())
		{
			Util::KillProcessByName(_T("nSSDClient.exe"));
			Util::KillProcessByName(_T("nSSDServer.exe"));
		}

		Util::KillProcessByName(_T("LMMThumbnail.exe"));
		Util::KillProcessByName(_T("ThumbnailViewer.exe"));
		Util::KillProcessByName(_T("nFTDClient.exe"));
		Util::KillProcessByName(_T("nFTDServer.exe"));
		Util::KillProcessByName(_T("LMMViewer.exe"));
		Util::KillProcessByName(_T("LMMManager.exe"));

		return TRUE;
	}

	return FALSE;
}


BOOL Util::GetIsService()
{
	CString strType = GetMyPrivateProfileString(_T("TYPE"), _T("PROGRAM_TYPE"), _T(""), GetProgramTypePath());
	if (strType == _T("LMM_SERVICE"))
	{
		return TRUE;
	}
	return FALSE;
}


BOOL Util::GetIsServerInput()
{
	CString strType = GetMyPrivateProfileString(_T("TYPE"), _T("SETTING"), _T(""), GetProgramTypePath());
	if (strType.Find(_T("SERVER_INPUT")) >= 0)
	{
		return TRUE;
	}
	return FALSE;
}


CString Util::GetCurrentAgentPath()
{
	return GetCurrentDirPath() + _T("\\") + _T("LMMAgent.exe");
}


CString Util::GetCurrentLgiMgrPath()
{
	return GetCurrentDirPath() + _T("\\") + _T("LMMLgiMgr.exe");
}


CString Util::GetCurrentSvcMgrPath()
{
	return GetCurrentDirPath() + _T("\\") + _T("LMMSvcMgr.exe");
}


CString Util::GetCurrentDirPath()
{
	TCHAR out[_MAX_PATH];
	ZeroMemory(out, _MAX_PATH);

	std::vector<TCHAR> buffer;
	DWORD size = MAX_PATH;
	DWORD ret;

	while (true) 
	{
		// Allocate buffer
		buffer.resize(size + 1);
		// Try to get file name
		ret = GetModuleFileName(NULL, &buffer[0], size);
		if (ret == 0) 
		{
			return false;
		}
		else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
		{
			size += 128;
		}
		else 
		{
			break;
		}
	} // while

	_tcscpy_s(out, _MAX_PATH, &buffer[0]);
	//out = &buffer[0];

	while (ret > 0)
	{
		if (out[--ret] == '\\')
		{
			out[ret] = '\0';
			break;
		}
	}

	if (out[_tcslen(out) - 1] != '\\')
	{
		_tcscat_s(out, _MAX_PATH, _T("\\"));
	}

	CString strPath;
	strPath.Format(_T("%s"), out);

	return strPath;
}

BOOL Util::ServiceStop()
{
	BOOL isStopped = TRUE;

	if (!GetIsService())
	{
		theApp.m_svc_name = _T("LMMSvcAgentService");
	}

	SERVICE_STATUS serviceStatus;
	ZeroMemory(&serviceStatus, sizeof(SERVICE_STATUS));

	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hScm, theApp.m_svc_name, SC_MANAGER_ALL_ACCESS);

	QueryServiceStatus(hService, &serviceStatus); // 서비스 상태 얻기
	if (serviceStatus.dwCurrentState != SERVICE_STOPPED) // 서비스가 중지 상태가 아니면
	{
		BOOL bControl = ::ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus); // 서비스 중지
		if (bControl == FALSE)
		{
			CloseServiceHandle(hScm);
			CloseServiceHandle(hService);
			return FALSE;
		}
	}

	int nChecks = 10;
	int nDelay = 1000;

	QueryServiceStatus(hService, &serviceStatus); // 서비스 상태 얻기
	while (serviceStatus.dwCurrentState != SERVICE_STOPPED) // 서비스가 중지 상태인지 확인
	{
		if (--nChecks <= 0)
		{
			isStopped = FALSE;
			break;
		}
		Sleep(nDelay);

		QueryServiceStatus(hService, &serviceStatus); // 서비스 상태 얻기
	}

	CloseServiceHandle(hScm);
	CloseServiceHandle(hService);

	return isStopped;
}

CString Util::LoadMyString(UINT nID)
{
	CString str;
	str.LoadString(nID);
	return str;
}

CString Util::CryptPassword(CString password)
{
	char temp_crypt_pass[MAXPWLEN];

	ZeroMemory(temp_crypt_pass, MAXPWLEN);
	strncpy_s(temp_crypt_pass, MAXPWLEN, CW2A(password), _TRUNCATE);

	neturoPassword::FromText crypt(temp_crypt_pass);
	ZeroMemory(temp_crypt_pass, MAXPWLEN);
	memcpy(temp_crypt_pass, crypt, MAXPWLEN);

	CString cryptedStr = _T("");
	for (int i = 0; i < MAXPWLEN; i++)
	{
		int ch = temp_crypt_pass[i]; // 아스키 코드로 변환

		// 아스키 코드 저장
		if (i == 0)
		{
			cryptedStr.Format(_T("%d"), ch);
		}
		else
		{
			CString temp = cryptedStr;
			cryptedStr.Format(_T("%s %d"), temp, ch);
		}
	}

	return cryptedStr;
}

CString Util::EnCryptPassword(CString cryptedPass)
{
	char temp[MAXPWLEN];
	ZeroMemory(temp, MAXPWLEN);
	for (int i = 0; i < MAXPWLEN; i++)
	{
		CString token;
		AfxExtractSubString(token, cryptedPass, i, ' ');
		temp[i] = _ttoi(token); // 아스키 코드를 char로 변환
	}

	neturoPassword::ToText plain(temp);
	wchar_t atow[MAXPWLEN];
	MultiByteToWideChar(CP_ACP, 0, plain, -1, atow, MAXPWLEN);

	CString enCryptedPass = atow;
	return enCryptedPass;
}

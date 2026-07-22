
// LMMLoginManager.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "LMMLoginManager.h"
#include "LMMLoginManagerDlg.h"

#include "Common/messagebox/CSCMessageBox/SCMessageBox.h"


#include "VersionDlg.h"
#include "PositionDlg.h"
#include "DeviceNameDlg.h"
#include "ServiceSetting.h"
#include "DeviceGroupDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLMMLoginManagerApp

BEGIN_MESSAGE_MAP(CLMMLoginManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLMMLoginManagerApp 생성

CLMMLoginManagerApp::CLMMLoginManagerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CLMMLoginManagerApp 개체입니다.

CLMMLoginManagerApp theApp;
CSCLog gLog;


// CLMMLoginManagerApp 초기화

BOOL CLMMLoginManagerApp::InitInstance()
{
	// Windows XP에서는 InitCommonControlsEx()를 필요로 합니다.
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		m_msgbox.DoModal(_T("Windows sockets initialization failed."));// IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("Koino"));

#if defined(_LINKMEMINE_10)
	gLog.set(get_known_folder(CSIDL_COMMON_DOCUMENTS) + _T("/LinkMeMine/Log/LMMLgiMgr"));
	m_theme.set_color_theme(CSCColorTheme::color_theme_linkmemine);
#elif defined(_LINKMEMINE_30)
	gLog.set(get_known_folder(CSIDL_COMMON_DOCUMENTS) + _T("/LinkMeMineSE/Log/LMMLgiMgr"));
	m_theme.set_color_theme(CSCColorTheme::color_theme_linkmemine_se);

#endif

	gLog.write_start_log();


	//20240729 scpark AutPatcher.exe_ 를 AutPatcher.exe로 변경하는 update.bat 파일이 돌지 않는 경우가
	//특정 사용자에서 발생하여 update.bat이 하는 액션을 LMMLgiMgr.exe, LMMAgent.exe에 추가한다.
	//LMMAgent.exe 코드를 살펴보니 이미 _tWinMain에 이와 같이 동작하는 코드가 구현되어 있음.
	CString oldFile;
	CString newFile;

	newFile.Format(_T("%s\\AutoPatcher.exe_"), get_exe_directory());
	oldFile.Format(_T("%s\\AutoPatcher.exe"), get_exe_directory());

	if (PathFileExists(newFile))
	{
		DeleteFile(oldFile);
		MoveFileEx(newFile, oldFile, MOVEFILE_REPLACE_EXISTING);
	}

	m_product_name = get_file_property(_T(""), _T("ProductName"));
	m_config_path.Format(_T("%s\\config.ini"), get_exe_directory());
	m_ini.SetFileName(m_config_path);
	m_ip = m_ini["SERVER"]["LIP"].to_CString();
	m_port = m_ini["SERVER"]["LPORT"].to_int();
	m_company_key = m_ini["SERVER"]["COMPANY_KEY"].to_int();

	CheckRegistrySetTLS();

	ServiceSetting::LoadServiceSetting();
	BOOL bDeleteDevice = FALSE;
	BOOL bUIStart = TRUE;


	m_msgbox.create(nullptr, _S(IDS_TITLE));
	m_msgbox.set_color_theme(m_theme);// CSCColorTheme::color_theme_dark_gray);

	if (__argc >= 2)
	{
		for (int i = 0; i < __argc; i++)
		{
			logWrite(_T("__targv[%d] = %s"), i, __targv[i]);
		}

		m_cmd = __targv[1];
		m_cmd.Trim();

		bUIStart = FALSE;

		if (m_cmd == _T("-about")) // 정보
		{
			CVersionDlg dlg;
			dlg.DoModal();
		}
		else if (m_cmd == _T("-logoff"))
		{
			ExitWindowsEx(EWX_LOGOFF, 0);
			return FALSE;
		}
		else if (m_cmd == _T("-pos")) // 화면 공유 위치 설정
		{
			CPositionDlg dlg;
			dlg.DoModal();
		}
		else if (m_cmd == _T("-name")) // 장치 이름 변경
		{
			CDeviceNameDlg dlg;
			dlg.DoModal();
		}
		else if (m_cmd == _T("-group")) // 그룹 변경
		{
			CDeviceGroupDlg dlgGroup;
			dlgGroup.DoModal();
		}
		else if (m_cmd == _T("-messagebox")) // 메시지박스 표시
		{
			for (int i = 1; i < __argc; i++)
				logWrite(_T("-messagebox cmd. __targv[%d] = %s"), i, __targv[i]);

			//삼성생명에서는 단순 메시지박스가 아닌 미등록, 이미 등록일 경우의 메시지를 표시한다.
			if (__argc >= 4)
			{
				int type = _ttoi(__targv[2]);
				CString msg = __targv[3];
				CString title = (__argc > 4 ? __targv[4] : _T(""));
				int timeout = (__argc > 5 ? _ttoi(__targv[5]) : 0);
				int hAlign = (__argc > 6 ? _ttoi(__targv[6]) : -1);

				CSCMessageBox dlg;
				dlg.DoModal(msg, type == 0 ? MB_OK : MB_OKCANCEL, timeout);

				return FALSE;
			}
		}
		else if (m_cmd == _T("-exit")) // 종료
		{
			CString strMessage = _S(IDS_EXIT_AGENT);

			if (m_msgbox.DoModal(strMessage, MB_OKCANCEL) == IDOK)
			{
				int nAutoLogin = m_ini["LOGIN"]["AUTO_LOGIN"].to_int();// Config::LoadIsAutoLogin();

				logWrite(_T("nAutoLogin = %d."), nAutoLogin);

				DWORD status = 0;
				DWORD error_code = 0;
				CString detail;

				if (nAutoLogin == 0)
				{
					status = service_command(m_service_name, _T("delete"), error_code, &detail);
					logWrite(_T("service cmd delete. status = %d, error_code = %d, detail = %s"), status, error_code, detail);
				}
				else
				{
					status = service_command(m_service_name, _T("stop"), error_code, &detail);
					logWrite(_T("service cmd stop. status = %d, error_code = %d, detail = %s"), status, error_code, detail);
				}

				//Config::SaveManualLoginStatus(0);
				m_ini["LOGIN"]["MANUAL_LOGIN_STATUS"] = 0;

				terminate_other_processes();
			}
		}
		else if (m_cmd == _T("-logout") || m_cmd == _T("-logoutservice")) // 로그아웃
		{
			CString strMessage = _S(IDS_EXIT_AGENT);

			CSCMessageBox dlg;

			if (dlg.DoModal(strMessage, MB_OKCANCEL) == IDOK)
			{
				strMessage = _S(IDS_AGENT_STOP_WAIT);
				CSCMessageBox dlg2;
				dlg2.DoModal(_S(IDS_AGENT_STOP_WAIT));

				//Config::SaveManualLoginStatus(0);
				//Config::SaveIsAutoLogin(0);
				m_ini["LOGIN"]["MANUAL_LOGIN_STATUS"] = 0;
				m_ini["LOGIN"]["AUTO_LOGIN"] = 0;

				terminate_other_processes();
			}
			// 2023.04.26 seodongho, 서비스 로그 아웃시에 디바이스를 삭제할것인지 확인
			if (m_cmd == _T("-logoutservice"))
				bDeleteDevice = TRUE;
			bUIStart = TRUE;
		}
		else if (m_cmd == _T("-delete")) // 장치 삭제
		{
			bDeleteDevice = TRUE;
		}
		//파라미터가 넘어왔는데 위의 지정된 cmd가 아닌 경우에 대한 처리 추가.
		else if (!m_cmd.IsEmpty())
		{
			//cmd가 http:// 또는 https://로 시작되면 ShellExecute()으로 실행시켜서 기본 브라우저로 열리게 한다.
			if (m_cmd.Left(7) == _T("http://") || m_cmd.Left(8) == _T("https://"))
			{
				logWrite(_T("param[1] is url. call ShellExecute()"));
				ShellExecute(NULL, _T("open"), m_cmd, 0, 0, SW_SHOWNORMAL);
			}
			//일반 실행파일을 실행시키는 경우
			else
			{
				CString param;
				logWrite(_T("m_cmd = %s. execute file. call ShellExecute()"), m_cmd);
				if (__argc >= 3)
				{
					param = __targv[2];
					logWrite(_T("param : %s"), param);
				}
				ShellExecute(NULL, _T("open"), m_cmd, param, 0, SW_SHOWNORMAL);
			}
		}

		if (bDeleteDevice)
		{
			//scpark 20250502 uninstall시에 DB에서 장비 삭제는 선택이 아니라 필수이므로
			//이를 물어보는 대화상자도 띠워서도 안된다. 무조건 DB에서도 삭제시킨다.
			//CDeviceDeleteDlg dlg;
			//if (dlg.DoModal() == IDOK)
			{
				//20260629 scpark 기존 코드는 LMM_LINUX에 의해 .netAPI 또는 flask가 호출되는 방식이었으나
				//.netAPI로 맞춰서 호출해도 status=200이 떨어지지만 InvalidToken 에러가 발생하여 장치가 지워지지 않는다.
				//따라서 flaskAPI(fastAPI)에 맞춰서 파라미터들을 변경한 후 삭제 성공함.
				//원래는 "DELETE"으로 줘야하지만 "POST"로 줘도 처리되도록 kym이 수정해 놓음.
				CRequestUrlParams param(m_ip, m_port, _T("/agent/api/v1.0/DeleteLmmDeviceInfo"), _T("POST"));

				CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"].to_CString();
				CString device_id = theApp.m_ini["SERVER"]["DID"].to_CString();
				CString header = _T("token: ") + agent_token + _T("\r\n");

				param.headers.push_back(header);
				param.body.Format(_T("{\"user_id\":\"%s\", \"device_id\":\"%s\"}"), m_ini["LOGIN"]["ID"].to_CString(), device_id);

				logWrite(_T("param : %s"), param.get_param_str());
				request_url(&param);
				logWrite(_T("status = %d, result = %s"), param.status, param.result);

				if (param.status != HTTP_STATUS_OK)
				{
					logWriteE(_T("fail to delete device from DB."));
				}
				else
				{
					logWrite(_T("success to delete device from DB."));
				}
				/*
				ApiParams* apiParams = new ApiParams[1];
				apiParams[0].dlg = NULL;
#ifndef LMM_LINUX
				apiParams[0].address.Format(_T("/api/v1.0/linkmemine/DeleteLmmDeviceInfo"));
				apiParams[0].method = _T("DELETE");
				apiParams[0].serverIp = Config::LoadNetAPIIP();
				apiParams[0].serverPort = _ttoi(Config::LoadNetAPIPort());
#else
				apiParams[0].address.Format(_T("/agent/api/v1.0/DeleteLmmDeviceInfo"));
				apiParams[0].method = _T("POST");
				apiParams[0].serverIp = Config::LoadFlaskAPIIP();
				apiParams[0].serverPort = _ttoi(Config::LoadFlaskAPIPort());
#endif
				apiParams[0].type = 0;

				if (apiParams[0].serverPort == 443 || apiParams[0].serverPort == 8443)
				{
					apiParams[0].useHttps = 1;
				}
				else
				{
					apiParams[0].useHttps = 0;
				}

				apiParams[0].useTimeout = 1;
				apiParams[0].token = Config::LoadAgentToken();
				apiParams[0].parameter.Format(_T("{\"user_id\":\"%s\", \"device_id\":\"%s\"}"),
					Config::LoadConfigString(_T("LOGIN"), _T("ID"), _T("")),
					Config::LoadDeviceID());
				apiParams[0].afterFunc = 0;
				apiParams[0].size = 1;
				apiParams[0].index = 1;
				apiParams[0].useDunamu = FALSE;

				Api api;
				api.ApiController(apiParams);
				delete apiParams;
				*/
			}
		}
	}
	else //if(__argc < 2)
	{
		logWrite(_T("no params."));
	}

	if (bUIStart)
	{
		// 중복 실행 체크
		if (is_duplicate_running())
		{
			if (pShellManager != NULL)
			{
				delete pShellManager;
			}
			return FALSE;
		}

		CLMMLoginManagerDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

bool CLMMLoginManagerApp::is_duplicate_running()
{
	logWrite(_T("%s"), __function__);

	// 중복 실행 체크
	HANDLE mutex = CreateMutex(NULL, FALSE, _T("LMMLoginMgr for Service"));
	if (mutex == NULL)
		return false;

	// Check that the mutex didn't already exist
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		logWrite(_T("duplicated running. exit."));

		HWND hWnd = ::FindWindow(NULL, m_product_name);

		if (hWnd != NULL)
		{
			ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
		}

		return true;
	}

	return false;
}

void CLMMLoginManagerApp::terminate_other_processes()
{
	//taskkill /f /im name.exe 는 같은 이름의 *모든* 프로세스를 죽이므로 LMM 3.0 SE 등
	//다른 제품의 동명 프로세스까지 종료시키는 부작용이 있다. 정확히 우리 설치 경로의
	//exe 만 종료하도록 kill_process_by_fullpath 로 전환 (다른 경로의 동명 exe 영향 없음).
	//taskkill 은 ShellExecute 비동기였지만 kill_process_by_fullpath 는 동기 TerminateProcess —
	//함수 반환 시점에 종료가 보장된다.
	CString dir = get_exe_directory();
	kill_process_by_fullpath(dir + _T("\\nScreenClient.exe"));
	kill_process_by_fullpath(dir + _T("\\nFTDClient.exe"));
	kill_process_by_fullpath(dir + _T("\\nFTDServer.exe"));
	kill_process_by_fullpath(dir + _T("\\nFTDClient2.exe"));
	kill_process_by_fullpath(dir + _T("\\nFTDServer2.exe"));
	kill_process_by_fullpath(dir + _T("\\nSSDClient.exe"));
	kill_process_by_fullpath(dir + _T("\\nSSDServer.exe"));
	kill_process_by_fullpath(dir + _T("\\hwmon.exe"));
}

void CLMMLoginManagerApp::set_company_key(int company_key)
{
	m_company_key = company_key;
	m_ini["SERVER"]["COMPANY_KEY"] = m_company_key;
}

//20240912 scpark 개인향/기업향, 승인결재 프로세스 설정값 조회.
//3.0 SE 백엔드만 이 API 를 제공하므로 1.0 에서는 호출하지 말 것 (호출 지점에서 가드).
int CLMMLoginManagerApp::request_account_type(int* account_type, int* flag_function_agent_approval)
{
	*account_type = -1;
	*flag_function_agent_approval = -1;

	CRequestUrlParams param(m_ip, m_port, _T("/lmm/api/v1.0/request_account_type"), _T("POST"));
	param.body.Format(_T("{\"user_id\":\"%s\"}"), m_ini["LOGIN"]["ID"].to_CString());
	request_url(&param);

	logWrite(_T("status = %d, url = %s, body = %s\nresult = \n%s"),
		param.status, param.full_url, param.body, param.result);

	//응답 예)
	//{
	//  "api": "RequestAccountTypeAPI",
	//  "meta": null,
	//  "objects": { "account_type": 1, "flag_connection_reason": 1, "flag_function_agent_approval": 0 },
	//  "status": 200
	//}
	if (param.status == HTTP_STATUS_OK)
	{
		Json json;
		json.parse(param.result);
		*account_type = json.doc["objects"]["account_type"].GetInt();
		*flag_function_agent_approval = json.doc["objects"]["flag_function_agent_approval"].GetInt();

		//기업향 & 결재프로세스 on 이 아닌 사용자는 등록 절차가 없으므로 REGI_STATUS = 9 로 표식.
		if (*account_type == 0 || *flag_function_agent_approval == 0)
			m_ini["LOGIN"]["REGI_STATUS"] = 9;

		return 1;
	}

	return 0;
}

//20240924 scpark 그룹 자동 등록.
//배경: 웹에서 에이전트 삭제 후 재설치 시 config.ini 의 FIRST_START=0 이 남아 Agent 자동등록이
//누락되던 문제 대응. LoginManager 가 로그인 성공 직후 이 API 를 호출하고, 백엔드는 이미 등록된
//디바이스면 no-op, 아니면 디바이스 등록 + 기본 그룹 자동 배정을 수행.
//기업향 & 결재프로세스 on 사용자는 등록신청 절차를 거치므로 자동등록 대상에서 제외.
int CLMMLoginManagerApp::agent_auto_device_and_group_register()
{
	logWrite(_T("%s"), __function__);

	int account_type = -1;
	int flag_function_agent_approval = -1;
	request_account_type(&account_type, &flag_function_agent_approval);
	logWrite(_T("account_type = %d, flag_function_agent_approval = %d"),
		account_type, flag_function_agent_approval);

	if (account_type == 1 && flag_function_agent_approval == 1)
		return 0;

	CRequestUrlParams param(m_ip, m_port, _T("/lmm/api/v1.0/agent_auto_device_and_group_register"), _T("POST"));
	param.body.Format(_T("{\"user_id\":\"%s\", \"device_pk\":%d}"),
		m_ini["LOGIN"]["ID"].to_CString(),
		m_ini["SERVER"]["SVRNUM"].to_int());
	request_url(&param);

	logWrite(_T("status = %d, url = %s, body = %s\nresult = \n%s"),
		param.status, param.full_url, param.body, param.result);

	return (param.status == HTTP_STATUS_OK) ? 1 : 0;
}

void CLMMLoginManagerApp::CheckRegistrySetTLS()
{
	DWORD dataType = REG_DWORD;
	DWORD dataSize = 256;
	TCHAR dataBuffer[256] = { 0, };

	HKEY hKey;
	LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_ALL_ACCESS, &hKey);
	if (ret == ERROR_SUCCESS)
	{
		ret = RegQueryValueEx(hKey, _T("SecureProtocols"), 0, &dataType, (LPBYTE)dataBuffer, &dataSize);
		if (ret != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return;
		}

		LPDWORD value = reinterpret_cast<LPDWORD>(&dataBuffer);
		if (!(*value & 128))
		{
			*value |= 128;
			RegSetValueEx(hKey, _T("SecureProtocols"), NULL, REG_DWORD, (LPBYTE)value, sizeof(DWORD));
		}

		RegCloseKey(hKey);
	}
}


// LMMLoginManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LMMLoginManager.h"
#include "LMMLoginManagerDlg.h"
#include "afxdialogex.h"

#include "Util.h"

#include "Common/Functions.h"
#include "Common/MemoryDC.h"
#include "Common/Json/rapid_json/json.h"
#include "Common/win_compat/dwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLMMLoginManagerDlg 대화 상자



CLMMLoginManagerDlg::CLMMLoginManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LMMLOGINMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLMMLoginManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_button_config);
	DDX_Control(pDX, IDC_BUTTON_MINIMIZE, m_button_minimize);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button_close);
	DDX_Control(pDX, IDC_EDIT_ID, m_edit_id);
	DDX_Control(pDX, IDC_EDIT_PW, m_edit_pw);
	DDX_Control(pDX, IDC_CHECK_SAVE_PW, m_check_save_pw);
	DDX_Control(pDX, IDC_CHECK_AUTO_LOGIN, m_check_auto_login);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_button_login);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_static_version);
	DDX_Control(pDX, IDC_BUTTON_RESTART, m_button_restart);
}

BEGIN_MESSAGE_MAP(CLMMLoginManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLMMLoginManagerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLMMLoginManagerDlg::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CLMMLoginManagerDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, &CLMMLoginManagerDlg::OnBnClickedButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CLMMLoginManagerDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_CHECK_SAVE_PW, &CLMMLoginManagerDlg::OnBnClickedCheckSavePw)
	ON_BN_CLICKED(IDC_CHECK_AUTO_LOGIN, &CLMMLoginManagerDlg::OnBnClickedCheckAutoLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLMMLoginManagerDlg::OnBnClickedButtonLogin)
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_MESSAGE(WM_APP_UI_INVOKE, &CLMMLoginManagerDlg::on_ui_invoke)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CLMMLoginManagerDlg::OnBnClickedButtonRestart)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLMMLoginManagerDlg 메시지 처리기

BOOL CLMMLoginManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	//자동 설치 패키지 여부 설정값 로딩
	DWORD is_auto_setup = 0;
	get_registry_int(HKEY_LOCAL_MACHINE, theApp.m_reg_path, _T("is auto setup"), &is_auto_setup);
	theApp.m_auto_setup = is_auto_setup;
	logWrite(_T("is_auto_setup = %d"), is_auto_setup);

	init_dialog();
	init_controls();
	m_udpSocket.Create();

	RestoreWindowPosition(&theApp, this, _T(""), false, false);

	//실제로는 100이며 서버 체크 목적으로 5000으로 설정.
	SetTimer(timer_check_version, 500, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLMMLoginManagerDlg::init_dialog()
{
	SetWindowText(theApp.m_product_name);

	LONG_PTR style = ::GetWindowLongPtr(m_hWnd, GWL_STYLE);

	//캡션 + 모든 테두리 제거
	style &= ~(WS_CAPTION | WS_THICKFRAME | WS_BORDER | WS_DLGFRAME);
	//style |= WS_THICKFRAME; //resize를 지원할 필요가 없을 경우는 주석처리한다.

	::SetWindowLongPtr(m_hWnd, GWL_STYLE, style);

	//반드시 필요 (프레임 다시 계산)
	::SetWindowPos(m_hWnd, nullptr, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		SWP_NOACTIVATE | SWP_FRAMECHANGED);

	//캡션바를 제거해도 직사각이 아닌 윈11처럼 라운드 모양으로.
	//XP/Win10 이하는 헬퍼 안에서 자동 no-op.
	win_compat::dwm::set_window_corner_round(m_hWnd);
}

void CLMMLoginManagerDlg::init_controls()
{
	m_theme.copy_colors_from(theApp.m_theme);

	//m_theme.set_color_theme(CSCColorTheme::color_theme_anysupport);
	//m_theme.set_color_theme(CSCColorTheme::color_theme_helpu);
	//origin 테마 (dark slate bg) 를 그대로 사용 — 이전 gGRAY(248) override 제거.

	//cr_back 등 dialog 측에서 가한 오버라이드까지 그대로 전달하기 위해 객체 자체를 넘김.
	theApp.m_msgbox.set_color_theme(m_theme);

	m_logo.load(IDB_LOGO);

	m_button_config.add_image(IDB_CONFIG);
	m_button_config.set_color_theme(m_theme);
	//m_button_config.set_parent_back_color(m_theme.cr_back);

	m_button_minimize.add_image(IDB_MINIMIZE);
	m_button_minimize.set_color_theme(m_theme);
	//m_button_minimize.set_parent_back_color(m_theme.cr_back);

	m_button_close.add_image(IDB_CLOSE);
	m_button_close.set_color_theme(m_theme);
	//m_button_close.set_parent_back_color(m_theme.cr_back);

	m_edit_id.set_color_theme(m_theme);
	m_edit_id.set_dim_text(_S(IDS_INPUT_ID));
	m_edit_id.set_prefix_image(IDB_USER);
	m_edit_id.set_round(6);

	m_edit_pw.set_color_theme(m_theme);
	m_edit_pw.set_dim_text(_S(IDS_INPUT_PASSWORD));
	m_edit_pw.set_prefix_image(IDB_PASSWORD);
	m_edit_pw.set_round(6);
	m_edit_pw.set_password_mode();
	m_edit_pw.set_action_button(CSCStaticEdit::action_password_toggle);

	m_check_save_pw.set_color_theme(m_theme);
	m_check_save_pw.set_text(_S(IDS_BTN_SAVE_PASSWORD));
	m_check_save_pw.set_check_style(CGdiButton::check_style_round_fill, m_theme.cr_button_back);
	//m_check_save_pw.set_text_color(m_theme.cr_text, false);
	m_check_save_pw.set_back_color(m_theme.cr_back, false);
	m_check_save_pw.SetCheck(theApp.m_ini["LOGIN"]["SAVE_PASSWORD"]);

	m_check_auto_login.set_color_theme(m_theme);
	m_check_auto_login.set_text(_S(IDS_BTN_AUTO_LOGIN));
	m_check_auto_login.set_check_style(CGdiButton::check_style_round_fill, m_theme.cr_button_back);
	//m_check_auto_login.set_text_color(m_theme.cr_text, false);
	m_check_auto_login.set_back_color(m_theme.cr_back, false);
	m_check_auto_login.SetCheck(theApp.m_ini["LOGIN"]["AUTO_LOGIN"]);

	m_button_login.set_color_theme(m_theme);
	m_button_login.set_text(_T("서버 연결중..."));
	//m_button_login.set_text_color(m_theme.cr_title_text, false);
	//m_button_login.set_back_color(m_theme.cr_back_selected);// , false);
	//m_button_login.set_parent_back_color(m_theme.cr_back);
	m_button_login.set_round(8);
	m_button_login.set_font_size(14);
	m_button_login.set_font_weight(FW_BOLD);
	m_button_login.copy_properties(m_button_restart);
	m_button_restart.ShowWindow(SW_HIDE);

	m_static_version.set_color_theme(m_theme);
	//m_static_version.set_back_color(m_theme.cr_back);
	//m_static_version.set_text_color(m_theme.cr_title_back_active);
	m_static_version.set_blink(true, 600, 400);
}

bool CLMMLoginManagerDlg::get_server()
{
	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString header = _T("token: ") + agent_token + _T("\r\n");

	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/server"));
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param);
	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		return false;
	}
	else
	{
		Json json;

		if (!json.parse(param.result))
		{
			logWriteE(_T("json parsing error."));
			return false;
		}

		logWrite(_T("result = \n%s"), json.get_json_str());

		rapidjson::Value& objs = json.doc["objects"];

		if (objs.Size() > 0)
		{
			if (!objs[0].HasMember("filetransfer_speed"))
			{
				//Config::SaveFileTransferSpeed(_T("0"));
				theApp.m_ini["FILE"]["SPEED"] = 0;
			}
			else
			{
				if (!objs[0]["filetransfer_speed"].IsNull())
				{
					//Config::SaveFileTransferSpeed(CString(std::to_string(objs[0]["filetransfer_speed"].GetInt()).c_str()));
					theApp.m_ini["FILE"]["SPEED"] = objs[0]["filetransfer_speed"].GetInt();
				}
				else
				{
					//Config::SaveFileTransferSpeed(_T("0"));
					theApp.m_ini["FILE"]["SPEED"] = 0;
				}
			}

			if (objs[0].HasMember("winapi_addr") && !objs[0]["winapi_addr"].IsNull() &&
				objs[0].HasMember("winapi_port") && !objs[0]["winapi_port"].IsNull() &&
				objs[0].HasMember("kms_addr") && !objs[0]["kms_addr"].IsNull() &&
				objs[0].HasMember("kms_port") && !objs[0]["kms_port"].IsNull() &&
				objs[0].HasMember("ap2p_addr") && !objs[0]["ap2p_addr"].IsNull() &&
				objs[0].HasMember("ap2p_port") && !objs[0]["ap2p_port"].IsNull())
			{
				theApp.m_ini["SERVER"]["WIP"]	= objs[0]["winapi_addr"].GetCString();
				theApp.m_ini["SERVER"]["WPORT"] = objs[0]["winapi_port"].GetInt();

				theApp.m_ini["SERVER"]["KIP"]	= objs[0]["kms_addr"].GetCString();
				theApp.m_ini["SERVER"]["KPORT"] = objs[0]["kms_port"].GetInt();

				theApp.m_ini["SERVER"]["RIP"]	= objs[0]["ap2p_addr"].GetCString();
				theApp.m_ini["SERVER"]["RPORT"] = objs[0]["ap2p_port"].GetInt();

				//Config::SaveNetAPIIP(objs[0]["winapi_addr"].GetCString());
				//Config::SaveNetAPIPort(CString(std::to_string(objs[0]["winapi_port"].GetInt()).c_str()));
				//Config::SaveKMSIP(objs[0]["kms_addr"].GetCString());
				//Config::SaveKMSPort(CString(std::to_string(objs[0]["kms_port"].GetInt()).c_str()));
				//Config::SaveAP2PIP(objs[0]["ap2p_addr"].GetCString());
				//Config::SaveAP2PPort(CString(std::to_string(objs[0]["ap2p_port"].GetInt()).c_str()));

				if (!objs[0].HasMember("thumb_port") || objs[0]["thumb_port"].GetInt() <= 0)
				{
					//Config::SaveThumbnailIP(objs[0]["winapi_addr"].GetCString());
					theApp.m_ini["SERVER"]["TIP"]	= objs[0]["winapi_addr"].GetCString();
					theApp.m_ini["SERVER"]["TPORT"] = objs[0]["winapi_port"].GetInt();

					if (objs[0]["winapi_port"].GetInt() >= 7000)
					{
						// 개발
						//Config::SaveThumbnailPort(_T("7003"));
						theApp.m_ini["SERVER"]["TPORT"] = 7003;
					}
					else {
						// 운영
						//Config::SaveThumbnailPort(_T("443"));
						theApp.m_ini["SERVER"]["TPORT"] = 443;
					}

				}
				else
				{
					//Config::SaveThumbnailIP(objs[0]["winapi_addr"].GetCString());
					//Config::SaveThumbnailPort(CUtil::intToString(objs[0]["thumb_port"].GetInt()));
					theApp.m_ini["SERVER"]["TIP"] = objs[0]["winapi_addr"].GetCString();
					theApp.m_ini["SERVER"]["TPORT"] = objs[0]["thumb_port"].GetInt();
				}
			}
		}

		return true;
	}

	return false;
}

void CLMMLoginManagerDlg::thread_get_version_and_login(CSCThread& th)
{
	//서버 연결 체크 — 네트워크 호출은 워커에서, UI 작업만 invoke_ui 로 마샬링.
	if (!is_server_reachable(theApp.m_ip, theApp.m_port))
	{
		invoke_ui([this]
		{
			m_button_login.set_text(_T("서버 연결 실패"));
			m_static_version.set_blink(false);
			m_static_version.set_text(_T("서버 연결 실패"), Gdiplus::Color::Crimson);
			theApp.m_msgbox.DoModal(_T("서버에 연결할 수 없습니다.\n네트워크 환경 또는 인터넷 연결 상태를 먼저 확인해주세요.\n\n또는 서버가 구동중일 수 있으니 잠시 후 다시 시도해주시기 바랍니다."));
			OnBnClickedCancel();
		});
		return;
	}



	if (th.stop_requested())
		return;

	m_button_login.set_text(_T("버전 확인중..."));
	Wait(10);

	//우선 SERVER -> GET_SERVER 설정에 따른 동작 처리
	if (theApp.m_ini["SERVER"]["GET_SERVER"].to_int() != 0)
	{
		get_server();
	}

	if (!get_current_version())
	{
		invoke_ui([this]
			{
				m_button_login.set_text(_T("현재 버전 확인 실패"));
				theApp.m_msgbox.DoModal(_T("현재 버전을 얻어올 수 없습니다."));
				OnBnClickedCancel();
			});
		return;
	}

	if (th.stop_requested())
		return;
	
	if (!get_latest_version())
	{
		invoke_ui([this]
			{
				m_button_login.set_text(_T("최신 버전 확인 실패"));
				theApp.m_msgbox.DoModal(_T("서버에서 버전 정보를 얻어올 수 없습니다."));
				OnBnClickedCancel();
			});
		return;
	}

	invoke_ui([this]
		{
			if (m_current_version == m_latest_version)
			{
				m_button_login.set_text(_S(IDS_BTN_LOGIN));
				m_static_version.set_blink(false);
				m_static_version.set_halign(DT_RIGHT);
				m_static_version.set_text_color(Gdiplus::Color::LightGray);
				m_static_version.set_text(_T("ver ") + m_current_version);
				m_check_auto_login.EnableWindow();
				m_check_save_pw.EnableWindow();
				m_button_login.EnableWindow();

				m_edit_id.set_text(CString(theApp.m_ini["LOGIN"]["ID"]));
				CString pw = theApp.m_ini["LOGIN"]["PASS"];
				if (!pw.IsEmpty())
				{
					pw = Util::EnCryptPassword(pw);
					m_edit_pw.set_text(pw);
				}

				m_edit_id.SetFocus();

				//로그인을 진행하기 전 현재 에이전트의 로그인 상태 정보를 요청한다.
				//true : 이미 로그인 된 에이전트이므로 로그인 완료 상태로 처리하고 로그인 후처리 진행
				//false : ReadLoginInfo() → 아직 로그인 전이고 자동 로그인 설정이라면 로그인 시도 진행(m_dlgLogin.Login())
				get_device_onoff_status();

				if (m_login_state == LOGIN_OK)
				{
					select_child_dialog();
				}
				//자동 로그인 옵션이 켜져있으면 ID, PW를 입력시키고 자동 로그인 시도.
				else if (m_check_auto_login.GetCheck() == BST_CHECKED)
				{
					OnBnClickedButtonLogin();
				}
				else
				{
					//원래 이 블록은 맨 위에 있어야 하지만 LOGIN_OK 또는 자동 로그인시에는 다시 disable 처리를 하므로
					//불필요하게 깜빡이게 된다.
					//그래서 로그인 상태도 아니고 자동 로그인도 아닐 경우 여기서 enable 시켜야 깔끔하다.
					m_edit_id.EnableWindow();
					m_edit_pw.EnableWindow();
				}
			}
			else
			{
				m_button_login.set_text(_S(IDS_BTN_UPDATE));
				m_button_login.EnableWindow(FALSE);

				CString str;

				str.Format(_T("현재 버전(%s)보다 최신 버전(%s)가 존재합니다. 자동 패치를 진행합니다."), m_current_version, m_latest_version);
				theApp.m_msgbox.DoModal(str, MB_OK, 5);

				str.Format(_T("%s\\AutoPatcher.exe"), get_exe_directory());
				ShellExecute(m_hWnd, _T("open"), str, nullptr, nullptr, SW_SHOW);
			}
		});
}

void CLMMLoginManagerDlg::invoke_ui(std::function<void()> func)
{
	const HWND h_wnd = GetSafeHwnd();
	if (!::IsWindow(h_wnd))
		return;

	auto* p_func = new std::function<void()>(std::move(func));
	if (!::PostMessage(h_wnd, WM_APP_UI_INVOKE, 0, reinterpret_cast<LPARAM>(p_func)))
		delete p_func;
}

LRESULT CLMMLoginManagerDlg::on_ui_invoke(WPARAM /*wParam*/, LPARAM lParam)
{
	std::unique_ptr<std::function<void()>> p_func(
		reinterpret_cast<std::function<void()>*>(lParam));
	(*p_func)();
	return 0;
}

void CLMMLoginManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLMMLoginManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc1(this);
		CRect rc;

		GetClientRect(rc);
		CMemoryDC dc(&dc1, &rc);

		dc.FillSolidRect(rc, m_theme.cr_back.ToCOLORREF());

		Gdiplus::Graphics g(dc);
		g.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
		g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		g.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAliasGridFit);

		CRect r = rc;
		r.top = 40;
		r.bottom = r.top + m_logo.height;
		m_logo.draw(g, r);

		r.OffsetRect(0, m_logo.height + 12);
		draw_text(g, r, _S(IDS_TITLE) + _T(" 1.0"), 16, Gdiplus::FontStyleBold, 0, 0.0f, _T("Segoe UI"), m_theme.cr_text);

		r = rc;
		r.top = r.bottom - 40;
		draw_text(g, r, _S(IDS_TITLE) + _T(" 1.0"), 10, Gdiplus::FontStyleBold, 0, 0.0f, _T("Segoe UI"), m_theme.cr_text);

		r.right -= 4;
		//r.bottom += 4;
		draw_text(g, r, m_current_version, 7, Gdiplus::FontStyleRegular, 0, 0.0f, _T("Segoe UI"),
			get_weak_color(m_theme.cr_text, -40),
			Gdiplus::Color::Transparent,
			Gdiplus::Color::Transparent,
			Gdiplus::Color::Transparent,
			DT_RIGHT | DT_VCENTER);
		
#ifdef _USING_V110_SDK71_
		draw_rect(g, rc, Gdiplus::Color::DimGray);
#endif
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLMMLoginManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLMMLoginManagerDlg::OnBnClickedOk()
{
}

void CLMMLoginManagerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CLMMLoginManagerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CLMMLoginManagerDlg::OnBnClickedButtonConfig()
{
	CString agent_path = get_exe_directory() + _T("\\LMMAgent.exe");
	ShellExecute(NULL, _T("open"), agent_path, _T("-configservice"), NULL, SW_SHOW);
}

void CLMMLoginManagerDlg::OnBnClickedButtonMinimize()
{
	ShowWindow(SW_MINIMIZE);
}

void CLMMLoginManagerDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}

void CLMMLoginManagerDlg::OnBnClickedCheckSavePw()
{
	theApp.m_ini["LOGIN"]["SAVE_PASSWORD"] = m_check_save_pw.GetCheck();
}

void CLMMLoginManagerDlg::OnBnClickedCheckAutoLogin()
{
	theApp.m_ini["LOGIN"]["AUTO_LOGIN"] = m_check_auto_login.GetCheck();
}

void CLMMLoginManagerDlg::OnBnClickedButtonLogin()
{
	if (m_login_state == LOGIN_BEFORE)
	{
		if (validate_login_input())
		{
			m_edit_id.EnableWindow(FALSE);
			m_edit_pw.EnableWindow(FALSE);
			Wait(10);
			service_start();
		}
		else if (m_edit_id.get_text().GetLength() == 0)
		{
			theApp.m_msgbox.DoModal(_S(IDS_INPUT_ID));
		}
		else if (m_edit_pw.get_text().GetLength() == 0)
		{
			theApp.m_msgbox.DoModal(_S(IDS_INPUT_PASSWORD));
		}
	}
	else if (m_login_state == LOGIN_OK)
	{
		//이미 로그인 된 상태에서 "로그아웃" 클릭 시
		//auto login이면 서비스를 중지하고
		if (theApp.m_ini["LOGIN"]["AUTO_LOGIN"])
		{
			service_stop();
		}
		//auto login이 아니면 서비스를 삭제한다.
		else
		{
			service_stop(true);
		}

		set_login_state(LOGIN_BEFORE);
		select_child_dialog();
		theApp.terminate_other_processes();
	}
}

bool CLMMLoginManagerDlg::service_start()
{
	//서비스가 이미 실행중이거나 설치되어 있을 경우 제거 후 새로 등록하여 실행시키도록 한다.
	service_stop(true);

	//서비스를 등록하고
	logWrite(_T("install service (%s)..."), theApp.m_service_name);
	CString agent_path = get_exe_directory() + _T("\\LMMAgent.exe");
	ShellExecute(NULL, _T("open"), agent_path, _T("-install"), NULL, SW_SHOW);

	//서비스가 정상 등록될 때까지 기다렸다가 구동시킨다.
	while (true)
	{
		DWORD error_code = 0;
		CString detail;
		DWORD status_code = service_command(theApp.m_service_name, _T("query"), error_code, &detail);

		//등록이 실패한 경우
		if (status_code < 1)
		{
			CString str = logWrite(_T("waiting for the service to be installed..."), theApp.m_service_name, error_code, detail);

			Sleep(1000);
			continue;
			//theApp.m_msgbox.DoModal(str);
			//return false;
		}
		
		break;
	}

	logWrite(_T("service installed successfully. start service..."));

	//서비스가 정상 등록되었다면 구동시킨다.
	ShellExecute(NULL, _T("open"), agent_path, _T("-start"), NULL, SW_SHOW);

	//이제 LMMAgent가 구동되면서 로그인 결과를 UDP로 전달하게 된다.
	return true;
}

bool CLMMLoginManagerDlg::service_stop(bool include_delete)
{
	DWORD error_code = 0;
	CString detail;
	CString str;

	if (service_command(theApp.m_service_name, _T("query"), error_code, &detail) == 0)
	{
		//str = logWrite(_T("service name = %s, query fail. error_code = %d (%s)"), theApp.m_service_name, error_code, detail);
		//theApp.m_msgbox.DoModal(str);
		return true;
	}

	//기존 동일한 이름으로 서비스가 실행중일때에도 제거 후 install, start 시킨다.
	//이렇게 하는 이유는 간혹 솔루션을 테스트 할 경우 해당 서비스와 동일한 이름으로 서비스가 등록되었거나 실행중일 경우에는
	//다른 경로의 LMMAgent.exe를 서비스로 구동시키므로 제대로 동작될 리 없다.
	//따라서 이미 등록되어 있는 서비스라도	 무조건 제거 후 새로 등록하여 실행시키도록 한다.
	if (error_code != ERROR_SERVICE_DOES_NOT_EXIST)//서비스가 존재할 경우 (실행중이든 중지중이든)
	{
		if (include_delete)
		{
			if (service_command(theApp.m_service_name, _T("delete"), error_code, &detail) == 0)
			{
				str = logWrite(_T("service delete fail. error_code = %d (%s)"), error_code, detail);
				//theApp.m_msgbox.DoModal(str);
				return false;
			}
			else
			{
				logWrite(_T("service delete success."));
			}
		}
		else
		{
			if (service_command(theApp.m_service_name, _T("stop"), error_code, &detail) == 0)
			{
				str = logWrite(_T("service stop fail. error_code = %d (%s)"), error_code, detail);
				return false;
			}
			else
			{
				logWrite(_T("service stop success."));
			}
		}
	}

	return true;
}

bool CLMMLoginManagerDlg::get_current_version()
{
	get_registry_str(HKEY_LOCAL_MACHINE, theApp.m_reg_path, _T("Version"), &m_current_version);

	if (m_current_version.IsEmpty())
	{
		//m_current_version = _T("0.0.0.0");	=> kms에서 에러 발생함.
		//맨 처음 실행이라서 로컬버전정보가 없다면 LMMAgent.exe의 ProductVersion을 사용한다.
		m_current_version = get_file_property(get_exe_directory() + _T("\\LMMAgent.exe"), _T("ProductVersion"));
		logWrite(_T("m_current_version is empty on first run. use ProductVersion of LMMAgent.exe"));
	}

	//3.4 -> 3.4.0.0 or 3.4.0 => 3.4.0.0과 같이 4자릿수로 변환한다.
	int dot_count = get_char_count(m_current_version, '.');
	while (dot_count < 3)
	{
		m_current_version += _T(".0");
		dot_count++;
	}

	Invalidate();
	logWrite(_T("m_current_version = %s"), m_current_version);
	return true;
}

bool CLMMLoginManagerDlg::get_latest_version()
{
	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/download/agent/program_kr/version.html"));
	request_url(&param);

	if (param.status != HTTP_STATUS_OK)
	{
		CString str = logWrite(_T("Failed to get latest version info. status: %d"), param.status);
		theApp.m_msgbox.DoModal(str);
		return false;
	}
	else
	{
		CString version_info = param.result;
		version_info.Trim();

		int dot_count = get_char_count(version_info, '.');

		while (dot_count < 3)
		{
			version_info += _T(".0");
			dot_count++;
		}

		m_latest_version = version_info;
	}

	logWrite(_T("m_latest_version = %s"), m_latest_version);

	return true;
}

bool CLMMLoginManagerDlg::validate_login_input()
{
	if (m_edit_id.get_text().IsEmpty() || m_edit_pw.get_text().IsEmpty())
		return false;

	return true;
}

BOOL CLMMLoginManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CLMMLoginManagerDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CLMMLoginManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_check_version)
	{
		KillTimer(timer_check_version);

		m_thread.start([this](CSCThread& th) { thread_get_version_and_login(th); });
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLMMLoginManagerDlg::select_child_dialog()
{
	if (m_login_state == LOGIN_BEFORE)
	{
		m_button_restart.ShowWindow(SW_HIDE);
		m_edit_id.EnableWindow(TRUE);
		m_edit_pw.EnableWindow(TRUE);
		m_edit_id.ShowWindow(SW_SHOW);
		m_edit_pw.ShowWindow(SW_SHOW);
		m_button_login.set_text(_T("로그인"));
	}
	else if (m_login_state == LOGIN_OK)
	{
		m_edit_id.ShowWindow(SW_HIDE);
		m_edit_pw.ShowWindow(SW_HIDE);
		m_button_restart.ShowWindow(SW_SHOW);
		m_button_login.set_text(_T("로그아웃"));
	}
}

//online(>0), offline(0), request error(<0)
int CLMMLoginManagerDlg::get_device_onoff_status()
{
	logWrite(_T("%s"), __function__);

	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString header = _T("token: ") + agent_token + _T("\r\n");
	CString device_id = theApp.m_ini["SERVER"]["DID"];

	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/GetLmmDeviceOnOff"), _T("POST"));
	param.body.Format(_T("{\"device_id\":\"%s\"}"), device_id);
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param);
	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		return -1;
	}
	else
	{
		Json json;

		if (!json.parse(param.result))
		{
			logWrite(_T("json parsing error."));
			return -1;
		}

		logWrite(_T("result = \n%s"), json.get_json_str());

		rapidjson::Value& objs = json.doc["objects"];

		if (objs.Size() > 0)
		{
			CString deviceName = objs[0]["device_name"].GetCString();
			CString onOffStr = objs[0]["b_onoff"].GetCString();
			if (onOffStr == _T("TRUE"))
			{
				m_login_state = LOGIN_OK;
			}
			else
			{
				m_login_state = LOGIN_BEFORE;
			}

			if (m_login_state == LOGIN_BEFORE)
			{
				CString parameter = _S(IDS_AGENT_CMD_STOP);
				ShellExecute(NULL, _T("open"), get_exe_directory(true) + _T("LMMAgent.exe"), parameter, NULL, SW_SHOW);
			}

			theApp.m_ini["SERVER"]["DNAME"] = deviceName;

			return m_login_state;
		}
	}

	return -1;
}

void CLMMLoginManagerDlg::request_put_device_env_info()
{
	CString browser_path;
	CString browser_version;
	CString default_browser = get_default_browser_info(&browser_path, &browser_version);

	CRequestUrlParams params(theApp.m_ip, theApp.m_port, _T("/lmm/api/v1.0/device_env_info"), _T("POST"));
	params.use_thread = true;
	params.body.Format(_T("{\"method\": \"PUT\", \"request_type\": \"agent\", \"login_id\": \"%s\", \"device_id\": \"%s\", \"os_type\": 1, \"os_str\": \"%s\", \"default_browser\": \"%s\", \"default_browser_version\": \"%s\"}"),
		CString(theApp.m_ini["LOGIN"]["ID"]),// Config::LoadConfigString(_T("LOGIN"), _T("ID"), _T("")),
		CString(theApp.m_ini["SERVER"]["DID"]),//Config::LoadConfigString(_T("SERVER"), _T("DID"), _T("")),
		get_windows_version_number(),
		default_browser,
		browser_version);

	logWrite(_T("%s"), params.get_param_str());
	request_url(&params);
	logWrite(_T("status = %d, result = %s"),
		params.status, params.result);

	if (params.status == HTTP_STATUS_OK)
	{
		logWrite(_T("request success."));
	}
	else
	{
		logWriteE(_T("request fail."));
	}

	OnBnClickedCancel();
}

void CLMMLoginManagerDlg::OnBnClickedButtonRestart()
{
	logWrite(_T("%s"), __function__);
	CString patcher_path = get_exe_directory() + _T("\\AutoPatcher.exe");
	ShellExecute(NULL, _T("open"), patcher_path, _S(IDS_AGENT_CMD_RESTART), NULL, SW_SHOW);
}

void CLMMLoginManagerDlg::OnDestroy()
{
	// 로그인 안된 상태에서 종료될 경우, 모두 강제 종료한다. - pjh
	if (m_login_state == LOGIN_BEFORE)
	{
		CString exe_path = get_exe_directory() + _T("\\LMMAgent.exe");
		while (get_process_running_count(exe_path) > 0)
			kill_process_by_fullpath(exe_path);

		theApp.terminate_other_processes();
	}

	CDialogEx::OnDestroy();
}

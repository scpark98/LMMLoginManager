
// LMMLoginManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LMMLoginManager.h"
#include "LMMLoginManagerDlg.h"
#include "afxdialogex.h"

#include "Util.h"

#include "Common/MemoryDC.h"

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
	init_dialog();
	init_controls();

	RestoreWindowPosition(&theApp, this, _T(""), false, false);

	//실제로는 100이며 서버 체크 목적으로 5000으로 설정.
	SetTimer(timer_check_version, 500, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLMMLoginManagerDlg::init_dialog()
{
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
	//XP에서는 지원되지 않을것이다.
#ifndef _USING_V110_SDK71_
	DWORD corner = DWMWCP_ROUND;
	DwmSetWindowAttribute(m_hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
#endif
}

void CLMMLoginManagerDlg::init_controls()
{
	m_theme.set_color_theme(CSCColorTheme::color_theme_linkmemine);

	m_msgbox.create(this, _S(IDS_TITLE));// , IDR_MAINFRAME);
	m_msgbox.set_color_theme(m_theme.get_color_theme());

	m_logo.load(IDB_LOGO);

	m_button_config.add_image(IDB_CONFIG);
	//m_button_config.set_parent_back_color(m_theme.cr_parent_back);

	m_button_minimize.add_image(IDB_MINIMIZE);
	//m_button_minimize.set_parent_back_color(m_theme.cr_parent_back);

	m_button_close.add_image(IDB_CLOSE);
	//m_button_close.set_parent_back_color(m_theme.cr_parent_back);

	m_edit_id.set_color_theme(m_theme.get_color_theme());
	m_edit_id.set_dim_text(_T("아이디를 입력하세요"));
	m_edit_id.set_prefix_image(IDB_USER);
	m_edit_id.set_round(8);

	m_edit_pw.set_color_theme(m_theme.get_color_theme());
	m_edit_pw.set_dim_text(_T("비밀번호를 입력하세요"));
	m_edit_pw.set_prefix_image(IDB_PASSWORD);
	m_edit_pw.set_round(8);
	m_edit_pw.set_password_mode();
	m_edit_pw.set_action_button(CSCStaticEdit::action_password_toggle);

	m_check_save_pw.set_check_style(CGdiButton::check_style_round_fill, m_theme.cr_title_back_inactive);
	m_check_save_pw.set_text_color(m_theme.cr_text, false);
	m_check_save_pw.set_back_color(m_theme.cr_back, false);
	m_check_save_pw.SetCheck(theApp.m_ini["LOGIN"]["SAVE_PASSWORD"]);

	m_check_auto_login.set_check_style(CGdiButton::check_style_round_fill, m_theme.cr_title_back_inactive);
	m_check_auto_login.set_text_color(m_theme.cr_text, false);
	m_check_auto_login.set_back_color(m_theme.cr_back, false);
	m_check_auto_login.SetCheck(theApp.m_ini["LOGIN"]["AUTO_LOGIN"]);

	m_button_login.set_text(_T("로그인"));
	m_button_login.set_text_color(m_theme.cr_title_text, false);
	m_button_login.set_back_color(m_theme.cr_title_back_inactive);// , false);
	m_button_login.set_round(8);
	m_button_login.set_font_size(14);
	m_button_login.set_font_weight(FW_BOLD);

	m_static_version.set_back_color(m_theme.cr_back);
	m_static_version.set_text_color(m_theme.cr_title_back_active);
	m_static_version.set_blink(true, 600, 400);
}

void CLMMLoginManagerDlg::thread_get_version_info(CSCThread& th)
{
	//서버 연결 체크 — 네트워크 호출은 워커에서, UI 작업만 invoke_ui 로 마샬링.
	if (!is_server_reachable(theApp.m_server_ip, theApp.m_server_port))
	{
		invoke_ui([this]
		{
			m_static_version.set_blink(false);
			m_static_version.set_text(_T("서버 연결 실패"), Gdiplus::Color::Crimson);
			m_msgbox.DoModal(_T("서버에 연결할 수 없습니다.\n네트워크 환경 또는 인터넷 연결 상태를 확인하세요."));
			OnBnClickedCancel();
		});
		return;
	}

	if (th.stop_requested())
		return;

	if (!get_current_version())
	{
		invoke_ui([this]
			{
				m_msgbox.DoModal(_T("현재 버전을 얻어올 수 없습니다."));
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
				m_msgbox.DoModal(_T("서버에서 버전정보를 얻어올 수 없습니다."));
				OnBnClickedCancel();
			});
		return;
	}

	invoke_ui([this]
		{
			if (m_current_version == m_latest_version)
			{
				m_static_version.set_blink(false);
				m_static_version.set_halign(DT_RIGHT);
				m_static_version.set_text_color(Gdiplus::Color::LightGray);
				m_static_version.set_text(_T("ver ") + m_current_version);
				m_edit_id.EnableWindow();
				m_edit_pw.EnableWindow();
				m_check_auto_login.EnableWindow();
				m_check_save_pw.EnableWindow();
				m_button_login.EnableWindow();

				m_edit_id.SetFocus();

				//자동 로그인 옵션이 켜져있으면 ID, PW를 입력시키고 자동 로그인 시도.
				if (m_check_auto_login.GetCheck() == BST_CHECKED)
				{
					m_edit_id.set_text(CString(theApp.m_ini["LOGIN"]["ID"]));
					CString pw = theApp.m_ini["LOGIN"]["PASS"];
					if (!pw.IsEmpty())
					{
						pw = Util::EnCryptPassword(pw);
						m_edit_pw.set_text(pw);
					}

					//OnBnClickedButtonLogin();
				}
			}
			else
			{
				CString str;

				str.Format(_T("현재 버전(%s)보다 최신 버전(%s)가 존재합니다. 자동 패치를 진행합니다."), m_current_version, m_latest_version);
				m_msgbox.DoModal(str, MB_OK, 5);

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
		draw_text(g, r, _S(IDS_TITLE) + _T(" 1.0"), 16, Gdiplus::FontStyleBold, 0, 0.0f, _T("Segoe UI"), m_theme.cr_title_back_active);

		r = rc;
		r.top = r.bottom - 40;
		draw_text(g, r, _S(IDS_TITLE) + _T(" 1.0"), 10, Gdiplus::FontStyleBold, 0, 0.0f, _T("Segoe UI"), get_weak_color(m_theme.cr_back, 80));
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLMMLoginManagerDlg::OnBnClickedCheckAutoLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLMMLoginManagerDlg::OnBnClickedButtonLogin()
{
	if (validate_login_input())
	{
		service_start();
	}
	else
	{
		m_msgbox.DoModal(_T("ID 또는 패스워드가 입력되지 않았습니다."));
	}
}

bool CLMMLoginManagerDlg::service_start()
{
	if (!service_stop(true))
		return false;

	CString agent_path = get_exe_directory() + _T("\\LMMAgent.exe");
	ShellExecute(NULL, _T("open"), agent_path, _T("-start"), NULL, SW_SHOW);

	return true;
}

bool CLMMLoginManagerDlg::service_stop(bool include_delete)
{
	DWORD error_code = 0;
	CString detail;
	CString str;

	if (service_command(theApp.m_svc_name, _T("query"), error_code, &detail) == 0)
	{
		str = logWrite(_T("service query fail. error_code = %d (%s)"), error_code, detail);
		m_msgbox.DoModal(str);
		return false;
	}

	//기존 동일한 이름으로 서비스가 실행중일때에도 제거 후 install, start 시킨다.
	//이렇게 하는 이유는 간혹 솔루션을 테스트 할 경우 해당 서비스와 동일한 이름으로 서비스가 등록되었거나 실행중일 경우에는
	//다른 경로의 LMMAgent.exe를 서비스로 구동시키므로 제대로 동작될 리 없다.
	//따라서 이미 등록되어 있는 서비스라도	 무조건 제거 후 새로 등록하여 실행시키도록 한다.
	if (include_delete && error_code != ERROR_SERVICE_DOES_NOT_EXIST)//서비스가 설치되어 있을 경우 (실행중이든 중지중이든)
	{
		if (service_command(theApp.m_svc_name, _T("delete"), error_code, &detail) == 0)
		{
			str = logWrite(_T("service delete fail. error_code = %d (%s)"), error_code, detail);
			m_msgbox.DoModal(str);
			return false;
		}
	}
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

	logWrite(_T("m_current_version = %s"), m_current_version);
	return true;
}

bool CLMMLoginManagerDlg::get_latest_version()
{
	CRequestUrlParams param(theApp.m_server_ip, theApp.m_server_port, _T("/download/agent/program_kr/version.html"));
	request_url(&param);

	if (param.status != HTTP_STATUS_OK)
	{
		CString str = logWrite(_T("Failed to get latest version info. status: %d"), param.status);
		m_msgbox.DoModal(str);
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
		m_thread.start([this](CSCThread& th) { thread_get_version_info(th); });
	}

	CDialogEx::OnTimer(nIDEvent);
}

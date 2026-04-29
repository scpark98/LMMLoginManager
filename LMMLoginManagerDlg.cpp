
// LMMLoginManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LMMLoginManager.h"
#include "LMMLoginManagerDlg.h"
#include "afxdialogex.h"

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

	//RestoreWindowPosition(&theApp, this, _T(""), false, false);

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
	m_theme.set_color_theme(CSCColorTheme::color_theme_white);

	m_button_config.add_image(IDB_CONFIG);
	m_button_config.set_parent_back_color(m_theme.cr_parent_back);

	m_button_minimize.add_image(IDB_MINIMIZE);
	m_button_minimize.set_parent_back_color(m_theme.cr_parent_back);

	m_button_close.add_image(IDB_CLOSE);
	m_button_close.set_parent_back_color(m_theme.cr_parent_back);

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

	m_check_save_pw.set_check_style(CGdiButton::check_style_round_fill);
	m_check_save_pw.set_text_color(m_theme.cr_text, false);
	m_check_save_pw.set_back_color(m_theme.cr_back, false);

	m_check_auto_login.set_check_style(CGdiButton::check_style_round_fill);
	m_check_auto_login.set_text_color(m_theme.cr_text, false);
	m_check_auto_login.set_back_color(m_theme.cr_back, false);
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

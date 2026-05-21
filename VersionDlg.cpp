// VersionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "VersionDlg.h"

#include "Common/MemoryDC.h"
#include "Common/win_compat/dwm.h"

// CVersionDlg 대화 상자

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VERSION, pParent)
{

}

CVersionDlg::~CVersionDlg()
{
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_static_title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button_close);
	DDX_Control(pDX, IDC_STATIC_VERSION_LABEL, m_static_version_label);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_static_version);
	DDX_Control(pDX, IDC_STATIC_COPYRIGHT, m_static_copyright);
}


BEGIN_MESSAGE_MAP(CVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVersionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVersionDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CVersionDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CVersionDlg 메시지 처리기

BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_theme.copy_colors_from(theApp.m_theme);
	win_compat::dwm::set_window_corner_round(m_hWnd);

	//m_theme.set_color_theme(CSCColorTheme::color_theme_dark_gray);
	m_theme.copy_colors_from(theApp.m_theme);

	m_static_title.set_text_color(m_theme.cr_title_text);
	m_static_title.set_back_color(m_theme.cr_title_back_inactive);
	m_static_title.set_font_weight(FW_BOLD);
	m_static_title.set_text(_T("  ") + _S(IDS_TITLE));

	m_button_close.add_image(IDB_CLOSE);
	m_button_close.set_back_color(m_theme.cr_title_back_inactive);

	m_static_version_label.set_text_color(m_theme.cr_text);
	m_static_version_label.set_back_color(m_theme.cr_back);
	m_static_version_label.copy_properties(m_static_version);
	m_static_version_label.copy_properties(m_static_copyright);

	m_static_version.set_text(get_file_property(get_exe_directory() + _T("\\LMMAgent.exe"), _T("ProductVersion")));
	m_static_copyright.set_text(get_file_property(get_exe_directory() + _T("\\LMMAgent.exe"), _T("LegalCopyright")));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CVersionDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CVersionDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CVersionDlg::OnPaint()
{
	CPaintDC dc1(this);

	CRect rc;
	GetClientRect(rc);

	CMemoryDC dc(&dc1, &rc);
	dc.FillSolidRect(rc, m_theme.cr_back.ToCOLORREF());
}

void CVersionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CVersionDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}

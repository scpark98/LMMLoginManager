// DeviceNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "DeviceNameDlg.h"

#include "Common/MemoryDC.h"

// CDeviceNameDlg 대화 상자

IMPLEMENT_DYNAMIC(CDeviceNameDlg, CDialogEx)

CDeviceNameDlg::CDeviceNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEVICE_NAME, pParent)
{

}

CDeviceNameDlg::~CDeviceNameDlg()
{
}

void CDeviceNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_static_title);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_static_message);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button_close);
	DDX_Control(pDX, IDOK, m_button_ok);
	DDX_Control(pDX, IDCANCEL, m_button_cancel);
}


BEGIN_MESSAGE_MAP(CDeviceNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeviceNameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeviceNameDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDeviceNameDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDeviceNameDlg 메시지 처리기

BOOL CDeviceNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD corner = DWMWCP_ROUND;
	DwmSetWindowAttribute(m_hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));

	m_theme.set_color_theme(CSCColorTheme::color_theme_dark_gray);

	m_static_title.set_text_color(m_theme.cr_title_text);
	m_static_title.set_back_color(m_theme.cr_title_back_inactive);
	m_static_title.set_font_weight(FW_BOLD);
	m_static_title.set_text(_T("  ") + _S(IDS_TITLE));

	m_button_close.add_image(IDB_CLOSE);
	m_button_close.set_back_color(m_theme.cr_title_back_inactive);


	m_static_message.set_text_color(m_theme.cr_text);
	m_static_message.set_back_color(m_theme.cr_back);
	m_static_message.set_font_weight(FW_BOLD);
	//m_static_message.set_font_size(12);

	m_edit_name.set_color_theme(m_theme);
	m_edit_name.set_text_color(Gdiplus::Color::Black);
	m_edit_name.set_back_color(Gdiplus::Color::White);
	//m_edit_name.set_round(6);

	m_button_ok.set_text_color(m_theme.cr_title_back_inactive);
	m_button_ok.set_back_color(m_theme.cr_title_text);
	m_button_ok.set_font_weight(FW_BOLD);

	m_button_cancel.set_text_color(m_theme.cr_title_back_inactive);
	m_button_cancel.set_back_color(m_theme.cr_title_text);
	m_button_cancel.set_font_weight(FW_BOLD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceNameDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CDeviceNameDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CDeviceNameDlg::OnPaint()
{
	CPaintDC dc1(this);

	CRect rc;
	GetClientRect(rc);

	CMemoryDC dc(&dc1, &rc);
	dc.FillSolidRect(rc, m_theme.cr_back.ToCOLORREF());
}

void CDeviceNameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDeviceNameDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}

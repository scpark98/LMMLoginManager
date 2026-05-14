// PositionDlg.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "PositionDlg.h"
//#include "Company.h"
#include "Share.h"
#include "afxdialogex.h"

#include "Common/MemoryDC.h"

// CPositionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPositionDlg, CDialogEx)

CPositionDlg::CPositionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POSITION, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPositionDlg::~CPositionDlg()
{
}

void CPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_USE, m_btnUse);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CPositionDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPositionDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPositionDlg::OnBnClickedBtnClose)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_USE, &CPositionDlg::OnBnClickedCheckUse)
END_MESSAGE_MAP()


// CPositionDlg 메시지 처리기입니다.


BOOL CPositionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ini.SetFileName(get_exe_directory() + _T("\\share.ini"));

	int nLeft = m_ini["SHARE"]["LEFT"];
	int nTop = m_ini["SHARE"]["TOP"];
	int nWidth = m_ini["SHARE"]["WIDTH"];
	int nHeight = m_ini["SHARE"]["HEIGHT"];
	int nUse = m_ini["SHARE"]["USE"];
	if (nUse)
	{
		m_btnUse.SetCheck(TRUE);
		m_btnSave.EnableWindow(TRUE);
	}
	else
	{
		m_btnUse.SetCheck(FALSE);
		m_btnSave.EnableWindow(FALSE);
	}

	MoveWindow(nLeft - 2, nTop - 32, nWidth + 4, nHeight + 34);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	LONG lResult = SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	if (!lResult)
	{
		return FALSE;
	}

	BOOL bResult = SetLayeredWindowAttributes(RGB(255, 255, 0), 0, LWA_COLORKEY);
	if (!bResult)
	{
		return FALSE;
	}

	m_btnClose.add_image(IDB_CLOSE);
	m_btnClose.set_back_color(Gdiplus::Color(82, 87, 92));
	//m_btnClose.SetSolidButton(Color(82, 87, 92), Color(102, 107, 112), Color(62, 67, 72), Color(82, 87, 92), 20, 20);
	//m_btnClose.SetButtonIcon(IDP_CLOSE, IDP_CLOSE_OVER, IDP_CLOSE_SEL, IDP_CLOSE_DIS, 2, 2, 16, 16);

	m_btnUse.add_image(IDB_OFF);
	m_btnUse.add_image(IDB_ON);
	m_btnUse.set_back_color(Gdiplus::Color(82, 87, 92), false);
	//m_btnUse.SetSolidButton(Color(82, 87, 92), Color(82, 87, 92), Color(82, 87, 92), Color(82, 87, 92), 32, 20);
	//m_btnUse.SetButtonIcon(IDP_OFF, IDP_OFF, IDP_ON, IDP_OFF, 0, 0, 32, 20);

	//이 대화창은 컬러키를 yellow로 지정해서 투명하게 보이는 대화창이므로
	//저장 버튼 이미지의 투명 영역을 alpha값 그대로 사용하면 흰색으로 표시되는 부작용이 있다.
	//따라서 이미지 또한 투명 영역을 yellow로 하고 다음과 같이 설정하니 버튼도 이미지만 정확히 표현된다.
	m_btnSave.add_image(IDB_SAVE, IDB_SAVE, IDB_SAVE, IDB_SAVE_DISABLED);
	m_btnSave.fit_to_image(true);
	m_btnSave.set_parent_back_color(Gdiplus::Color(255, 255, 0));
	m_btnSave.set_transparent();
	m_btnSave.set_down_offset(1, 1);
	//m_btnSave.set_back_color(Gdiplus::Color(82, 87, 92));
	//m_btnSave.SetSolidButton(Color(242, 242, 242), Color(202, 202, 202), Color(142, 142, 142), Color(100, 100, 100), 48, 48);
	//m_btnSave.SetButtonIcon(IDP_SAVE, IDP_SAVE, IDP_SAVE, IDP_SAVE, 12, 12, 24, 24);
	//m_btnSave.SetBorder(TRUE, Color(105, 105, 105));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CPositionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPositionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int hitCode = 0;

	CRect rw;
	GetClientRect(&rw);

	if ((point.x >= rw.left && point.x < 5) && (point.y >= rw.top && point.y < 5))
		hitCode = HTTOPLEFT;
	else if ((point.x <= rw.right && point.x > rw.right - 5) && (point.y >= rw.top && point.y < 5))
		hitCode = HTTOPRIGHT;
	else if ((point.x >= rw.left && point.x < 5) && (point.y <= rw.bottom && point.y > rw.bottom - 5))
		hitCode = HTBOTTOMLEFT;
	else if ((point.x <= rw.right && point.x > rw.right - 5) && (point.y <= rw.bottom && point.y > rw.bottom - 5))
		hitCode = HTBOTTOMRIGHT;
	else if ((point.x <= rw.right && point.x > rw.right - 5) && (point.y > rw.top + 15 && point.y < rw.bottom - 5))
		hitCode = HTRIGHT;
	else if ((point.x >= rw.left && point.x < 5) && (point.y > rw.top + 15 && point.y < rw.bottom - 5))
		hitCode = HTLEFT;
	else if ((point.x > rw.left + 5 && point.x < rw.right - 5) && (point.y >= rw.top && point.y < 5))
		hitCode = HTTOP;
	else if ((point.x > rw.left + 5 && point.x < rw.right - 5) && (point.y <= rw.bottom && point.y > rw.bottom - 5))
		hitCode = HTBOTTOM;

	if (hitCode != 0)
	{
		SendMessage(WM_NCLBUTTONDOWN, hitCode, MAKELPARAM(point.x, point.y));
	}

	if (point.y <= 31)
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CPositionDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CPositionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		Gdiplus::Graphics graphics(dc);

		CRect rect;
		GetClientRect(&rect);

		// 더블버퍼링
		Gdiplus::Bitmap bitmap(rect.Width(), rect.Height());
		Gdiplus::Graphics g(&bitmap);

		Gdiplus::SolidBrush outBR(Gdiplus::Color(43, 43, 43));
		g.FillRectangle(&outBR, 0, 0, rect.Width(), rect.Height());


		Gdiplus::SolidBrush bgBR(Gdiplus::Color(82, 87, 92));
		g.FillRectangle(&bgBR, 1, 1, rect.Width() - 2, rect.Height() - 2);

		Gdiplus::SolidBrush trBR(Gdiplus::Color(255, 255, 0));
		g.FillRectangle(&outBR, 1, 31, rect.Width() - 2, 1);
		g.FillRectangle(&trBR, 2, 32, rect.Width() - 4, rect.Height() - 34);

		CString strFont = _S(IDS_FONT);
		Gdiplus::Font titleFont(strFont, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Gdiplus::SolidBrush textBR(Gdiplus::Color(255, 255, 255));
		Gdiplus::StringFormat textSF;
		textSF.SetAlignment(Gdiplus::StringAlignmentNear);
		textSF.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		CString strDraw = _S(IDS_TITLE);

		Gdiplus::RectF boundRect;
		g.MeasureString(strDraw, -1, &titleFont, Gdiplus::PointF(7, 1), &boundRect);
		boundRect.Height = 30;
		g.DrawString(strDraw, -1, &titleFont, boundRect, &textSF, &textBR);

		graphics.DrawImage(&bitmap, 0, 0);

	}
}


void CPositionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rw;
	GetClientRect(&rw);

	if ((point.x >= rw.left && point.x <= 5) && (point.y >= rw.top && point.y <= 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else if ((point.x <= rw.right && point.x >= rw.right - 5) && (point.y >= rw.top && point.y <= 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
	else if ((point.x >= rw.left && point.x <= 5) && (point.y <= rw.bottom && point.y >= rw.bottom - 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
	else if ((point.x <= rw.right && point.x >= rw.right - 5) && (point.y <= rw.bottom && point.y >= rw.bottom - 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else if ((point.x <= rw.right && point.x >= rw.right - 5) && (point.y >= rw.top + 15 && point.y <= rw.bottom - 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	else if ((point.x >= rw.left && point.x <= 5) && (point.y > rw.top + 15 && point.y <= rw.bottom - 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	else if ((point.x >= rw.left + 5 && point.x <= rw.right - 5) && (point.y >= rw.top && point.y <= 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
	else if ((point.x >= rw.left + 5 && point.x <= rw.right - 5) && (point.y <= rw.bottom && point.y >= rw.bottom - 5))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	CDialogEx::OnMouseMove(nFlags, point);
}


void CPositionDlg::OnBnClickedCheckUse()
{
//	m_btnUse.SetCheck(!m_btnUse.GetCheck());

	if (m_btnUse.GetCheck())
	{
		//Config::SaveIsShareUse(_T("1"));
		m_ini["SHARE"]["USE"] = 1;
		m_btnSave.EnableWindow(TRUE);
	}
	else
	{
		//Config::SaveIsShareUse(_T("0"));
		m_ini["SHARE"]["USE"] = 0;
		m_btnSave.EnableWindow(FALSE);
	}
}


typedef HRESULT(WINAPI* DwmIsCompositionEnabledFunction)(__out BOOL* isEnabled);
typedef HRESULT(WINAPI* DwmGetWindowAttributeFunction) (
	__in  HWND hwnd,
	__in  DWORD dwAttribute,
	__out PVOID pvAttribute,
	DWORD cbAttribute
	);
void CPositionDlg::OnBnClickedBtnSave()
{
	//20260514 scpark
	//저장버튼을 클릭하면 1,1 오프셋 다운 효과로 그려지는데 마우스를 떼자마자 바로 메시지박스가 표시되면
	//그 효과가 가려진다. 약간의 인위적인 딜레이를 준다.
	Wait(500);

	CRect rect;
	//if (CUtil::GetIsXP())
	if (get_windows_major_version() < 6)
	{
		GetWindowRect(&rect);
	}
	else
	{
		HINSTANCE dwmapiDllHandle = NULL;
		dwmapiDllHandle = LoadLibrary(_T("dwmapi.dll"));
		if (dwmapiDllHandle == NULL)
		{
			GetWindowRect(&rect);
		}
		else
		{
			DwmGetWindowAttributeFunction DwmGetWindowAttribute;
			DwmGetWindowAttribute = (DwmGetWindowAttributeFunction) ::GetProcAddress(dwmapiDllHandle, "DwmGetWindowAttribute");
			if (DwmGetWindowAttribute == NULL)
			{
				GetWindowRect(&rect);
			}
			else
			{
				DwmGetWindowAttribute(GetSafeHwnd(), DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
			}
			//DwmGetWindowAttribute(GetSafeHwnd(), DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
		}
		FreeLibrary(dwmapiDllHandle);
	}

	//Config::SaveShareLeft(rect.left + 2);
	//Config::SaveShareTop(rect.top + 32);
	//Config::SaveShareWidth(rect.Width() - 4);
	//Config::SaveShareHeight(rect.Height() - 34);
	m_ini["SHARE"]["LEFT"] = rect.left + 2;
	m_ini["SHARE"]["TOP"] = rect.top + 32;
	m_ini["SHARE"]["WIDTH"] = rect.Width() - 4;
	m_ini["SHARE"]["HEIGHT"] = rect.Height() - 34;

	//CMessageDlg dlgMessage;
	//dlgMessage.SetMessage(_S(IDS_SAVE_POS));
	//dlgMessage.DoModal();
	theApp.m_msgbox.DoModal(_S(IDS_SAVE_POS));
}


void CPositionDlg::OnBnClickedBtnClose()
{
	EndDialog(IDOK);
}


void CPositionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_btnClose.m_hWnd == nullptr)
		return;

	CRect rect;

	GetClientRect(rect);

	m_btnClose.MoveWindow(rect.Width() - 26, 6, 20, 20);
	m_btnUse.MoveWindow(rect.Width() - 26 - 37, 6, 32, 20);

	rect.top += 30;
	m_btnSave.MoveWindow((rect.Width() - m_btnSave.width()) / 2, rect.top + ((rect.Height() - m_btnSave.height()) / 2), m_btnSave.width(), m_btnSave.height());

	Invalidate(FALSE);
}

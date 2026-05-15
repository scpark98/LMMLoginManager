// DeviceNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "DeviceNameDlg.h"
#include "DeviceGroupDlg.h"

#include "Common/MemoryDC.h"
#include "Common/win_compat/dwm.h"

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
	win_compat::dwm::set_window_corner_round(m_hWnd);

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
	m_edit_name.set_text_color(Gdiplus::Color::RoyalBlue);// Gdiplus::Color::Black);
	m_edit_name.set_back_color(Gdiplus::Color::White);
	m_edit_name.set_limit_text(250);
	m_cur_dev_name = theApp.m_ini["SERVER"]["DNAME"];
	m_edit_name.set_text(m_cur_dev_name);
	m_edit_name.set_font_weight(FW_SEMIBOLD);

	m_button_ok.set_text_color(get_weak_color(m_theme.cr_back, -128));// m_theme.cr_title_back_inactive);
	m_button_ok.set_back_color(get_weak_color(m_theme.cr_back, 128));
	m_button_ok.set_font_weight(FW_SEMIBOLD);

	m_button_cancel.set_text_color(m_theme.cr_title_back_inactive);
	m_button_cancel.set_back_color(m_theme.cr_title_text);
	m_button_cancel.set_font_weight(FW_SEMIBOLD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceNameDlg::OnBnClickedOk()
{
	CString new_dev_name = m_edit_name.get_text();

	if (new_dev_name == m_cur_dev_name)
	{
		theApp.m_msgbox.DoModal(_S(IDS_ALREADY_USE_NAME), MB_ICONEXCLAMATION);
		return;
	}

	//사용자가 입력 후 엔터를 치면
	//1.입력 시 이미 존재하는지 확인, 중복된 이름이면 팝업 메시지 표시하고 종료
	//2.유효한 입력이면 이름변경 API 호출
	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString mgr_id = theApp.m_ini["LOGIN"]["ID"];
	CString header = _T("token: ") + agent_token + _T("\r\n");
	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/GetLmmManagerInfo"), _T("POST"));

	param.body.Format(_T("{\"id\":\"%s\"}"), mgr_id);
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param, true);

	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		logWriteE(_T("fail GetLmmManagerInfo."));
		theApp.m_msgbox.DoModal(_S(IDS_FAIL_LOAD_USER), MB_ICONEXCLAMATION);
		OnBnClickedCancel();
	}
	else
	{
		logWrite(_T("success GetLmmManagerInfo"));

		Json json;
		if (!json.parse(param.result))
		{
			theApp.m_msgbox.DoModal(_T("읽어온 매니저 정보 데이터에 오류가 있습니다."), MB_ICONEXCLAMATION);
			return;
		}

		rapidjson::Value& objs = json.doc["objects"];

		if (objs.Size() > 0)
		{
			theApp.set_company_key(objs[0]["company_fk"].GetInt());

			// 장치명이 동일하다면
			if (new_dev_name == m_cur_dev_name)
			{
				//agnet에서 "-name -first"로 호출된 경우 그룹선택창까지 표시한다.
				if (__argc >= 2)
				{
					CString execParameter(__targv[2]);
					if (execParameter == _T("-first"))
					{
						CDeviceGroupDlg dlgGroup;
						dlgGroup.DoModal();
					}
				}

				EndDialog(IDOK);
				return;
			}

			//입력된 장치명이 기존 장치명과 다를 경우 중복체크로 넘어간다.
			logWrite(_T("before call DuplicateCheck..."));
			//Api::DuplicateCheck(this, m_strInputName, m_strCompanyKey);
			if (is_dev_name_duplicated(new_dev_name))
				return;
		}
	}

	CDialogEx::OnOK();
}

bool CDeviceNameDlg::is_dev_name_duplicated(CString new_dev_name)
{
	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString header = _T("token: ") + agent_token + _T("\r\n");
	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/GetLmmDeviceDuplicateCheck"), _T("POST"));

	param.body.Format(_T("{\"device_name\":\"%s\", \"company_fk\":%d}"), new_dev_name, theApp.get_company_key());
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param, true);

	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		logWriteE(_T("fail GetLmmDeviceDuplicateCheck."));
		theApp.m_msgbox.DoModal(_S(IDS_FAIL_DEVICE_DUP_CHECK), MB_ICONEXCLAMATION);
		m_button_ok.EnableWindow(TRUE);
		return true;
	}
	else
	{
		logWrite(_T("success GetLmmDeviceDuplicateCheck"));

		Json json;
		if (!json.parse(param.result))
		{
			theApp.m_msgbox.DoModal(_T("장치명 중복체크 요청에 대한 응답 데이터에 오류가 있습니다."), MB_ICONEXCLAMATION);
			return true;
		}

		rapidjson::Value& objs = json.doc["objects"];

		CString server_dev_name;

		if (objs.Size() > 0)
		{
			if (objs[0].HasMember("device_name"))
				server_dev_name = objs[0]["device_name"].GetCString();

			if (server_dev_name.IsEmpty() == false)
			{
				theApp.m_msgbox.DoModal(_S(IDS_ALREADY_USE_NAME), MB_ICONEXCLAMATION);
				m_button_ok.EnableWindow(TRUE);
			}
			else
			{
				if (!set_dev_name(new_dev_name))
					return true;
				return false;
			}
		}
	}

	return true;
}

bool CDeviceNameDlg::set_dev_name(CString new_dev_name)
{
	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString device_id = theApp.m_ini["SERVER"]["DID"];
	CString header = _T("token: ") + agent_token + _T("\r\n");

	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/UpdateLmmDeviceName"), _T("POST"));

	param.body.Format(_T("{\"device_id\":\"%s\",\"old_name\":\"%s\",\"new_name\":\"%s\"}"), device_id, m_cur_dev_name, new_dev_name);
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param, true);

	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		logWriteE(_T("fail UpdateLmmDeviceName."));
		theApp.m_msgbox.DoModal(_S(IDS_FAIL_UPDATE_NAME), MB_ICONEXCLAMATION);
		m_button_ok.EnableWindow(TRUE);
		return false;
	}
	else
	{
		logWrite(_T("success UpdateLmmDeviceName"));

		Json json;
		if (!json.parse(param.result))
		{
			theApp.m_msgbox.DoModal(_T("장치명 변경 요청에 대한 응답 데이터에 오류가 있습니다."), MB_ICONEXCLAMATION);
			m_button_ok.EnableWindow(TRUE);
			return false;
		}

		if (__argc > 2 && CString(__targv[2]) == _T("-first"))
		{
			CDeviceGroupDlg	dlg;
			dlg.DoModal();
		}

		theApp.m_ini["SERVER"]["DNAME"] = new_dev_name;

		//20260514 scpark
		//device name이 변경되면 에이전트를 재시작해주기 위해
		//최근까지는 AutoPatcher까지 실행했으나 에이전트도 재시작하지 않아도 정상 적용되고 있다.
		//좀 더 테스트 확인 필요!

		return true;
	}

	return false;
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

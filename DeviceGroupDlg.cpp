// DeviceGroupDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "DeviceGroupDlg.h"

#include "Common/MemoryDC.h"
#include "Common/win_compat/dwm.h"


// CDeviceGroupDlg 대화 상자

IMPLEMENT_DYNAMIC(CDeviceGroupDlg, CDialogEx)

CDeviceGroupDlg::CDeviceGroupDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEVICE_GROUP, pParent)
{

}

CDeviceGroupDlg::~CDeviceGroupDlg()
{
}

void CDeviceGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_static_title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button_close);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_STATIC_BUILD_PROCESSING, m_static_build_processing);
	DDX_Control(pDX, IDOK, m_button_ok);
}


BEGIN_MESSAGE_MAP(CDeviceGroupDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeviceGroupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeviceGroupDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDeviceGroupDlg::OnBnClickedButtonClose)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDeviceGroupDlg 메시지 처리기

BOOL CDeviceGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	win_compat::dwm::set_window_corner_round(m_hWnd);

	//m_theme.set_color_theme(CSCColorTheme::color_theme_dark_gray);
	//m_theme.set_color_theme(CSCColorTheme::color_theme_linkmemine);
	m_theme.copy_colors_from(theApp.m_theme);

	m_static_title.set_color_theme(m_theme);
	m_static_title.set_text_color(m_theme.cr_title_text);
	m_static_title.set_back_color(m_theme.cr_title_back_inactive);
	m_static_title.set_font_weight(FW_BOLD);
	m_static_title.set_text(_T("  ") + _S(IDS_TITLE));

	//m_button_close.set_color_theme(m_theme);
	m_button_close.add_image(IDB_CLOSE);
	m_button_close.set_back_color(m_theme.cr_title_back_inactive);

	m_static_build_processing.set_color_theme(m_theme);
	m_static_build_processing.set_text_color(m_theme.cr_text);
	m_static_build_processing.set_back_color(m_theme.cr_back);
	m_static_build_processing.set_blink(true, 800, 600);
	m_static_build_processing.set_text(_S(IDS_GROUP_LOADING_INFO));

	m_button_ok.set_color_theme(m_theme);
	m_button_ok.set_text_color(m_theme.cr_title_back_inactive);
	m_button_ok.set_back_color(m_theme.cr_title_text);
	m_button_ok.set_font_weight(FW_BOLD);

	m_tree.set_color_theme(m_theme);
	m_tree.set_draw_border(false);

	//트리 배경을 dlg 배경과 분리해서 "패널" 느낌. cr_back(64,64,64) 보다 +12 밝게.
	//m_tree.set_back_color(get_weak_color(m_theme.cr_back, 12));

	//트리는 본 화면의 주역이므로 폰트를 키워 가독성 강조 (기본 9 → 11pt).
	m_tree.set_font_size(10);

	//IDB_TABLET PNG 을 트리 imagelist 로 등록.
	//CSCGdiplusBitmap::load(UINT) 가 PNG 리소스 로딩 후 Gdiplus::Bitmap 으로 보관.
	//Gdiplus::Bitmap::GetHICON 으로 HICON 변환 → 기존 set_imagelist(vector<HICON>) 사용.
	//imagelist 가 HICON 을 내부 복사하므로 등록 후 즉시 DestroyIcon 으로 해제.
	{
		CSCGdiplusBitmap img;
		std::vector<HICON> icons;
		if (img.load(UINT(IDB_TABLET)) && img.m_pBitmap)
		{
			HICON h = NULL;
			if (img.m_pBitmap->GetHICON(&h) == Gdiplus::Ok && h)
				icons.push_back(h);
		}

		if (!icons.empty())
		{
			m_tree.set_imagelist(icons, 16);
			for (HICON h : icons)
				::DestroyIcon(h);
		}
	}

	//트리 주변에 좌우 padding 추가 — dlg edge 에 붙어있던 답답함 해소.
	{
		CRect rc;
		m_tree.GetWindowRect(rc);
		ScreenToClient(rc);
		rc.DeflateRect(8, 4, 8, 4);
		m_tree.MoveWindow(rc);
	}

	SetTimer(timer_get_group_list, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceGroupDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = m_tree.GetSelectedItem();
	auto data = (GroupInfo*)m_tree.GetItemData(hItem);
	
	trace(data->GetGroupName());

	CString header = _T("token: ") + theApp.m_ini["LOGIN"]["TOKEN"].to_CString() + _T("\r\n");

	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/UpdateLmmDeviceGroup"), _T("POST"));

	param.body.Format(_T("{\"device_id\":\"%s\", \"group_fk\":%s}"), theApp.m_ini["SERVER"]["DID"].to_CString(), data->GetGroupNum());
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param);

	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		logWriteE(_T("fail UpdateLmmDeviceGroup."));
		theApp.m_msgbox.DoModal(_S(IDS_FAIL_SET_GROUP), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		logWrite(_T("success UpdateLmmDeviceGroup. moved to %s"), data->GetGroupName());

		Json json;
		if (!json.parse(param.result))
		{
			logWriteE(_T("json.parse"));
			theApp.m_msgbox.DoModal(_T("읽어온 그룹정보에 오류가 있습니다."));
			return;
		}
	}

	CDialogEx::OnOK();
}

void CDeviceGroupDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CDeviceGroupDlg::OnPaint()
{
	CPaintDC dc1(this);

	CRect rc;
	GetClientRect(rc);

	CMemoryDC dc(&dc1, &rc);
	dc.FillSolidRect(rc, m_theme.cr_back.ToCOLORREF());
}

void CDeviceGroupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDeviceGroupDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}

void CDeviceGroupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_get_group_list)
	{
		KillTimer(timer_get_group_list);
		get_group_list();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDeviceGroupDlg::get_group_list()
{
	CString agent_token = theApp.m_ini["LOGIN"]["TOKEN"];
	CString mgr_id = theApp.m_ini["LOGIN"]["ID"];
	CString header = _T("token: ") + agent_token + _T("\r\n");

	CRequestUrlParams param(theApp.m_ip, theApp.m_port, _T("/agent/api/v1.0/GetLmmGroupList"), _T("POST"));

	param.body.Format(_T("{\"company_fk\":%d, \"mgrid\":\"%s\"}"), theApp.get_company_key(), mgr_id);
	param.headers.push_back(header);

	logWrite(_T("param : %s"), param.get_param_str());
	request_url(&param);

	logWrite(_T("status = %d, result = %s"), param.status, param.result);

	if (param.status != HTTP_STATUS_OK)
	{
		logWriteE(_T("fail GetLmmGroupList."));
		theApp.m_msgbox.DoModal(_S(IDS_FAIL_LOAD_GROUP));
		OnBnClickedCancel();
	}
	else
	{
		logWrite(_T("success GetLmmGroupList"));

		Json json;
		if (!json.parse(param.result))
		{
			theApp.m_msgbox.DoModal(_T("읽어온 그룹정보에 오류가 있습니다."));
			return;
		}

		rapidjson::Value& objs = json.doc["objects"];

		//std::vector<std::map<CString, CString>> getManagerResult;
		//json.array_to_map(param.result, "objects", &getManagerResult);

		if (objs.Size() == 0)
		{
#if defined(_LINKMEMINE_10)
			theApp.m_msgbox.DoModal(_T("설정된 그룹 목록이 존재하지 않습니다.\n그룹 지정이 필요할 경우 웹페이지에서 \"원격 관리\" 모드로 전환한 후 원하는 그룹을 추가하시기 바랍니다."));
#else
			theApp.m_msgbox.DoModal(_T("설정된 그룹 목록이 존재하지 않습니다.\n그룹 지정이 필요할 경우 웹페이지에서 원하는 그룹을 추가하시기 바랍니다."));
#endif
			OnBnClickedCancel();
			return;
		}

		for (size_t i = 0; i < objs.Size(); i++)
		{
			CString strGroupNum = i2S(objs[i]["groupnum"].GetInt());
			CString strGroupName = objs[i]["name"].GetCString();
			CString strCreateTime = objs[i]["create_time"].GetCString();
			CString strUpdateTime = objs[i]["update_time"].GetCString();
			CString strEmail = objs[i]["email"].GetCString();
			CString strPhone = objs[i]["phone"].GetCString();
			CString strFlagDfGroup = objs[i]["flag_df_group"].GetBool() ? CString("1") : CString("0");
			CString strFlagConfirm = objs[i]["flag_confirm"].GetBool() ? CString("1") : CString("0");
			CString strCompanyFk = i2S(objs[i]["company_fk"].GetInt());
			CString strGroupFk = i2S(objs[i]["group_fk"].GetInt());
			CString strRootGroupFk = i2S(objs[i]["root_group_fk"].GetInt());

			GroupInfo* group = new GroupInfo();
			group->SetGroupNum(strGroupNum);
			group->SetGroupName(strGroupName);
			group->SetCreateTime(strCreateTime);
			group->SetUpdateTime(strUpdateTime);
			group->SetManagerEmail(strEmail);
			group->SetManagerPhone(strPhone);
			group->SetFlagDfGroup(_ttoi(strFlagDfGroup));
			group->SetFlagConfirm(_ttoi(strFlagConfirm));
			group->SetCompanyFk(strCompanyFk);
			group->SetParentGroupFk(strGroupFk);
			group->SetRootGroupFk(strRootGroupFk);
			m_groups.push_back(group);

			/*
			if (strRootGroupFk == _T("-1")) // 최상위 그룹
			{
				if (strGroupName != _T(""))
				{
					if (group->GetFlagDfGroup())
					{
						strGroupName.LoadString(IDS_DEFAULT_GROUP);
						m_selFirstGroup = group;

						CRect rect;
						GetClientRect(&rect);
						CRect drawRect(92, rect.Height() - 58, 272, rect.Height() - 18);
						InvalidateRect(&drawRect);
					}

					m_listFirstGroup.InsertItem(nCount, strGroupName);
					m_listFirstGroup.SetItemData(nCount, (DWORD)group);
					++nCount;
				}
			}
			*/
		}

		//scpark 20240503. group을 gfk, gn 두 필드를 기준으로 오름차순으로 정렬한다.
		//그래야만 tree를 구성할 때 parent node가 아직 트리에 추가되지 않은 상태에서 child node를 추가해야하는 등의 상황이 발생하지 않는다.
		std::sort(m_groups.begin(), m_groups.end(),
			[](GroupInfo* a, GroupInfo* b)
			{
				int apfk = _ttoi(a->GetParentGroupFk());
				int bpfk = _ttoi(b->GetParentGroupFk());
				//만약 parent가 같다면 groupNum으로 정렬한다.
				if (apfk == bpfk)
				{
					return (a->GetGroupNum() < b->GetGroupNum());
				}
				else
				{
					return (apfk < bpfk);
				}
			}
		);

		for (int i = 0; i < m_groups.size(); i++)
		{
			TRACE(_T("%s (gn:%s, gfk:%s)\n"), m_groups[i]->GetGroupName(), m_groups[i]->GetGroupNum(), m_groups[i]->GetParentGroupFk());
		}

		//이제 m_groups 정보를 이용해서 트리로 표시한다.
		build_tree();
	}
}

HTREEITEM CDeviceGroupDlg::get_parent_node(CString parentGroupFk)
{
	for (int i = 0; i < m_groups.size(); i++)
	{
		if (m_groups.at(i)->GetGroupNum() == parentGroupFk)
		{
			return m_groups.at(i)->m_hItem;
		}
	}

	return TVI_ROOT;
}

void CDeviceGroupDlg::build_tree()
{
	HTREEITEM hItem = TVI_ROOT;

	for (int i = 0; i < m_groups.size(); i++)
	{
		//pfk를 가진 노드를 구해서 현재 노드를 추가한다.
		if (m_groups.at(i)->GetParentGroupFk() == _T("") || m_groups.at(i)->GetParentGroupFk() == _T("-1"))
		{
			//pgfk가 -1이면 최상위 노드들의 경우이다.
			hItem = (m_tree.get_use_root() ? m_tree.get_root_item() : nullptr);
		}
		else
		{
			//pgfk가 -1이 아니면 해당 값을 가진 노드(parent node)를 구한다.
			hItem = get_parent_node(m_groups.at(i)->GetParentGroupFk());
		}

		//트리에 넣고 HTREEITEM 값을 기억해놓는다.
		//imagelist 에는 윈도우 폴더 아이콘(index 0) 만 등록되어 있으므로 모든 그룹이 동일 아이콘.
		//그룹 종류별로 다른 아이콘이 필요해지면 OnInitDialog 의 set_imagelist 에 추가하고 분기.
		int img_idx = 0;

		m_groups.at(i)->m_hItem = m_tree.InsertItem(m_groups.at(i)->GetGroupName(), img_idx, img_idx, hItem, TVI_LAST);

		//해당 노드에 그룹정보를 연결시킨다.
		m_tree.SetItemData(m_groups.at(i)->m_hItem, (DWORD_PTR)m_groups.at(i));
	}

	m_tree.expand_all();

	m_static_build_processing.set_blink(false);
	m_static_build_processing.ShowWindow(SW_HIDE);
	m_tree.ShowWindow(SW_SHOW);
	Wait(100);
	m_tree.select_item(m_groups[0]->m_hItem);
}

void CDeviceGroupDlg::OnDestroy()
{
	for (int i = 0; i < m_groups.size(); i++)
	{
		GroupInfo* pGroup = (GroupInfo*)m_groups[i];
		delete pGroup;
		pGroup = nullptr;
	}

	CDialogEx::OnDestroy();
}

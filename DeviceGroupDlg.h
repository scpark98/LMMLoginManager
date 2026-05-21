#pragma once
#include "afxdialogex.h"

#include "Common/CTreeCtrl/SCTreeCtrl/SCTreeCtrl.h"

#include "GroupInfo.h"

// CDeviceGroupDlg 대화 상자

class CDeviceGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceGroupDlg)

public:
	CDeviceGroupDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDeviceGroupDlg();

	CSCColorTheme		m_theme = CSCColorTheme(this);

	bool				m_build_tree_completed = false;
	void				get_group_list();
	void				build_tree();
	HTREEITEM			get_parent_node(CString parentGroupFk);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEVICE_GROUP };
#endif

protected:
	enum TIMER_ID
	{
		timer_get_group_list = 1,
	};

	std::vector<GroupInfo*> m_groups;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClose();
	CSCStatic m_static_title;
	CGdiButton m_button_close;
	CGdiButton m_button_exit;
	CSCTreeCtrl m_tree;
	CSCStatic m_static_build_processing;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CGdiButton m_button_ok;
	afx_msg void OnDestroy();
};

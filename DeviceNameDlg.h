#pragma once
#include "afxdialogex.h"

#include "Common/CEdit/CSCStaticEdit/SCStaticEdit.h"

// CDeviceNameDlg 대화 상자

class CDeviceNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceNameDlg)

	CSCColorTheme		m_theme = CSCColorTheme(this);
	CString				m_cur_dev_name;

	//장치명이 중복인지 요청하고 요청 실패 또는 중복이라면 true를 리턴한다.
	//중복이 아니라면 false를 리턴한다.
	bool				is_dev_name_duplicated(CString new_dev_name);
	bool				set_dev_name(CString new_dev_name);

public:
	CDeviceNameDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDeviceNameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEVICE_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClose();
	CSCStatic m_static_title;
	CGdiButton m_button_close;
	CSCStatic m_static_message;
	CSCStaticEdit m_edit_name;
	CGdiButton m_button_ok;
	CGdiButton m_button_cancel;
};

#pragma once
#include "afxdialogex.h"

#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CButton/GdiButton/GdiButton.h"

// CVersionDlg 대화 상자

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CVersionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CVersionDlg();

	CSCColorTheme		m_theme;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSION };
#endif

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

	CSCStatic m_static_version_label;
	CSCStatic m_static_version;
	CSCStatic m_static_copyright;
};

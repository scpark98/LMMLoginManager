
// LMMLoginManagerDlg.h: 헤더 파일
//

#pragma once

#include "Common/colors.h"
#include "Common/CEdit/CSCStaticEdit/SCStaticEdit.h"
#include "Common/CButton/GdiButton/GdiButton.h"

// CLMMLoginManagerDlg 대화 상자
class CLMMLoginManagerDlg : public CDialogEx
{
// 생성입니다.
public:
	CLMMLoginManagerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LMMLOGINMANAGER_DIALOG };
#endif

protected:
	CSCColorTheme m_theme = CSCColorTheme(this);

	void			init_dialog();
	void			init_controls();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CGdiButton m_button_config;
	CGdiButton m_button_minimize;
	CGdiButton m_button_close;
	CSCStaticEdit m_edit_id;
	CSCStaticEdit m_edit_pw;
	CGdiButton m_check_save_pw;
	CGdiButton m_check_auto_login;
	CGdiButton m_button_login;
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonMinimize();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedCheckSavePw();
	afx_msg void OnBnClickedCheckAutoLogin();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};

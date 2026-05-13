#pragma once
#include "afxwin.h"

#include "Common/ini/EZIni/EZIni.h"
#include "Common/messagebox/CSCMessageBox/SCMessageBox.h"

// CPositionDlg 대화 상자입니다.

class CPositionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPositionDlg)

	EZIni				m_ini;
	CSCMessageBox		m_msgbox;

public:
	CPositionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPositionDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POSITION };
#endif

protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	CGdiButton m_btnUse;
	CGdiButton m_btnSave;
	CGdiButton m_btnClose;

	CString m_strProduct;
	afx_msg void OnBnClickedCheckUse();
};

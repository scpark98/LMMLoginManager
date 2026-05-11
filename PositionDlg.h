#pragma once
#include "afxdialogex.h"


// CPositionDlg 대화 상자

class CPositionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPositionDlg)

public:
	CPositionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPositionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POSITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

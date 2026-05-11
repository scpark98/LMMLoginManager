#pragma once
#include "afxdialogex.h"


// CDeviceGroupDlg 대화 상자

class CDeviceGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceGroupDlg)

public:
	CDeviceGroupDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDeviceGroupDlg();

	void			set_company_key(CString company_key) { m_company_key = company_key; }

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEVICE_GROUP };
#endif

protected:
	CString			m_company_key;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

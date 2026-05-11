#pragma once
#include "afxdialogex.h"


// CDeviceNameDlg 대화 상자

class CDeviceNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceNameDlg)

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
};

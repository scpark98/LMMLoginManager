// DeviceNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "DeviceNameDlg.h"


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
}


BEGIN_MESSAGE_MAP(CDeviceNameDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeviceNameDlg 메시지 처리기

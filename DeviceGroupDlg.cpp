// DeviceGroupDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "DeviceGroupDlg.h"


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
}


BEGIN_MESSAGE_MAP(CDeviceGroupDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeviceGroupDlg 메시지 처리기

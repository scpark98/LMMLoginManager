// PositionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LMMLoginManager.h"
#include "afxdialogex.h"
#include "PositionDlg.h"


// CPositionDlg 대화 상자

IMPLEMENT_DYNAMIC(CPositionDlg, CDialogEx)

CPositionDlg::CPositionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POSITION, pParent)
{

}

CPositionDlg::~CPositionDlg()
{
}

void CPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPositionDlg, CDialogEx)
END_MESSAGE_MAP()


// CPositionDlg 메시지 처리기
